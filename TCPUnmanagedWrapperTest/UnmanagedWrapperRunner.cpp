/*
UnmanagedWrapperRunner: unmanaged C++ application to test the functionality of the UnmanagedWrapper
and C# DLL class/functions.
*/

#include <iostream>
#include "UnmanagedWrapper.h"
using namespace std;
int main()
{
	UnmanagedWrapper wrap;
	wrap.printMsg();
	UnmanagedWrapper wrap2(1, 5, 9);
	wrap2.printValues();
	cout << "Doubled: " << wrap2.doubleMessage("ToDouble") << endl;
}