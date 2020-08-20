/*
ClientV5: builds off TCPConnectionV5 to create an instance of a client to test the server program.
Includes the default messages for exchange in response to host queries from the server.
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

string inpL1 = string(1, char(2)) + "1H|\\^&|||Panther|||||LISHost||P|1|"
+ string(1, char(13)) + string(1, char(3)) + "8E" + string(1, char(13)) + string(1, char(10));
string inpL2 = string(1, char(2)) + "2Q|1|^SAMPLE01||ALL||||||||O"
+ string(1, char(13)) + string(1, char(3)) + "3D" + string(1, char(13)) + string(1, char(10));
string inpL3 = string(1, char(2)) + "3Q|2|^SAMPLE02||ALL||||||||O"
+ string(1, char(13)) + string(1, char(3)) + "40" + string(1, char(13)) + string(1, char(10));
string inpL4 = string(1, char(2)) + "4Q|3|^SAMPLE03||ALL||||||||O"
+ string(1, char(13)) + string(1, char(3)) + "43" + string(1, char(13)) + string(1, char(10));
string inpL5 = string(1, char(2)) + "5Q|4|^SAMPLE04||ALL||||||||O"
+ string(1, char(13)) + string(1, char(3)) + "46" + string(1, char(13)) + string(1, char(10));
string inpL6 = string(1, char(2)) + "6Q|5|^SAMPLE05||ALL||||||||O"
+ string(1, char(13)) + string(1, char(3)) + "49" + string(1, char(13)) + string(1, char(10));
string inpL7 = string(1, char(2)) + "7L|1|N|"
+ string(1, char(13)) + string(1, char(3)) + "86" + string(1, char(13)) + string(1, char(10));
string inpL8 = string(1, char(4));
string inpL9 = string(1, char(6));

string clientMessages[8] = { inpL1, inpL2, inpL3, inpL4, inpL5, inpL6, inpL7, inpL8 };

/* Alternate compressed query format to send */
string compressedQuery = string(1, char(2)) + "2Q|1|^SAMPLE01\\^SAMPLE02\\^SAMPLE03\\^SAMPLE04\\^SAMPLE05\\^SAMPLE06||ALL||||||||O"
+ string(1, char(13)) + string(1, char(3)) + "9D" + string(1, char(13)) + string(1, char(10));

string altCompressedQuery = string(1, char(2)) + "2Q|1|^SAMPLE01\\^SAMPLE06\\^SAMPLE07||ALL||||||||O"
+ string(1, char(13)) + string(1, char(3)) + "9D" + string(1, char(13)) + string(1, char(10));

string altCompressedQuery2 = string(1, char(2)) + "2Q|1|^SAMPLE01||ALL||||||||O"
+ string(1, char(13)) + string(1, char(3)) + "9D" + string(1, char(13)) + string(1, char(10));

string clientMessagesCompressed[4] = { inpL1, altCompressedQuery2, inpL7, inpL8 };

/** <summary>
Create and start client; connect with server and then start sending/receiving messages
with it. Raise errors if encountered during any step. Shutdown once done transmitting messages.
</summary>
*/
int __cdecl main(int argc, char** argv)
{
	bool isServer = false;
	string msg1 = "default 1";
	string msg2 = "default 2";
	double sendDelay = 0;
	int sendWorks, receiveWorks;

	TCPConnectionV5 testClient(isServer, sendDelay, msg1, msg2);
	testClient.startClient();
	int loop = 1;
	int counter = 0;

	/* Uncomment line below to test broadcasts */
	//testClient.receiveBroadcast(testClient.getMessager());
	
	/* Send H, Q, L queries to the Server until sending EOT + receiving ACK in response */
	while (msg1 != string(1, char(4))) 
	{
		/* Send message */
		//msg1 = clientMessages[counter];
		msg1 = clientMessagesCompressed[counter];
		sendWorks = testClient.sendMessage(testClient.getMessager(), msg1);
		if (sendWorks == SOCKET_ERROR) 
		{
			return 1;
		}

		/* Receive Message(s) */
		receiveWorks = testClient.receiveMessage(testClient.getMessager());
		counter++;
	}

	/* Send ACK following EOT */
	testClient.setSendDelay(3.2);
	sendWorks = testClient.sendMessage(testClient.getMessager(), inpL9);
	if (sendWorks == SOCKET_ERROR) {
		return 1;
	}


	testClient.setSendDelay(1.2);
	
	/* Receive Server's responses to the queries until receiving EOT */
	while (true)
	{
		/* Receive message */
		receiveWorks = testClient.receiveMessage(testClient.getMessager());
		if (receiveWorks <= 0)
		{
			return 1;
		}
		if (testClient.getRMessage() == string(1, char(4)))
		{
			break;
		}
		/* Send message */
		sendWorks = testClient.sendACK(testClient.getMessager());
		if (sendWorks == SOCKET_ERROR) 
		{
			return 1;
		}
	}

	/* Send ENQ, Receive ACK, Send EOT for idle state... repeat */
	testClient.setSendDelay(1.5);
	for (unsigned int i = 0; i < 3; i++)
	{
		sendWorks = testClient.sendENQ(testClient.getMessager());
		if (sendWorks == SOCKET_ERROR) {
			return 1;
		}
		receiveWorks = testClient.receiveMessage(testClient.getMessager());
		if (receiveWorks == SOCKET_ERROR) {
			return 1;
		}
		sendWorks = testClient.sendEOT(testClient.getMessager());
		if (sendWorks == SOCKET_ERROR) {
			return 1;
		}
	}
	testClient.clientShutdown();
	return 0;
}