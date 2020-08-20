/*
TCPV5CSharpApp: C# application to test the CLI wrapper class and running TCPConnectionV5 from 
managed C# code (ability to run unmanaged C++ code).
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TCPV5CSharpApp
{
    class Program
    {
        void printCSMessage()
        {
            Console.WriteLine("Testing the other way around");
            Console.WriteLine("Just to extend the message...");
        }
        static void Main(string[] args)
        {
            bool isServer = false;
            double sendDelay = 0;
            char ACK = (char)6;
            string sACK = ACK.ToString();
            string msg1 = sACK + "default" + sACK;
            string msg2 = "default 2";
            TCPWrapperClassV5 TCPWrapper = new TCPWrapperClassV5(isServer, sendDelay, msg1, msg2);
            TCPWrapper.printMessage(msg1);
            
        }
    }
}
