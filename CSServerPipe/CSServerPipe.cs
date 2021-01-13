
using System;
using System.IO;
using System.IO.Pipes;
using System.Text;
using System.Threading;

namespace CSServerPipe
{
    /* CSharpPipeServer creates a template for a multithreaded pipeserver class that repeatedly receives messages from each of its clients.
     * Should be able to connect w/ up to 16 clients simultaneously */
    public class CSServerPipe
    {
        bool running; // true if pipeServer still live
        Thread runningThread; // main thread through which pipeServer is initiated
        EventWaitHandle terminateHandle = new EventWaitHandle(false, EventResetMode.AutoReset); // allows manipulation of the threadState

        /* Default constructor */
        public CSServerPipe()
        {
            running = true;
        }
        /* Continuously be able to accept + process new clients */
        public void ServerLoop()
        {
            while (running)
            {
                ProcessNextClient();
            }
            terminateHandle.Set();
        }

        /* Create a thread to handle continuous server loop */
        public void Run()
        {
            running = true;
            runningThread = new Thread(ServerLoop);
            runningThread.Start();
        }
        /* End the Pipe Server--conceptually makes sense never to call (because we never
           know when we want to accept more clients ) */
        public void Stop()
        {
            running = false;
            terminateHandle.WaitOne();
        }
        /* Dummy method in case we want to do something with message received */
        public void ProcessRequest(string msg)
        {
            Console.WriteLine($"Message: {msg}");
            Console.Out.Flush();
        }
        /* Keep reading messages from the client pipe. Need to look into: does this ever
         * find a closed connection from a client? */
        public void ProcessClientThread(object o)
        {
            NamedPipeServerStream tempPipe = (NamedPipeServerStream)o;
            while (true)
            {
                StringBuilder messageBuilder = new StringBuilder();
                byte[] messageBuffer = new byte[2048];
                do
                {
                    try
                    {
                        var bytesRead = tempPipe.Read(messageBuffer, 0, messageBuffer.Length);
                        string partialMessage = Encoding.UTF8.GetString(messageBuffer);
                        messageBuilder.Append(partialMessage);
                        messageBuffer = new byte[messageBuilder.Length];
                        //Console.Write($"Bytes Read: {bytesRead}");
                    }
                    catch
                    {
                        Console.Write("Is this reached?\n"); // Don't think we ever reach this point
                        tempPipe.Close();
                        tempPipe.Dispose();
                        return;
                    }

                }
                while (!tempPipe.IsMessageComplete);

                Console.Write(messageBuilder.ToString());
                Console.Out.Flush();

            }
        }
        /* Create a new thread to work with a new client. Need to create a new instance of the serverPipe each time b/c pipe connections are 1 to 1,
         * meaning we can't use the same serverPipe (even if the "new" one has the same name) to connect w/ a new client */
        public void ProcessNextClient()
        {
            try
            {
                NamedPipeServerStream serverPipe = new NamedPipeServerStream("pipeName", PipeDirection.InOut, 254, PipeTransmissionMode.Message);
                serverPipe.WaitForConnection();
                Thread t = new Thread(ProcessClientThread);
                t.Start(serverPipe);
            }
            catch
            {
                Console.WriteLine("No more connections available.");
            }
        }

    }
}
