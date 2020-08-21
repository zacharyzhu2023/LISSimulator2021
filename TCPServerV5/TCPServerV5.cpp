/*
ServerV5: application to start a server building on the TCPConnectionV5 class--starting
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
#include "TCPConnectionV5.h"

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
	TCPConnectionV5 testServer(isServer, sendDelay, msg1, msg2);

	// Work IP: 10.9.74.48
	// Home IP: 192.168.1.19
	testServer.startServer("192.168.1.19", 1787);
	/* Close the server */
	closesocket(testServer.getListener());
	WSACleanup();
	return 0;
}