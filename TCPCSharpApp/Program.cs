/*
TCPCSharpApp: C# application to test the CLI wrapper class and running TCPConnection from 
managed C# code (ability to run unmanaged C++ code).
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TCPCSharpApp
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
            bool isServer = true;
            double sendDelay = 0;
            char ACK = (char)6;
            string sACK = ACK.ToString();
            string msg1 = sACK + "default" + sACK;
            string msg2 = "default 2";
            TCPWrapperClass TCPWrapper = new TCPWrapperClass(isServer, sendDelay, msg1, msg2);
            TCPWrapper.listen("127.0.0.1", 1787);
            TCPWrapper.printMessage(msg1);
            
        }
    }
}
