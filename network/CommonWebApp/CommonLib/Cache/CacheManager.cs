using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections.Concurrent;
using System.Threading;
using System.Threading.Tasks;

namespace CommonLib.Cache
{
    //This class is used to manage the cache. It starts a background thread to see if there's any
    //cache items expired. If found one, the CacheManager will try to call "RefreshAction" of 
    //that item to refresh it. If there's no "RefreshAction", the expired item will be discarded directly.
    //If the cache items are added without expiration, they will NEVER be expired.
    public class CacheManager
    {
        private static readonly object DUMMY = new object();

        private static readonly int MIN_WAIT_TIME = 5;

        //The cache item saved in this dictionary
        private ConcurrentDictionary<string, CacheItem> m_cacheItems = null;

        //All modify operations for the cache are processed by dispatching thread
        private ConcurrentQueue<OperationEvent> m_messages = null;

        //A min heap for task scheduler
        private SortedDictionary<SchedulerKey, object> m_heap = null;

        //A auto reset event for processing user actions events and timer events
        private AutoResetEvent m_waitHandle = null;

        //Is this scheduler thread still running?
        private bool m_running = false;

        public CacheManager()
        {
            m_cacheItems = new ConcurrentDictionary<string, CacheItem>();
            m_messages = new ConcurrentQueue<OperationEvent>();
            m_heap = new SortedDictionary<SchedulerKey, object>();
            m_waitHandle = new AutoResetEvent(false);

            //start to run scheduler
            m_running = true;
            Task.Factory.StartNew(() => Run());
        }

        public int Count()
        {
            return m_cacheItems.Count;
        }

        public void Add(string key, object value)
        {
            CacheItem ci = new CacheItem(key, value);
            PostEvent(OperationType.OP_ADD, ci);
        }

        public void Add(string key, object value, ICacheItemRefreshAction refreshAction, long expiration)
        {
            CacheItem ci = new CacheItem(key, value, refreshAction, expiration);
            PostEvent(OperationType.OP_ADD, ci);
        }

        public void Add(string key, object value, ICacheItemRefreshAction refreshAction, ICacheItemExpiration expiration)
        {
            CacheItem ci = new CacheItem(key, value, refreshAction, expiration);
            PostEvent(OperationType.OP_ADD, ci);
        }

        public bool Contains(string key)
        {
            return m_cacheItems.ContainsKey(key);
        }

        public object GetData(string key)
        {
            CacheItem ci = null;
            if (!m_cacheItems.TryGetValue(key, out ci))
            {
                return null;
            }

            return ci.Value;
        }

        public object this[string key]
        {
            get
            {
                return GetData(key);
            }
        }

        public void Remove(string key)
        {
            CacheItem ci = null;
            if (m_cacheItems.TryGetValue(key, out ci))
            {
                PostEvent(OperationType.OP_REMOVE, ci);
            }
        }

        private void PostEvent(OperationType operType, object operObj)
        {
            OperationEvent cme = new OperationEvent(operType, operObj);
            m_messages.Enqueue(cme);
            m_waitHandle.Set();
        }

        private void Run()
        {
            LogManager.Info("CacheManager:Run", "Start to run");
            while (m_running)
            {
                try
                {
                    //1. process user action events
                    if (!ProcessOperationEvents())
                    {
                        continue;
                    }

                    //2. process timer events
                    ProcessTimerEvents();

                    //3. wait events
                    if (m_heap.Count <= 0)
                    {
                        //wait until there's an event comming
                        m_waitHandle.WaitOne();
                    }
                    else
                    {
                        SchedulerKey sk = m_heap.First().Key;

                        //wait for operation events or timer events
                        TimeSpan ts = sk.NextExpirationTime - DateTime.Now;
                        if (ts.TotalMilliseconds >= MIN_WAIT_TIME)
                        {
                            m_waitHandle.WaitOne(ts);
                        }
                    }
                }
                catch (Exception ex)
                {
                    LogManager.Error("CacheManager:Run", ex);
                }
            }
            LogManager.Info("CacheManager:Run", "Stopped to run");
        }

        //This function is triggered by public API call, such as Add/Remove etc.
        //Only this dispatching thread CAN modify the content of CacheManager, but it is allowed to read by any thread.
        private bool ProcessOperationEvents()
        {
            OperationEvent cme = null;
            while (!m_messages.IsEmpty)
            {
                //This should not happen. If it does, then this is a critial bug!
                if (!m_messages.TryDequeue(out cme))
                {
                    LogManager.Warn("CacheManager:Assert Failure", "Failed to dequeue a message");
                    continue;
                }

                if (!ProcessOperationEvent(cme))
                {
                    return false;
                }
            }
            return true;
        }

        private bool ProcessOperationEvent(OperationEvent cme)
        {
            bool shouldContinue = true;
            switch (cme.OperType)
            {
                case OperationType.OP_ADD:
                    {
                        shouldContinue = DoAdd(cme.OperObject as CacheItem);
                    }
                    break;

                case OperationType.OP_REMOVE:
                    {
                        shouldContinue = DoRemove(cme.OperObject as CacheItem);
                    }
                    break;

                case OperationType.OP_STOP:
                    {
                        m_running = false;
                        shouldContinue = false;
                    }
                    break;
                default:
                    {
                        LogManager.Warn("CacheManager:ProcessUserAction", "Unsupported operation: " + (int)cme.OperType);
                    }
                    break;
            }

            return shouldContinue;
        }

