using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CSServerPipeTest
{
    public class CSServerPipeInstance
    {
        /* Serves as an application that creates an instance of the server pipe to receive messages.
         * Essentially serves as a template of how to export instances of CSharpPipeServers for any C# application. */
        static void Main(string[] args)
        {
            CSServerPipe.CSServerPipe serverPipe = new CSServerPipe.CSServerPipe();
            serverPipe.ServerLoop();
        }
    }
}


