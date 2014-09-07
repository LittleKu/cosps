using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CommonLib
{
    public static class LogManager
    {
        private static log4net.ILog m_log4net = null;

        static LogManager()
        {
            m_log4net = log4net.LogManager.GetLogger("default");
        }

        public static void Error(string title, Exception message, string remark)
        {
            if (m_log4net == null || !m_log4net.IsErrorEnabled)
            {
                return;
            }
            m_log4net.Error(Format(title, null, remark), message);
        }

        public static void Error(string title, Exception message)
        {
            if (m_log4net == null || !m_log4net.IsErrorEnabled)
            {
                return;
            }
            m_log4net.Error(Format(title, null, null), message);
        }

        public static void Warn(string title, string message, string remark)
        {
            if (m_log4net == null || !m_log4net.IsWarnEnabled)
            {
                return;
            }
            m_log4net.Warn(Format(title, message, remark));
        }

        public static void Warn(string title, string message)
        {
            if (m_log4net == null || !m_log4net.IsWarnEnabled)
            {
                return;
            }
            m_log4net.Warn(Format(title, message, null));
        }

        public static void Info(string title, string message, string remark)
        {
            if (m_log4net == null || !m_log4net.IsInfoEnabled)
            {
                return;
            }
            m_log4net.Info(Format(title, message, remark));
        }

        public static void Info(string title, string message)
        {
            if (m_log4net == null || !m_log4net.IsInfoEnabled)
            {
                return;
            }
            m_log4net.Info(Format(title, message, null));
        }

        public static void Debug(string title, string message, string remark)
        {
            if (m_log4net == null || !m_log4net.IsDebugEnabled)
            {
                return;
            }
            m_log4net.Debug(Format(title, message, remark));
        }

        public static void Debug(string title, string message)
        {
            if (m_log4net == null || !m_log4net.IsDebugEnabled)
            {
                return;
            }
            m_log4net.Debug(Format(title, message, null));
        }

        private static string Format(string title, string message, string remark)
        {
            StringBuilder sb = new StringBuilder();
            if (!string.IsNullOrEmpty(title))
            {
                sb.Append("title=").Append(title);
            }
            if (!string.IsNullOrEmpty(message))
            {
                if (sb.Length > 0)
                {
                    sb.Append(",");
                }
                sb.Append("message=").Append(message);
            }
            if (!string.IsNullOrEmpty(remark))
            {
                if (sb.Length > 0)
                {
                    sb.Append(",");
                }
                sb.Append("remark=").Append(remark);
            }

            return sb.ToString();
        }
    }
}