        private bool DoAdd(CacheItem ci)
        {
            if (ci == null)
            {
                LogManager.Warn("CacheManager:Assert Failure", "DoAdd a null item");
                return true;
            }

            CacheItem existingItem = null;

            //If the key exists, remove it firstly, then re-add it again
            if (m_cacheItems.TryRemove(ci.Key, out existingItem))
            {
                SchedulerKey existingSK = new SchedulerKey(existingItem.NextExpirationTime, existingItem.Key);
                if (!m_heap.ContainsKey(existingSK))
                {
                    //This should not happen. If it does, then this is a critial bug!
                    LogManager.Warn("CacheManager:Assert Failure", string.Format("The key [{0}] exists in CacheItems but not in SchedulerKeys", existingItem.Key));
                }
                else
                {
                    m_heap.Remove(existingSK);
                }
            }

            //Cache Item
            if (!m_cacheItems.TryAdd(ci.Key, ci))
            {
                //This should not happen. If it does, then this is a critial bug!
                LogManager.Warn("CacheManager:Assert Failure", string.Format("The key [{0}] add failed", ci.Key));
            }

            //And the SchedulerKey
            SchedulerKey sk = new SchedulerKey(ci.NextExpirationTime, ci.Key);
            if (m_heap.ContainsKey(sk))
            {
                //This should not happen. If it does, then this is a critial bug!
                LogManager.Warn("CacheManager:Assert Failure", string.Format("The SchedulerKey key[{0}] is already exising", sk.ToString()));
            }
            else
            {
                m_heap.Add(sk, DUMMY);
            }

            LogManager.Info("CacheManager:DoAdd", string.Format("The key [{0}] is added, current={1}, original={2}", 
                ci.Key, ci.ToString(), ((existingItem == null) ? "null" : existingItem.ToString())));
            
            return true;
        }

        private bool DoRemove(CacheItem ci)
        {
            if (ci == null)
            {
                LogManager.Warn("CacheManager:Assert Failure", "DoRemove a null item");
                return true;
            }

            CacheItem value = null;
            if (!m_cacheItems.TryRemove(ci.Key, out value))
            {
                LogManager.Warn("CacheManager:DoRemove", string.Format("The CacheItem key[{0}] is already removed", ci.Key));
            }

            SchedulerKey sk = new SchedulerKey(ci.NextExpirationTime, ci.Key);
            if (!m_heap.ContainsKey(sk))
            {
                LogManager.Warn("CacheManager:DoRemove", string.Format("The SchedulerKey key[{0}] is already removed", sk.ToString()));
            }
            else
            {
                m_heap.Remove(sk);
            }

            LogManager.Info("CacheManager:DoRemove", string.Format("The key [{0}] is removed", ci.Key));
            return true;
        }

        //This function is triggered by timer. If a key is expired, this function will 
        //try to refresh the value if there's a refresh action associated with it
        private void ProcessTimerEvents()
        {
            if (m_heap.Count <= 0)
            {
                return;
            }

            //The first key is the minimum value to be the first expiration value
            SchedulerKey sk = m_heap.First().Key;

            //Check if this is really an expiration event?
            TimeSpan ts = sk.NextExpirationTime - DateTime.Now;
            //not yet, wait
            if (ts.TotalMilliseconds >= MIN_WAIT_TIME)
            {
                return;
            }

            //Expired, remove this key firstly.
            //Don't remove the cache item to make sure we still can access the old one
            m_heap.Remove(sk);

            CacheItem ci = null;
            if (!m_cacheItems.TryGetValue(sk.Key, out ci))
            {
                //This should not happen. If it does, then this is a critial bug!
                LogManager.Warn("CacheManager:Assert Failure", string.Format("The key[{0}] exists in heap, but not in dictionary", sk.Key));
                return;
            }

            //No refresh action, just remove it directly
            if (ci.RefreshAction == null)
            {
                m_cacheItems.TryRemove(sk.Key, out ci);
                LogManager.Warn("CacheManager:ProcessTimerEvents", string.Format("The key[{0}] expired without a refresh action, just remove it"));
                return;
            }

            //Refresh the expired item now

            //Save the last expiration time
            ci.LastExpirationInfo.LastExpiration = ci.NextExpirationTime;
            //Update the next expiration time for next schedule
            ci.NextExpirationTime = ci.Expiration.NextExpiration(ci.LastExpirationInfo);
            
            //Validation
            ts = ci.NextExpirationTime - ci.LastExpirationInfo.LastExpiration;
            if (ts.TotalMilliseconds < MIN_WAIT_TIME)
            {
                LogManager.Warn("CacheManager:ProcessTimerEvents", "Too short time interval, ts=" + ts.TotalMilliseconds);

                //wait 3s
                ci.NextExpirationTime = ci.LastExpirationInfo.LastExpiration.AddMilliseconds(3000);
            }

            //Re-schedule again
            m_heap.Add(new SchedulerKey(ci.NextExpirationTime, sk.Key), DUMMY);

            //Fire the refresh action
            Task.Factory.StartNew(() => ci.Refresh(CacheItemRemovedReason.EXPIRED));
        }
    }
}
