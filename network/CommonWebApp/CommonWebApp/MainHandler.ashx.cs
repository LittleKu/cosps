using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Text;
using System.Threading;
using System.Collections.Specialized;
using CommonLib;

namespace CommonWebApp
{
    /// <summary>
    /// Summary description for MainHandler
    /// </summary>
    public class MainHandler : IHttpHandler
    {
        public void ProcessRequest(HttpContext context)
        {
            context.Response.ContentType = "application/octet-stream";

            ByteArrays bas = new ByteArrays();

            byte[] buffer = null;

            string reqParams = GetRequestParams(context.Request);
            buffer = Encoding.UTF8.GetBytes(reqParams);
            bas.AddByteArray(buffer);

            string reqQS = GetQueryString(context.Request);
            buffer = Encoding.UTF8.GetBytes(reqQS);
            bas.AddByteArray(buffer);

            string str = "Hello World";
            buffer = Encoding.UTF8.GetBytes(str);
            int repeatCount = 100;
            for (int i = 0; i < repeatCount; i++)
            {
                bas.AddByteArray(buffer);
            }
            bas.Write(context.Response.OutputStream);
        }

        private string GetRequestParams(HttpRequest request)
        {
            return ToString(request.Params);
        }

        private string GetQueryString(HttpRequest request)
        {
            return ToString(request.QueryString);
        }

        private static string ToString(NameValueCollection nvc)
        {
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < nvc.Count; i++)
            {
                if (i > 0)
                {
                    sb.Append(";");
                }
                sb.Append(nvc.GetKey(i));
                sb.Append("=");

                string[] currValues = nvc.GetValues(i);
                for (int j = 0; j < currValues.Length; j++)
                {
                    if (j > 0)
                    {
                        sb.Append(",");
                    }
                    sb.Append(currValues[j]);
                }
            }
            return sb.ToString();
        }

        public bool IsReusable
        {
            get
            {
                return false;
            }
        }
    }
}