/*
Server: application to start a server building on the TCPConnection class--starting
the server and listening for clients & testing messaging protocols.
*/

#pragma once
#define WIN32_LEAN_AND_MEAN
#define NATIVE_CODE
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "TCPConnection.h"
#include "UnmanagedWrapper.h"
using namespace std;

/** <summary>
Create and start server; calls startServer() method to listen for new clients and communicate
with existing clients. Print and raise issues if encountered.
</summary>
*/
int main()
{
	bool isServer = true;
	string msg1 = string(1, char(6));
	string msg2 = "another default message";
	double sendDelay = 0;
	
	/* Testing integration from a C# application */
	UnmanagedWrapper wrap;
	wrap.doubleMessage("testerMsg");
	//cout << "Double message: " << wrap.doubleMessage("testerMsg") << endl;



	TCPConnection testServer(isServer, sendDelay, msg1, msg2);

	// Work IP: 10.9.74.48
	// Home IP: 192.168.1.32
	// Local IP: 127.0.0.1
	testServer.startServer("127.0.0.1", 1787);
	/* Close the server */
	closesocket(testServer.getListener());
	WSACleanup();
	return 0;
}