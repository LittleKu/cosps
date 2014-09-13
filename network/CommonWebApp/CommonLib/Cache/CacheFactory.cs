using System;
using System.Collections.Concurrent;
using System.Collections.Generic;

namespace CommonLib.Cache
{
    /// <summary>
    /// 用于创建CacheManager的工厂类
    /// </summary>
    public static class CacheFactory
    {
        private static Dictionary<string, CacheManager> m_sCacheManagers = new Dictionary<string, CacheManager>();
        
        private static object m_lock = new object();

        /// <summary>
        /// 获取CacheManager实例
        /// 如果cacheManagerName的实例已经存在，则返回该实例
        /// 否则创建一个新的CacheManager实例
        /// </summary>
        /// <param name="cacheManagerName">CacheManager的名字</param>
        /// <param name="cacheManagerParams">可选参数列表，当前只支持一个整形参数，表示最大刷新线程数量，默认为3</param>
        /// <returns></returns>
        public static CacheManager GetCacheManager(string cacheManagerName, params object[] cacheManagerParams)
        {
            CacheManager manager = null;

            if (cacheManagerName == null)
            {
                cacheManagerName = "";
            }

            lock (m_lock)
            {
                if (!m_sCacheManagers.ContainsKey(cacheManagerName))
                {
                    manager = CreateCacheManager(cacheManagerParams);
                    m_sCacheManagers.Add(cacheManagerName, manager);
                }
                else
                {
                    manager = m_sCacheManagers[cacheManagerName];
                }
            }

            return manager;
        }

        /// <summary>
        /// 获取名字为空字符串""的默认CacheManager
        /// </summary>
        /// <returns></returns>
        public static CacheManager GetCacheManager()
        {
            return GetCacheManager("");
        }

        /// <summary>
        /// 解析可选参数列表
        /// </summary>
        /// <param name="cacheManagerParams"></param>
        /// <returns></returns>
        private static CacheManager CreateCacheManager(params object[] cacheManagerParams)
        {
            int threadCount = -1;
            if (cacheManagerParams != null && cacheManagerParams.Length > 0 && cacheManagerParams[0] != null)
            {
                if (!Int32.TryParse(cacheManagerParams[0].ToString(), out threadCount))
                {
                    threadCount = -1;
                }
            }

            CacheManager cm = null;
            if (threadCount > 0)
            {
                cm = new CacheManager(threadCount);
            }
            else
            {
                cm = new CacheManager();
            }

            return cm;
        }
    }
}
