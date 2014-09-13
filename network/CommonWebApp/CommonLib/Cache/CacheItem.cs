using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Diagnostics;
using CommonLib;

namespace CommonLib.Cache
{
    class CacheItem
    {
        private const int STOPPED = 0;
        private const int RUNNING = 1;

        private int m_running;
        private readonly object m_lock = new object();

        private string m_key;

        private object m_value;

        private CacheItemRefreshAction m_refreshAction;

        private ICacheItemExpiration m_expiration;

        //Lock for Refresh Action
        public object Lock
        {
            get { return m_lock; }
        }

        public string Key
        {
            get { return m_key; }
        }

        public CacheItemRefreshAction RefreshAction
        {
            get { return m_refreshAction; }
        }

        public ICacheItemExpiration Expiration
        {
            get { return m_expiration; }
        }

        public object Value 
        {
            get { return m_value; }
            set { m_value = value; }
        }

        public LastExpirationInfo LastExpirationInfo { get; set; }

        public DateTime NextExpirationTime { get; set; }

        public CacheItem(string key, object value, CacheItemRefreshAction refreshAction, ICacheItemExpiration expiration)
        {
            this.m_running = STOPPED;
            this.m_key = key;
            this.m_value = value;
            this.m_refreshAction = refreshAction;
            this.m_expiration = expiration;

            this.LastExpirationInfo = new LastExpirationInfo();

            if (this.m_expiration == null)
            {
                //never expire
                this.NextExpirationTime = this.LastExpirationInfo.LastExpiration.AddYears(10);
            }
            else
            {
                this.NextExpirationTime = m_expiration.NextExpiration(LastExpirationInfo);
            }
        }

        public CacheItem(string key, object value, CacheItemRefreshAction refreshAction, long expiration)
            : this(key, value, refreshAction, new DefaultCacheItemExpiration(expiration))
        {
        }

        public CacheItem(string key, object value) : this(key, value, null, null)
        {
        }

        private void DoRefresh(CacheItemRemovedReason reason)
        {
            if (m_refreshAction == null)
            {
                return;
            }

            LogManager.Info("CacheItem", string.Format("The key [{0}] begin to refresh", this.Key));

            Stopwatch sw = new Stopwatch();
            sw.Start();

            this.LastExpirationInfo.LastBeginRefresh = DateTime.Now;

            object newValue = null;
            try
            {
                newValue = m_refreshAction(m_key, m_value, reason);
            }
            catch (Exception ex)
            {
                newValue = null;
                LogManager.Error(string.Format("CacheItem Refresh key[{0}] failed", Key), ex);
            }
            m_value = newValue;

            this.LastExpirationInfo.LastEndRefresh = DateTime.Now;

            sw.Stop();
            LogManager.Info("CacheItem", string.Format("The key [{0}] end to refresh, cost {1}ms", this.Key, sw.ElapsedMilliseconds));
        }

        public void RefreshNoWait(CacheItemRemovedReason reason)
        {
            bool acquiredLock = false;
            try
            {
                Monitor.TryEnter(Lock, ref acquiredLock);
                if (!acquiredLock)
                {
                    LogManager.Warn("CacheItem", string.Format("The key [{0}] is already in refreshing", this.Key), "Refresh");
                }
                else
                {
                    DoRefresh(reason);
                }
            }
            finally
            {
                Monitor.Exit(Lock);
            }
        }

        public object RefreshWait(CacheItemRemovedReason reason, int waitTimeout)
        {
            object result = null;

            bool acquiredLock = false;
            try
            {
                Monitor.TryEnter(Lock, waitTimeout, ref acquiredLock);
                if (!acquiredLock)
                {
                    LogManager.Warn("CacheItem", string.Format("Timeout when try to refresh [{0}]", this.Key));
                }
                else
                {
                    if (m_value == null)
                    {
                        DoRefresh(reason);
                    }
                }

                result = m_value;
            }
            finally
            {
                if (acquiredLock)
                {
                    Monitor.Exit(Lock);
                }
            }

            return result;
        }

        public void Refresh(CacheItemRemovedReason reason)
        {
            if(Interlocked.CompareExchange(ref m_running, RUNNING, STOPPED) != STOPPED)
            {
                LogManager.Warn("CacheItem", string.Format("The key [{0}] is already in refreshing", this.Key), "Refresh");
                return;
            }

            LogManager.Info("CacheItem", string.Format("The key [{0}] begin to refresh", this.Key));

            Stopwatch sw = new Stopwatch();
            sw.Start();

            if (m_refreshAction != null)
            {
                this.LastExpirationInfo.LastBeginRefresh = DateTime.Now;

                object newValue = null;
                try
                {
                    newValue = m_refreshAction(m_key, m_value, reason);
                }
                catch (Exception ex)
                {
                    newValue = null;
                    LogManager.Error(string.Format("CacheItem Refresh key[{0}] failed", Key), ex);
                }
                m_value = newValue;

                this.LastExpirationInfo.LastEndRefresh = DateTime.Now;
            }

            sw.Stop();
            LogManager.Info("CacheItem", string.Format("The key [{0}] end to refresh, cost {1}ms", this.Key, sw.ElapsedMilliseconds));
            Interlocked.Exchange(ref m_running, STOPPED);
        }

        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append("CacheItem[key=").Append(m_key);
            sb.Append(", expiration=").Append(this.NextExpirationTime).Append("]");
            return sb.ToString();
        }
    }

    enum OperationType
    {
        OP_ADD,
        OP_REMOVE,
        OP_CLEAR,
        OP_STOP
    }

    class OperationEvent
    {
        private readonly object m_lock = new object();

        public OperationType OperType { get; set; }

        public bool Processed { get; set; }

        public object OperObject { get; set; }

        public object Lock
        {
            get { return m_lock; }
        }

        public OperationEvent(OperationType oper, object obj)
        {
            this.OperType = oper;
            this.OperObject = obj;
            this.Processed = false;
        }

        public void WaitUntilProcessed()
        {
            try
            {
                Monitor.Enter(Lock);
                while (!this.Processed)
                {
                    Monitor.Wait(Lock);
                }
            }
            finally
            {
                Monitor.Exit(Lock);
            }
        }
    }

    class SchedulerKey : IComparable<SchedulerKey>
    {
        public DateTime NextExpirationTime { get; set; }

        public string Key { get; set; }

        public SchedulerKey(DateTime time, string key)
        {
            NextExpirationTime = time;
            Key = key;
        }

        public int CompareTo(SchedulerKey other)
        {
            if (other == null)
            {
                return -1;
            }

            //sorted by time
            var result = NextExpirationTime.CompareTo(other.NextExpirationTime);
            if (result != 0)
            {
                return result;
            }

            //then by key to avoid same key
            result = Key.CompareTo(other.Key);
            return result;
        }

        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append("SchedulerKey[expiration=").Append(NextExpirationTime);
            sb.Append(", key=").Append(Key).Append("]");
            return sb.ToString();
        }
    }

    public class DefaultCacheItemExpiration : ICacheItemExpiration
    {
        private long m_expiration = 0;

        public DefaultCacheItemExpiration(long expiration)
        {
            m_expiration = expiration;
        }

        public DateTime NextExpiration(LastExpirationInfo lastInfo)
        {
            return lastInfo.LastExpiration.AddMilliseconds(m_expiration);
        }
    }
}
