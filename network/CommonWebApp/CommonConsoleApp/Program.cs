using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Threading;
using System.Net;
using CommonLib;

namespace CommonConsoleApp
{
    class MyListener : ExecuteListener
    {
        private AutoResetEvent m_done = null;
        public MyListener(AutoResetEvent done)
        {
            m_done = done;
        }

        public void ExecuteCompleted(RequestStateInfo stateInfo, Exception ex)
        {
            bool flag = false;

            do
            {
                //error happened
                if (stateInfo == null || ex != null)
                {
                    break;
                }

                WebResponse rsp = stateInfo.response;

                // Display each header and it's key , associated with the response object.
                for (int i = 0; i < rsp.Headers.Count; ++i)
                    Console.WriteLine("{0}={1}", rsp.Headers.Keys[i], rsp.Headers[i]);

                ByteBuffer content = stateInfo.responseBuffer;
                if (content != null && content.GetCount() > 0)
                {
                    //Save the content to the file
                    using (FileStream fs = File.Create("result.bin"))
                    {
                        fs.Write(content.GetRawBuffer(), 0, content.GetCount());
                    }

                    ByteArrays bas = new ByteArrays();

                    try
                    {
                        bas.Read(content.GetRawBuffer());
                    }
                    catch (Exception ex2)
                    {
                        Console.WriteLine(ex2.ToString());
                    }

                    Console.WriteLine("Found {0} RangeArrays", bas.ByteArrayList.Count);

                    string entry;
                    for(int i = 0; i < bas.ByteArrayList.Count; i++)
                    {
                        RangeArray ra = bas.ByteArrayList[i];

                        int len = ra.Count;
                        entry = Encoding.UTF8.GetString(ra.Buffer, ra.Offset, ra.Count);

                        Console.WriteLine("[{0,3}]: len={1}, content=[{2}]", i + 1, len, entry);
                    }
                }

                flag = true;
            }
            while (false);

            if (flag)
            {
                Console.WriteLine("\r\nOK");
            }
            else
            {
                Console.WriteLine("\r\nNOK");
            }
            m_done.Set();
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            //WebRequestGetExample.Run();
            //WebRequestGetExample.Run2();
            //WebRequest_BeginGetResponse.Run();

            TestWebAgent();
        }

        static void TestWebAgent()
        {
            AutoResetEvent done = new AutoResetEvent(false);

            WebAgentAsync agent = new WebAgentAsync();
            string url = null;
            url = "http://mirror.bit.edu.cn/apache//logging/log4net/source/log4net-1.2.13-src.zip";
            //url = "http://mirrors.ustc.edu.cn/eclipse/technology/epp/downloads/release/juno/SR2/eclipse-jee-juno-SR2-win32.zip";
            //url = "http://cn.bing.com/few";
            //url = "xyz://f.com";
            url = "http://www.baidu.com/";
            //url = "http://localhost:4129/";
            url = "http://localhost:4129/MainHandler.ashx?op=test&param=abc";
            //url = "http://download.httpwatch.com/httpwatch.exe";
            agent.Execute(url, new MyListener(done));

            done.WaitOne();
        }
    }
}
