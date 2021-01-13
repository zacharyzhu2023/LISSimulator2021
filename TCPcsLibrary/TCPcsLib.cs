/*
csLib: class contained in C# DLL project called TCPcsLibrary tha tcontains functions to 
test wrapper (calling C# functions from a C++ application).
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TCPcsLibrary
{
    public class csLib
    {
        public csLib()
        {
            setA(0);
            setB(0);
            setC(0);
        }
        public csLib(int a, int b, int c)
        {
            setA(a);
            setB(b);
            setC(c);
        }


        int _a, _b, _c;

        public int add(int a, int b)
        {
            return a + b;
        }
        public int subtract(int a, int b)
        {
            return a - b;
        }
        public void printMsg()
        {
            Console.WriteLine("Message to display");
        }

        public void printValues()
        {
            Console.WriteLine("Values: {0}, {1}, {2}", _a, _b, _c);
        }

        public int getA() { return _a; }
        public int getB() { return _b; }
        public int getC() { return _c; }

        public void setA(int a) { _a = a; }
        public void setB(int b) { _b = b; }
        public void setC(int c) { _c = c; }

        public string doubleMessage(string msg)
        {
            return msg + msg;
        }
    }
}
