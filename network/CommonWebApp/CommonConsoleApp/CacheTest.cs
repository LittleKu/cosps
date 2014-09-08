using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CommonLib.Cache;
using System.Threading;
using CommonLib;
using System.Threading.Tasks;
using System.Diagnostics;

namespace CommonConsoleApp
{
    class CacheTest
    {
        public static void Run()
        {
            CacheManager cm = new CacheManager();

            Random rnd = new Random();

            List<Task> tasks = new List<Task>();
            for (int i = 0; i < 30; i++)
            {
                Worker w = new Worker(cm, "key" + i, rnd.Next(20, 100));
                Task task = Task.Factory.StartNew(() => w.Run());
                tasks.Add(task);
                Thread.Sleep(3);
            }

            Task.WaitAll(tasks.ToArray());
        }
    }

    class Worker : ICacheItemRefreshAction
    {
        private CacheManager m_cm;
        private string m_key;
        private int m_round;
        private Random rnd = new Random();
        public Worker(CacheManager cm, string key, int round)
        {
            m_cm = cm;
            m_key = key;
            m_round = round;
        }

        public void Run()
        {
            Stopwatch sw = new Stopwatch();
            object obj;
            int sleepTime;
            for (int i = 0; i < m_round; i++)
            {
                string value = m_cm.GetData(m_key) as string;

                if (value == null)
                {
                    Thread.Sleep(3);
                    m_cm.Add(m_key, m_key + "-init", this, rnd.Next(30000, 300000));
                }
                else
                {
                    LogManager.Info("CacheTest", string.Format("key={0}, value={1}", m_key, value));
                    int val = rnd.Next(1, 10);
                    if (val <= 5)
                    {
                        LogManager.Info("CacheTest", string.Format("try to remove key={0}", m_key));
                        m_cm.Remove(m_key);
                    }
                }

                sw.Restart();
                for (int j = 0; j < 10000; j++)
                {
                    obj = m_cm.GetData(m_key);

                    obj = m_cm[m_key];
                }
                sw.Stop();
                LogManager.Info("CacheTest", string.Format("key={0}, cost={1}, {2}", m_key, sw.ElapsedMilliseconds, sw.ElapsedMilliseconds / 10));

                Thread.Sleep(3);
                sleepTime = rnd.Next(1000, 500000);

                Thread.Sleep(sleepTime);
            }
        }
        public object Refresh(string removedKey, object expiredValue, CacheItemRemovedReason removalReason)
        {
            if (removalReason != CacheItemRemovedReason.EXPIRED)
            {
                return null;
            }
            Thread.Sleep(3);
            Thread.Sleep(rnd.Next(5000, 20000));
            return expiredValue.ToString() + "-" + removedKey;
        }
    }

    class MyRefreshAction : ICacheItemRefreshAction
    {
        public object Refresh(string removedKey, object expiredValue, CacheItemRemovedReason removalReason)
        {
            if (removalReason != CacheItemRemovedReason.EXPIRED)
            {
                return null;
            }

            Random rnd = new Random();

            if (removedKey == "key2")
            {
                Thread.Sleep(rnd.Next(5000, 25000));

                return expiredValue + "-value2";
            }
            else if(removedKey == "key3")
            {
                Thread.Sleep(10000);


                return expiredValue + "-value3";
            }
            else if (removedKey == "key4")
            {
                if (rnd.Next(0, 1) == 0)
                {
                    throw new Exception("key4-" + expiredValue);
                }
                return expiredValue + "-value4";
            }

            return null;
        }
    }
}
