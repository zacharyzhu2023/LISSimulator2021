#define DEFAULT_LENGTH 2048

#include <iostream>
#include <string>
#include <Windows.h>

using namespace std;

/* C++ version of a duplex named pipeClient (accepts the connection). Used to test against the C# pipeServer */

/* Send a message through a pipe */
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

/* Receive a message through a pipe */
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

	string serverMessage1 = "Testing a message to send.\nThe second line of the message."
		"\nThe third line of the message.\n";
	string serverMessage2 = "Sending another message.\nThis is message #2\n";
	string serverMessage3 = "The third message.\nFolk etymology dominates.\n"
		"How can this be the end?\n";
	string serverMessage4 = "Ok this is the last one.\nFour messages should be enough.\n";
	string toSend[4] = { serverMessage1, serverMessage2, serverMessage3, serverMessage4 };


	/* Connect to the server pipe */
	HANDLE clientPipe = CreateFile(L"\\\\.\\pipe\\pipeName", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (clientPipe == INVALID_HANDLE_VALUE)
	{
		cout << "Failed to connect to server pipe." << endl;
		cout << "Enter any number to exit: ";
		int ok;
		cin >> ok;
		CloseHandle(clientPipe);
		return 1;
	}
	cout << "Successfully connected to server pipe." << endl;
	/* Write data through pipe */
	DWORD bytesRead;
	char messageReceiver[DEFAULT_LENGTH];
	int counter = 0;
	while (counter < 4)
	{
		bool works = sendPipeMessage(clientPipe, toSend[counter]);
		if (!works)
		{
			return 1;
		}
		counter++;
	}

	

	/* Disconnect from the pipe */
	CloseHandle(clientPipe);
	cout << "Enter any number to exit: ";
	int ok;
	cin >> ok;
	return 0;

}