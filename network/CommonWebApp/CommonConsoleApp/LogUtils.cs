using System;
using System.Text;
using System.Threading;

namespace CommonConsoleApp
{
    public class LogUtils
    {
        public static Log GetLog(string category)
        {
            return new Log(category, LogLevel.INFO);
        }
    }

    public enum LogLevel
    {
        TRACE = 10,

        DEBUG = 20,

        INFO = 30,

        WARN = 40,

        ERROR = 50,

        FATAL = 60
    }

    public class Log
    {
        private const string DATE_TIME_FORMAT = "yyyy-MM-dd HH:mm:ss:fffffff";

        private string m_category = "";
        private LogLevel m_logLevel = LogLevel.INFO;

        public Log(string category, LogLevel level)
        {
            m_category = category;
            m_logLevel = level;
        }

        public Log() : this("", LogLevel.INFO)
        {
        }

        private void Output(string category, object message, Exception ex, LogLevel level)
        {
            if (level < m_logLevel)
            {
                return;
            }

            DateTime timestamp = DateTime.Now;
            int tid = Thread.CurrentThread.ManagedThreadId;

            string str;

            if (ex != null)
            {
                str = string.Format("[{0}][{1}][{2}][{3}] - {4}\r\n{5}", 
                    timestamp.ToString(DATE_TIME_FORMAT), tid, level, category, message.ToString(), ex.ToString());
            }
            else
            {
                str = string.Format("[{0}][{1}][{2}][{3}] - {4}",
                    timestamp.ToString(DATE_TIME_FORMAT), tid, level, category, message.ToString());
            }

            Console.WriteLine(str);
        }

        public bool IsDebugEnabled()
        {
            return (LogLevel.DEBUG >= m_logLevel);
        }

        public bool IsInfoEnabled()
        {
            return (LogLevel.INFO >= m_logLevel);
        }

        public bool IsWarnEnabled()
        {
            return (LogLevel.WARN >= m_logLevel);
        }

        public bool IsErrorEnabled()
        {
            return (LogLevel.ERROR >= m_logLevel);
        }

        public bool IsFatalEnabled()
        {
            return (LogLevel.FATAL >= m_logLevel);
        }

        public void Debug(object message)
        {
            Output(m_category, message, null, LogLevel.DEBUG);
        }

        public void Debug(object message, Exception ex)
        {
            Output(m_category, message, ex, LogLevel.DEBUG);
        }

        public void Info(object message)
        {
            Output(m_category, message, null, LogLevel.INFO);
        }

        public void Info(object message, Exception ex)
        {
            Output(m_category, message, ex, LogLevel.INFO);
        }

        public void Warn(object message)
        {
            Output(m_category, message, null, LogLevel.WARN);
        }

        public void Warn(object message, Exception ex)
        {
            Output(m_category, message, ex, LogLevel.WARN);
        }

        public void Error(object message)
        {
            Output(m_category, message, null, LogLevel.ERROR);
        }

        public void Error(object message, Exception ex)
        {
            Output(m_category, message, ex, LogLevel.ERROR);
        }

        public void Fatal(object message)
        {
            Output(m_category, message, null, LogLevel.FATAL);
        }

        public void Fatal(object message, Exception ex)
        {
            Output(m_category, message, ex, LogLevel.FATAL);
        }
    }
}
