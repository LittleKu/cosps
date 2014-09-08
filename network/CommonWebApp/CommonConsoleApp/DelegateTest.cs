using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections.Concurrent;

namespace CommonConsoleApp
{
    class DelegateTest
    {
        private ConcurrentDictionary<string, int> m_dic = null;
        public DelegateTest()
        {
            m_dic = new ConcurrentDictionary<string, int>();
        }

        private int AddValueFactory(string key)
        {
            Console.WriteLine("AddValueFactory: " + key);
            return key.Length * 2;
        }

        private int UpdateValueFactory(string key, int value)
        {
            Console.WriteLine("UpdateValueFactory: " + key + ", value=" + value);
            return key.Length * 2 + value;
        }

        public void Test1()
        {
            m_dic.AddOrUpdate("k1", new Func<string, int>(AddValueFactory), new Func<string, int, int>(UpdateValueFactory));
            m_dic.AddOrUpdate("k1", new Func<string, int>(AddValueFactory), new Func<string, int, int>(UpdateValueFactory));
            m_dic.AddOrUpdate("k1", new Func<string, int>(AddValueFactory), new Func<string, int, int>(UpdateValueFactory));
            m_dic.AddOrUpdate("k1", new Func<string, int>(AddValueFactory), new Func<string, int, int>(UpdateValueFactory));
            m_dic.AddOrUpdate("k1", new Func<string, int>(AddValueFactory), new Func<string, int, int>(UpdateValueFactory));
        }

        public static void Run()
        {
            DelegateTest dt = new DelegateTest();
            dt.Test1();
        }
    }
}
