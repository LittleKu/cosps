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
            CacheManager cm = new CacheManager(2, 5);

            Random rnd = new Random();

            List<Task> tasks = new List<Task>();
            for (int i = 0; i < 30; i++)
            {
                Worker w = new Worker(cm, "key" + i % 6, rnd.Next(200, 1000));
                Task task = Task.Factory.StartNew(() => w.Run());
                tasks.Add(task);
                Thread.Sleep(3);
            }

            Task.WaitAll(tasks.ToArray());
        }
    }

    class Worker
    {
        private CacheManager m_cm;
        private string m_key;
        private int m_round;
        private Random rnd = new Random();
        private int m_refreshCount = 0;

        public Worker(CacheManager cm, string key, int round)
        {
            m_cm = cm;
            m_key = key;
            m_round = round;
        }

        public void Run()
        {
            for (int i = 0; i < m_round; i++)
            {
                Test1();
                Test2();
            }
        }

        private void Test1()
        {
            Stopwatch sw = new Stopwatch();
            int sleepTime;

            sw.Restart();
            string value = m_cm.GetOrAdd(m_key, Refresh, rnd.Next(10000, 60000), 800) as string;
            sw.Stop();

            LogManager.Info("CacheTest", string.Format("key=[{0}], value=[{1}], cost [{2}]ms",
                m_key, ((value == null) ? "null" : value), sw.ElapsedMilliseconds));
            //Thread.Sleep(10);
            sleepTime = rnd.Next(1000, 500000);

            //Thread.Sleep(sleepTime);
        }

        public void Test2()
        {
            Stopwatch sw = new Stopwatch();
            int sleepTime;

            string value = m_cm.GetData(m_key) as string;

            if (value == null)
            {
                //Thread.Sleep(3);
                m_cm.Add(m_key, m_key + "-init", Refresh, rnd.Next(30000, 300000));
            }
            else
            {
                int val = rnd.Next(1, 10);
                if (val <= 5)
                {
                    m_cm.Remove(m_key);
                }
            }

            LogManager.Info("CacheTest", string.Format("key=[{0}], value=[{1}]", m_key, ((value == null) ? "null" : value)));
            //Thread.Sleep(10);
            sleepTime = rnd.Next(1000, 500000);

            //Thread.Sleep(sleepTime);
        }

        public object Refresh(string removedKey, object expiredValue)
        {
            Thread.Sleep(3);
            Thread.Sleep(rnd.Next(5000, 20000));

            m_refreshCount++;
            if (!removedKey.EndsWith("0"))
            {
                int val = m_refreshCount % 20;
                if (val < 5)
                {
                    if (expiredValue == null)
                    {
                        return removedKey;
                    }
                    else
                    {
                        return expiredValue.ToString() + "-" + removedKey;
                    }
                }
                else if (val < 15)
                {
                    throw new Exception("Can't get value for key=" + removedKey);
                }
                else
                {
                    if (expiredValue == null)
                    {
                        return "value1";
                    }
                    else
                    {
                        return expiredValue.ToString() + "-" + "value1";
                    }
                }
            }
            else
            {
                if (expiredValue == null)
                {
                    return removedKey;
                }
                else
                {
                    return expiredValue.ToString() + "-" + removedKey;
                }
            }
        }
    }
}
