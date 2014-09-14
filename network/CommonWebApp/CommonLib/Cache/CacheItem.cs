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
        private readonly object m_refreshLock = new object();

        private string m_key;

        private object m_value;

        private CacheItemRefreshAction m_refreshAction;

        private ICacheItemExpiration m_expiration;

        //ProcessedLock for Refresh Action
        //This lock should be locked when do refresh action
        public object RefreshLock
        {
            get { return m_refreshLock; }
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

        public CacheItem(string key, object value, CacheItemRefreshAction refreshAction = null, ICacheItemExpiration expiration = null)
        {
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

        private void DoRefresh()
        {
            if (m_refreshAction == null)
            {
                LogManager.Warn("CacheItem", string.Format("The key [{0}] has not Refresh action", this.Key));
                return;
            }

            LogManager.Info("CacheItem", string.Format("The key [{0}] begin to refresh", this.Key));

            Stopwatch sw = new Stopwatch();
            sw.Start();

            this.LastExpirationInfo.LastBeginRefresh = DateTime.Now;

            object newValue = null;
            try
            {
                newValue = m_refreshAction(m_key, m_value);
            }
            catch (Exception ex)
            {
                newValue = null;
                LogManager.Error(string.Format("CacheItem [{0}] refresh failed", Key), ex);
            }
            m_value = newValue;

            this.LastExpirationInfo.LastEndRefresh = DateTime.Now;

            sw.Stop();
            LogManager.Info("CacheItem", string.Format("The key [{0}] end to refresh, cost {1}ms", this.Key, sw.ElapsedMilliseconds));
        }

        //This function is only executed by CacheManager worker thread in asynchoronisely
        public void RefreshAsync()
        {
            bool acquiredLock = false;
            try
            {
                Monitor.TryEnter(RefreshLock, ref acquiredLock);
                if (!acquiredLock)
                {
                    LogManager.Warn("CacheItem", string.Format("The key [{0}] is already in refreshing", this.Key), "Refresh");
                }
                else
                {
                    //alwasy refresh even the current value is valid
                    DoRefresh();
                }
            }
            finally
            {
                Monitor.Exit(RefreshLock);
            }
        }

        //This function is only executed by external thread in synchoronisely
        public void RefreshSync(int waitTimeout)
        {
            bool acquiredLock = false;
            try
            {
                Monitor.TryEnter(RefreshLock, waitTimeout, ref acquiredLock);
                if (!acquiredLock)
                {
                    LogManager.Warn("CacheItem", string.Format("Timeout when try to refresh [{0}], timeout=[{1}]", this.Key, waitTimeout));
                }
                else
                {
                    //only refresh when there's no valid values
                    if (m_value == null)
                    {
                        DoRefresh();
                    }
                }
            }
            finally
            {
                if (acquiredLock)
                {
                    Monitor.Exit(RefreshLock);
                }
            }
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
        private readonly object m_processedLock = new object();

        public OperationType OperType { get; set; }

        public bool Processed { get; set; }

        public object OperObject { get; set; }

        public object ProcessedLock
        {
            get { return m_processedLock; }
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
                Monitor.Enter(ProcessedLock);
                while (!this.Processed)
                {
                    Monitor.Wait(ProcessedLock);
                }
            }
            finally
            {
                Monitor.Exit(ProcessedLock);
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
