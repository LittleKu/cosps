using System;
using System.Net;
using System.IO;
using System.Text;
using System.Threading;

namespace CommonConsoleApp
{
    public interface ExecuteListener
    {
        void ExecuteCompleted(RequestStateInfo stateInfo, Exception e);
    }

    public class RequestStateInfo
    {
        // This class stores the state of the request.
        public const int BUFFER_SIZE = 4096;
        public byte[] recvBuffer;

        public string url;
        public WebRequest request;

        public ExecuteListener listener;

        public ByteBuffer responseBuffer;
        public string errorMessage;

        public WebResponse response;
        public Stream responseStream;

        public RequestStateInfo()
        {
            recvBuffer = new byte[BUFFER_SIZE];

            url = null;
            request = null;

            listener = null;

            responseBuffer = new ByteBuffer();
            errorMessage = null;

            response = null;
            responseStream = null;
        }
    }

    public class WebAgentAsync
    {
        private static readonly Log log = LogUtils.GetLog(System.Reflection.MethodBase.GetCurrentMethod().ReflectedType.FullName);

        public WebAgentAsync()
        {
        }

        public IAsyncResult Execute(string url, ExecuteListener listener)
        {
            WebRequest request = null;

            try
            {
                // Create a new webrequest to the mentioned URL.
                request = WebRequest.Create(url);
            }
            catch (Exception e)
            {
                log.Error(string.Format("Create WebRequest for url [{0}] failed", url), e);
                listener.ExecuteCompleted(null, e);

                return null;
            }

            //Callback parameter
            RequestStateInfo requestStateInfo = new RequestStateInfo();
            requestStateInfo.url = url;
            requestStateInfo.request = request;
            requestStateInfo.listener = listener;

            IAsyncResult asyncResult = null;
            try
            {
                // Start the Asynchronous call for response.
                asyncResult = (IAsyncResult)request.BeginGetResponse(new AsyncCallback(RespCallback),
                    requestStateInfo);
            }
            catch (Exception e)
            {
                requestStateInfo.errorMessage = string.Format("Execute for BeginGetResponse [{0}] failed", url);
                ExecuteDone(requestStateInfo, e);
            }

            return asyncResult;
        }

        private void ExecuteDone(RequestStateInfo requestStateInfo, Exception e)
        {
            //log the exception if there is one
            if(e != null)
            {
                log.Error(requestStateInfo.errorMessage, e);
            }

            //close the stream and release the connection
            if(requestStateInfo.response != null)
            {
                CloseQuietly(requestStateInfo.response);
            }

            //execute the listener
            requestStateInfo.listener.ExecuteCompleted(requestStateInfo, e);
        }

        private void RespCallback(IAsyncResult asynchronousResult)
        {
            RequestStateInfo requestStateInfo = null;
            try
            {
                // Set the State of request to asynchronous.
                requestStateInfo = (RequestStateInfo)asynchronousResult.AsyncState;
                WebRequest request = requestStateInfo.request;
                // End the Asynchronous response.
                requestStateInfo.response = request.EndGetResponse(asynchronousResult);
                // Read the response into a 'Stream' object.
                Stream responseStream = requestStateInfo.response.GetResponseStream();
                requestStateInfo.responseStream = responseStream;

                // Begin the reading of the contents.
                IAsyncResult asynchronousResultRead = responseStream.BeginRead(requestStateInfo.recvBuffer, 
                    0, requestStateInfo.recvBuffer.Length, new AsyncCallback(ReadCallBack), requestStateInfo);
            }
            catch (Exception e)
            {
                requestStateInfo.errorMessage = string.Format("RespCallback [{0}] failed", requestStateInfo.url);
                ExecuteDone(requestStateInfo, e);
            }
        }

        private void ReadCallBack(IAsyncResult asyncResult)
        {
            RequestStateInfo requestStateInfo = null;
            try
            {
                // Result state is set to AsyncState.
                requestStateInfo = (RequestStateInfo)asyncResult.AsyncState;
                Stream responseStream = requestStateInfo.responseStream;
                int read = responseStream.EndRead(asyncResult);

                // Read the contents to save the responseBuffer
                if (read > 0)
                {
                    requestStateInfo.responseBuffer.AddBytes(requestStateInfo.recvBuffer, 0, read);

                    IAsyncResult asynchronousResult = responseStream.BeginRead(requestStateInfo.recvBuffer,
                        0, requestStateInfo.recvBuffer.Length, new AsyncCallback(ReadCallBack), requestStateInfo);
                }
                else
                {
                    ExecuteDone(requestStateInfo, null);
                }
            }
            catch (Exception e)
            {
                requestStateInfo.errorMessage = string.Format("ReadCallBack [{0}] failed", requestStateInfo.url);
                ExecuteDone(requestStateInfo, e);
            }
        }

        public static void CloseQuietly(WebResponse response)
        {
            try
            {
                if (response != null)
                {
                    response.Close();
                }
            }
            catch (Exception ex)
            {
                log.Warn(response, ex);
            }
        }
    }
}
