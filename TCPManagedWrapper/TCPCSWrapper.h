#pragma once

class TCPCSWrapperPrivate;
class __declspec(dllexport) TCPCSWrapper
{
public:
	TCPCSWrapper();
	~TCPCSWrapper();
	int add(int a, int b);
	int subtract(int a, int b);
	void printMessage();
private:
	TCPCSWrapperPrivate* _wrapper;
};