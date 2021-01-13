/*
UnmanagedWrapper: Wrapper class that allows for a C# class to be used by C++ applications. CLI code to
export C# functions for unmanaged C++ use.
*/
#pragma once
#include <string>
class UnmanagedWrapperPrivate;
class __declspec(dllexport) UnmanagedWrapper
{
private:
	UnmanagedWrapperPrivate* _private;
public:
	UnmanagedWrapper();
	~UnmanagedWrapper();
	UnmanagedWrapper(int a, int b, int c);
	int add(int a, int b);
	int subtract(int a, int b);
	void printMsg();
	void printValues();
	std::string doubleMessage(const char* msg);
};
