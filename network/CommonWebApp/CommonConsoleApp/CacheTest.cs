using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CommonLib.Cache;
using System.Threading;
using CommonLib;

namespace CommonConsoleApp
{
    class CacheTest
    {
        public static void Run()
        {
            CacheManager cm = new CacheManager();

            Random rnd = new Random();
            cm.Add("key1", "value1");
            cm.Add("key2", "value2", new MyRefreshAction(), 20000);

            cm.Add("key3", "value3", new MyRefreshAction(), 20000);

            string[] keys = {"key1", "key2", "key3", "key4"};
            for (int i = 0; i < 20; i++)
            {
                for (int j = 0; j < keys.Length; j++)
                {
                    object value = cm.GetData(keys[j]);

                    LogManager.Info(keys[j], value == null ? "null" : value.ToString());

                    Thread.Sleep(rnd.Next(30, 5000));
                }

                cm.Add("key4", "value0", new MyRefreshAction(), 5000);
            }
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

            if (removedKey == "key2")
            {
                Thread.Sleep(5000);

                return expiredValue + "-value2";
            }
            else if(removedKey == "key3")
            {
                Thread.Sleep(10000);


                return expiredValue + "-value3";
            }
            else if (removedKey == "key4")
            {
                return expiredValue + "-value4";
            }
            return null;
        }
    }
}
