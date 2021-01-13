using System;

namespace CSharpPipeInstance
{
    class PipeServerInstance
    {
        /* Serves as an application that creates an instance of the server pipe to receive messages.
         * Essentially serves as a template of how to export instances of CSharpPipeServers for any C# application. */
        static void Main(string[] args)
        {
            CSharpReceiverPipe.CSharpPipeServer serverPipe = new CSharpReceiverPipe.CSharpPipeServer();
            serverPipe.ServerLoop();
        }
    }
}
