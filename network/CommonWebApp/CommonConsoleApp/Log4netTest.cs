using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CommonLib;


namespace CommonConsoleApp
{
    class Log4netTest
    {
        public static void Run(string[] args)
        {
            Exception ex = new Exception("Exception test");

            LogManager.Debug("t debug 1", "run 1");
            LogManager.Debug("t debug 2", "run 2", "remark 2");

            LogManager.Info("t Info 1", "run 3");
            LogManager.Info("t Info 2", "run 4", "remark 4");

            LogManager.Warn("t warn 1", "run 5");
            LogManager.Warn("t warn 2", "run 6", "remark 6");

            LogManager.Error("t error 1", ex, "run 7");
            LogManager.Error("t error 2", ex);

            foo("hell");
        }

        private static void foo(string msg)
        {
            try
            {
                bar(msg);
            }
            catch (Exception ex)
            {
                LogManager.Error("foo", ex);
            }
        }

        private static string bar(string msg)
        {
            throw new Exception(msg);
        }
    }
}
