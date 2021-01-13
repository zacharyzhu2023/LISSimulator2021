#include <iostream>
#include <string>
#include <Windows.h>

#define DEFAULT_LENGTH 2048
using namespace std;

/* C++ application for a duplex named server pipe (initiates the connection). Useful point of comparison against C# server pipe. 
   Stipulation: this version hasn't been tested against numerous clients simultaneously. */

/* Send a message through the pipe */
bool sendPipeMessage(HANDLE pipe, string msg)
{
	int counter = 0;
	while (true)
	{
		DWORD bytesWritten = 0;
		bool works = WriteFile(pipe, msg.c_str(), msg.length(), &bytesWritten, NULL);
		if (works)
		{
			cout << "Success! Bytes sent: " << bytesWritten << endl;
			cout << "Message sent: " << msg << endl;
			return true;
		}
		else
		{
			cout << "Failed to send data. Error: " << GetLastError() << endl;
			CloseHandle(pipe);
			return false;
		}
		counter++;
	}
}

/* Receive a message through the pipe */
bool receivePipeMessage(HANDLE pipe)
{
	DWORD bytesRead;
	char messageReceiver[DEFAULT_LENGTH];
	bool works = ReadFile(pipe, messageReceiver, DEFAULT_LENGTH, &bytesRead, NULL);
	if (works)
	{
		cout << "Bytes read: " << bytesRead << endl;
		printf("Message read:\n%.*s", bytesRead, messageReceiver);
		//cout << endl;
		return true;
	}
	else
	{
		cout << "Failed to receive message." << endl;
		CloseHandle(pipe);
		return false;
	}
}

/* Test the functionality of creating a client pipe, connecting to server, then exchanging messages */
int main()
{
	/* Creating an instance of a named pipe */
	// Pipe names start with: \\.pipe\
	
	HANDLE serverPipe = CreateNamedPipe(L"\\\\.\\pipe\\pipeName", PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE,
		1, 0, 0, 0, NULL);
	if (serverPipe == NULL || serverPipe == INVALID_HANDLE_VALUE)
	{
		cout << "Failed to create server pipe." << endl;
		return 1;
	}

	/* Waiting for client program to connect */
	bool works = ConnectNamedPipe(serverPipe, NULL); // Wait for client to connect
	if (!works)
	{
		cout << "Client connection failed" << endl;
		CloseHandle(serverPipe);
		return 1;
	}
	else
	{
		cout << "Success! Client has connected." << endl;
	}
	/* Sending data down the pipe */
	/*string serverMessage1 = "Testing a message to send.\nThe second line of the message."
		"\nThe third line of the message.\n";
	string serverMessage2 = "Sending another message.\nThis is message #2\n";
	string serverMessage3 = "The third message.\nFolk etymology dominates.\n"
		"How can this be the end?\n";
	string serverMessage4 = "Ok this is the last one.\nFour messages should be enough.\n";
	string toSend[4] = { serverMessage1, serverMessage2, serverMessage3, serverMessage4 };*/
	int counter = 0;
	while (counter < 4)
	{
		bool works = receivePipeMessage(serverPipe);
		if (!works)
		{
			return 1;
		}
		counter++;
	}

	/* Disconnect/close pipe instance */
	CloseHandle(serverPipe);
	return 0;

}