using System;
using System.IO;
using System.Net;
using System.Text;

namespace CommonConsoleApp
{
    //http://msdn.microsoft.com/zh-cn/library/system.net.webrequest(v=vs.110).aspx
    public class WebRequestGetExample
    {
        public static void Run()
        {
            // Create a request for the URL. 		
            WebRequest request = WebRequest.Create("http://localhost:4129/MainHandler.ashx");
            // If required by the server, set the credentials.
            request.Credentials = CredentialCache.DefaultCredentials;
            // Get the response.
            HttpWebResponse response = (HttpWebResponse)request.GetResponse();
            // Display the status.
            Console.WriteLine(response.StatusDescription);
            // Get the stream containing content returned by the server.
            Stream dataStream = response.GetResponseStream();
            // Open the stream using a StreamReader for easy access.
            StreamReader reader = new StreamReader(dataStream);
            // Read the content.
            string responseFromServer = reader.ReadToEnd();
            // Display the content.
            Console.WriteLine(responseFromServer);
            // Cleanup the streams and the response.
            reader.Close();
            dataStream.Close();
            response.Close();
        }

        public static void Run2()
        {
            try
            {
                // Create a web request for an invalid site. Substitute the "invalid site" strong in the Create call with a invalid name.
                HttpWebRequest myHttpWebRequest = (HttpWebRequest)WebRequest.Create("http://localhost:4129/MainHandler.ashx");

                // Get the associated response for the above request.
                HttpWebResponse myHttpWebResponse = (HttpWebResponse)myHttpWebRequest.GetResponse();
                myHttpWebResponse.Close();
            }
            catch (WebException e)
            {
                Console.WriteLine("This program is expected to throw WebException on successful run." +
                                    "\n\nException Message :" + e.Message);
                if (e.Status == WebExceptionStatus.ProtocolError)
                {
                    Console.WriteLine("Status Code : {0}", ((HttpWebResponse)e.Response).StatusCode);
                    Console.WriteLine("Status Description : {0}", ((HttpWebResponse)e.Response).StatusDescription);
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }

        }
    }
}
