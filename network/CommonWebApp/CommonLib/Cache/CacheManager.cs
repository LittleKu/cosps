using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections.Concurrent;
using System.Threading;
using System.Threading.Tasks;
using CommonLib;
using System.Diagnostics;

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
        private ConcurrentQueue<OperationEvent> m_operEvents = null;

        //A min heap for task scheduler
        private SortedDictionary<SchedulerKey, object> m_heap = null;

        //Auto reset event for processing user actions events and timer events
        private AutoResetEvent m_waitHandle = null;

        //The semaphore for counting the concurrent threads to refresh the invalid items
        private Semaphore m_refreshSyncSem = null;

        //The maximum count of m_refreshSyncSem
        private int m_maxRefreshSyncSemCount = 0;

        //The current available thread count that could be used to Refresh by CacheManager
        private int m_availRefreshThreadCount = 0;

        //Is this scheduler thread still running?
        private bool m_running = false;

        public CacheManager(int maxRefreshThreadCount = 3, int maxWaitRefreshSemCount = Int32.MaxValue)
        {
            m_cacheItems = new ConcurrentDictionary<string, CacheItem>();
            m_operEvents = new ConcurrentQueue<OperationEvent>();
            m_heap = new SortedDictionary<SchedulerKey, object>();
            m_waitHandle = new AutoResetEvent(false);

            m_maxRefreshSyncSemCount = maxWaitRefreshSemCount;
            if (m_maxRefreshSyncSemCount <= 0)
            {
                m_maxRefreshSyncSemCount = Int32.MaxValue;
            }
            m_refreshSyncSem = new Semaphore(m_maxRefreshSyncSemCount, m_maxRefreshSyncSemCount);

            m_availRefreshThreadCount = maxRefreshThreadCount;
            if (m_availRefreshThreadCount <= 0)
            {
                m_availRefreshThreadCount = 1;
            }

            //start to run scheduler
            m_running = true;
            Task.Factory.StartNew(() => Run());
        }

        public int Count 
        {
            get
            {
                return m_cacheItems.Count;
            }
        }

        public void Add(string key, object value)
        {
            CacheItem ci = new CacheItem(key, value);
            SendEvent(OperationType.OP_ADD, ci);
        }

        public void Add(string key, object value, CacheItemRefreshAction refreshAction, long expiration)
        {
            CacheItem ci = new CacheItem(key, value, refreshAction, expiration);
            SendEvent(OperationType.OP_ADD, ci);
        }

        public void Add(string key, object value, CacheItemRefreshAction refreshAction, ICacheItemExpiration expiration)
        {
            CacheItem ci = new CacheItem(key, value, refreshAction, expiration);
            SendEvent(OperationType.OP_ADD, ci);
        }

        public object GetOrAdd(string key, CacheItemRefreshAction refreshAction, long expiration, int lockTimeout)
        {
            return GetOrAdd(key, refreshAction, new DefaultCacheItemExpiration(expiration), lockTimeout);
        }

        public object GetOrAdd(string key, CacheItemRefreshAction refreshAction, ICacheItemExpiration expiration, int lockTimeout)
        {
            //First try to get the value for the key, if it doesn't exist, then try to add the candidate value.
            //It's thread safe. The result "ci" for multiple threads should be the exactly the same value.
            CacheItem candidate = new CacheItem(key, null, refreshAction, expiration);
            CacheItem ci = SendEvent(OperationType.OP_ADD_OR_GET, candidate) as CacheItem;
            if (ci == null)
            {
                //This should not happen. If it does, then this is a critial bug!
                LogManager.Warn("CacheManager:Assert Failure", string.Format("The result of GetOrAdd for key [{0}] is null", key));
                return null;
            }

            //Try to get the value. If there's no valid value, try to refresh it now.
            object result = null;
            if ((result = ci.Value) == null)
            {
                Stopwatch sw = new Stopwatch();
                sw.Start();
                if (!m_refreshSyncSem.WaitOne(lockTimeout))
                {
                    LogManager.Warn("CacheManager", string.Format("Timeout when try to get a refresh token for [{0}]", ci.Key));
                }
                else
                {
                    sw.Stop();
                    int remainTime = lockTimeout - (int)sw.ElapsedMilliseconds;
                    if (remainTime < 0 || remainTime > lockTimeout)
                    {
                        LogManager.Warn("CacheManager", string.Format("Invalid remaining time for [{0}], remainTime=[{1}]", ci.Key, remainTime));
                        remainTime = 0;
                    }

                    //Now we are allowed to refresh this key
                    ci.RefreshSync(remainTime);

                    //Release the token
                    int prevCount = m_refreshSyncSem.Release();
                    LogManager.Info("CacheManager", string.Format("Refresh done for [{0}], the previous semaphore count is [{1}]", ci.Key, prevCount));
                }

                //get the updated value
                result = ci.Value;
            }

            return result;
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
                SendEvent(OperationType.OP_REMOVE, ci);
            }
        }

        public void Flush()
        {
            SendEvent(OperationType.OP_CLEAR, null);
        }

        private object SendEvent(OperationType operType, object operObj)
        {
            OperationEvent operEvent = new OperationEvent(operType, operObj);
            m_operEvents.Enqueue(operEvent);
            m_waitHandle.Set();

            //Make the public API call synchornized
            return operEvent.WaitUntilProcessed();
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
                    if (!ProcessTimerEvents())
                    {
                        continue;
                    }

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
            OperationEvent operEvent = null;
            while (!m_operEvents.IsEmpty)
            {
                //This should not happen. If it does, then this is a critial bug!
                if (!m_operEvents.TryDequeue(out operEvent))
                {
                    LogManager.Warn("CacheManager:Assert Failure", "Failed to dequeue a message");
                    continue;
                }

                if (!ProcessOperationEvent(operEvent))
                {
                    return false;
                }
            }
            return true;
        }

        private bool ProcessOperationEvent(OperationEvent operEvent)
        {
            bool result = true;
            bool acquiredLock = false;
            try
            {
                Monitor.Enter(operEvent.ProcessedLock, ref acquiredLock);
                if (acquiredLock)
                {
                    result = DoProcessOperationEvent(operEvent);
                }
            }
            finally
            {
                if (acquiredLock)
                {
                    operEvent.Processed = true;
                    Monitor.PulseAll(operEvent.ProcessedLock);
                    Monitor.Exit(operEvent.ProcessedLock);
                }
            }

            return result;
        }

        private bool DoProcessOperationEvent(OperationEvent cme)
        {
            bool shouldContinue = true;
            switch (cme.OperType)
            {
                case OperationType.OP_ADD:
                    {
                        shouldContinue = DoAdd(cme.OperObject as CacheItem);
                    }
                    break;

                case OperationType.OP_ADD_OR_GET:
                    {
                        shouldContinue = DoGetOrAdd(cme);
                    }
                    break;

                case OperationType.OP_REMOVE:
                    {
                        shouldContinue = DoRemove(cme.OperObject as CacheItem);
                    }
                    break;

                case OperationType.OP_CLEAR:
                    {
                        Clear();
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

        private bool DoGetOrAdd(OperationEvent operEvent)
        {
            CacheItem ci = operEvent.OperObject as CacheItem;
            if (ci == null)
            {
                LogManager.Warn("CacheManager:Assert Failure", "DoGetOrAdd a null item");
                return true;
            }

            //Cache Item
            bool added = m_cacheItems.TryAdd(ci.Key, ci);
            if (added)
            {
                //Add the SchedulerKey at the same time
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

                operEvent.OperResult = ci;

                LogManager.Info("CacheManager:DoGetOrAdd", string.Format("The key [{0}] is added, value=[{1}]", ci.Key, ci.ToString()));
            }
            else
            {
                CacheItem existingItem = null;
                if (!m_cacheItems.TryGetValue(ci.Key, out existingItem))
                {
                    //This should not happen. If it does, then this is a critial bug!
                    LogManager.Warn("CacheManager:Assert Failure", string.Format("The expected existing key[{0}] doesn't exist", ci.Key));
                }
                else if (existingItem == null)
                {
                    //This should not happen. If it does, then this is a critial bug!
                    LogManager.Warn("CacheManager:Assert Failure", string.Format("The expected existing key[{0}] is null", ci.Key));
                }
                else
                {
                    operEvent.OperResult = existingItem;
                    LogManager.Info("CacheManager:DoGetOrAdd", string.Format("The key [{0}] is already existing, value=[{1}]", 
                        ci.Key, existingItem.ToString()));
                }
            }

            return true;
        }

        private bool DoAdd(CacheItem ci)
        {
            if (ci == null)
            {
                LogManager.Warn("CacheManager:Assert Failure", "DoAdd a null item");
                return true;
            }

            //Cache Item
            bool added = m_cacheItems.TryAdd(ci.Key, ci);
            if (added)
            {
                //Add the SchedulerKey at the same time
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
            }

            if (added)
            {
                LogManager.Info("CacheManager:DoAdd", string.Format("The key [{0}] is added, value=[{1}]", ci.Key, ci.ToString()));
            }
            else
            {
                LogManager.Info("CacheManager:DoAdd", string.Format("The key [{0}] is already existing", ci.Key));
            }

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

        private void Clear()
        {
            m_cacheItems.Clear();
            m_heap.Clear();

            LogManager.Info("CacheManager:Clear", "Clear all the cached items");
        }

        //This function is triggered by timer. If a key is expired, this function will 
        //try to refresh the value if there's a refresh action associated with it
        private bool ProcessTimerEvents()
        {
            if (m_heap.Count <= 0)
            {
                return true;
            }

            //The first key is the minimum value to be the first expiration value
            SchedulerKey sk = m_heap.First().Key;

            //Check if this is really an expiration event?
            TimeSpan ts = sk.NextExpirationTime - DateTime.Now;
            //not yet, wait
            if (ts.TotalMilliseconds >= MIN_WAIT_TIME)
            {
                return true;
            }

            //Expired, don't remove the cache item to make sure we can still access the old one

            CacheItem ci = null;
            if (!m_cacheItems.TryGetValue(sk.Key, out ci))
            {
                m_heap.Remove(sk);

                //This should not happen. If it does, then this is a critial bug!
                LogManager.Warn("CacheManager:Assert Failure", string.Format("The key[{0}] exists in heap, but not in dictionary", sk.Key));
                return true;
            }

            //No refresh action, just remove it directly
            if (ci.RefreshAction == null)
            {
                m_cacheItems.TryRemove(sk.Key, out ci);
                m_heap.Remove(sk);

                LogManager.Info("CacheManager:ProcessTimerEvents", string.Format("The key[{0}] expired without a refresh action, just remove it", sk.Key));
                return true;
            }

            return RefreshExpiredCache(sk, ci);
        }

        //The item is expired, call the "Refresh" action of this item to get a refresh value
        //1. If there's not enough worker threads, the dispatching thread will wait until the below 2 conditions happend:
        //(1). There is worker threads available
        //(2). There is user event comming (Add/Remove etc.)
        //2. If there's at least one worker thread, the dispatching thread will schedule the "Refresh" action in one worker thread
        //after the worker thread end to refresh the cache item, it will release the worker thread resource, and notify the
        //dispatching thread, so that other expired cache items wait for refresh worker thread can do their refresh job
        private bool RefreshExpiredCache(SchedulerKey sk, CacheItem ci)
        {
            //Refresh the expired item now, but firstly we should get a token
            if (Interlocked.Decrement(ref m_availRefreshThreadCount) < 0)
            {
                //Got an invalid token, return it
                Interlocked.Increment(ref m_availRefreshThreadCount);

                //No available refresh thread now, wait for one
                LogManager.Info("CacheManager:ProcessTimerEvents", string.Format("Begin to wait for refreshing thread token with key [{0}]", sk.Key));

                m_waitHandle.WaitOne();

                //Either there's an user event comming or a refresh thread token is available
                LogManager.Info("CacheManager:ProcessTimerEvents", string.Format("End to wait for refreshing thread token with key [{0}]", sk.Key));

                //No need to wait again, start a new loop
                return false;
            }

            //Save the last expiration time
            ci.LastExpirationInfo.LastExpiration = ci.NextExpirationTime;
            //Update the next expiration time for next schedule
            ci.NextExpirationTime = ci.Expiration.NextExpiration(ci.LastExpirationInfo);

            //Validation
            TimeSpan ts = ci.NextExpirationTime - ci.LastExpirationInfo.LastExpiration;
            if (ts.TotalMilliseconds < MIN_WAIT_TIME)
            {
                LogManager.Warn("CacheManager:ProcessTimerEvents", "Too short time interval, ts=" + ts.TotalMilliseconds);

                //wait 3s
                ci.NextExpirationTime = ci.LastExpirationInfo.LastExpiration.AddMilliseconds(3000);
            }

            //Re-schedule again
            m_heap.Remove(sk);
            m_heap.Add(new SchedulerKey(ci.NextExpirationTime, sk.Key), DUMMY);

            //Fire the refresh action
            Task.Factory.StartNew(() =>
            {
                try
                {
                    ci.RefreshAsync();

                    //Release the token
                    Interlocked.Increment(ref m_availRefreshThreadCount);

                    //And notify the listeners
                    m_waitHandle.Set();
                }
                catch (Exception ex)
                {
                    LogManager.Error(string.Format("Task for key [{0}] exception", ci.Key), ex);
                }
            });

            return true;
        }
    }
}
