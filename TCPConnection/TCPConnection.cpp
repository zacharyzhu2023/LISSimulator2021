#define NATIVE_CODE
#include "TCPConnection.h"
#pragma once
/**
<summary>
Function to print out time in HH:MM:SS:MMM format in local time.
</summary>
<returns></returns>
*/
void TCPConnection::printTime()
{
	SYSTEMTIME currTime;
	GetLocalTime(&currTime);
	printf("%02d:%02d:%02d:%03d", currTime.wHour, currTime.wMinute, currTime.wSecond, currTime.wMilliseconds);
	//DWORD end = GetTickCount();
	DWORD end = (DWORD) GetTickCount64();
	printf(" (%03d ms): ", end - _lastEvent);
	_lastEvent = end;
}

/**
<summary>
Function to get string representation of time in HH:MM:SS:MMM format in local time.
</summary>
<returns>String version of time</returns>
*/
string TCPConnection::getTime()
{
	SYSTEMTIME currTime;
	GetLocalTime(&currTime);
	//DWORD end = GetTickCount();
	DWORD end = (DWORD) GetTickCount64();
	char time[15];
	sprintf_s(time, sizeof(time), "%02d:%02d:%02d:%03d: ", currTime.wHour, currTime.wMinute, currTime.wSecond, currTime.wMilliseconds);
	return time;
}

/**
<summary>
Function to print out chars that can't be printed inherently.
</summary>
<returns></returns>
<param name = "c">Char to be printed</param>
*/
void TCPConnection::printChar(char c)
{
	if (c == char(0))
	{
		//printf("<NUL>");
		cout << "<NUL>";
	}
	else if (c == char(1))
	{
		//printf("<SOH>");
		cout << "<SOH>";
	}
	else if (c == char(2))
	{
		//printf("<STX>");
		cout << "<STX>";
	}
	else if (c == char(3))
	{
		//printf("<ETX>");
		cout << "<ETX>";
	}
	else if (c == char(4))
	{
		//printf("<EOT>");
		cout << "<EOT>";
	}
	else if (c == char(5))
	{
		//printf("<ENQ>");
		cout << "<ENQ>";
	}
	else if (c == char(6))
	{
		//printf("<ACK>");
		cout << "<ACK>";
	}
	else if (c == char(21))
	{
		//printf("<NAK>");
		cout << "<NAK>";
	}
	else if (c == char(10))
	{
		//printf("<LF>");
		cout << "<LF>";
	}
	else if (c == char(13))
	{
		//printf("<CR>");
		cout << "<CR>";
	}
	else {
		//printf("%c", c);
		cout << c;
	}
}

/**
<summary>
Print the message sent/received char by char using printChar() method above.
</summary>
<returns></returns>
<param name = "msg">Complete message to print out.</param>
*/
void TCPConnection::printMessage(string msg)
{
	for (unsigned int i = 0; i < msg.length(); i++)
	{
		printChar(msg[i]);
	}
	cout << endl;
}

string getLogMessage(string msg)
{
	string formattedMsg;
	for (unsigned int i = 0; i < msg.length(); i++)
	{
		char c = msg[i];
		if (c == char(0))
		{
			formattedMsg += "<NUL>";
		}
		else if (c == char(1))
		{
			formattedMsg += "<SOH>";
		}
		else if (c == char(2))
		{
			formattedMsg += "<STX>";
		}
		else if (c == char(3))
		{
			formattedMsg += "<ETX>";
		}
		else if (c == char(4))
		{
			formattedMsg += "<EOT>";
		}
		else if (c == char(5))
		{
			formattedMsg += "<ENQ>";
		}
		else if (c == char(6))
		{
			formattedMsg += "<ACK>";
		}
		else if (c == char(21))
		{
			formattedMsg += "<NAK>";
		}
		else if (c == char(10))
		{
			formattedMsg += "<LF>";
		}
		else if (c == char(13))
		{
			formattedMsg += "<CR>";
		}
		else 
		{
			formattedMsg += c;
		}
	}
	return formattedMsg;
}

/**
<summary>
Timer to delay sending a message
</summary>
<returns></returns>
<param name = "time">Amount of time to delay </param>
*/
void TCPConnection::timeDelay(double time)
{
	Sleep((DWORD) time * 1000);
}

/**
<summary>
Method to get the time elapsed from the previous time set and start
the timer anew.
</summary>
<returns>The time elapsed from previous message in seconds</returns>
<param name = "s"> Relevant client for which to "make time received" </param>
*/
double TCPConnection::makeTimeReceived(SOCKET s)
{
	//DWORD newTime = GetTickCount();
	DWORD newTime = (DWORD) GetTickCount64();
	double timeElapsed = newTime - _socketTimers[s];
	_socketTimers[s] = newTime;
	return timeElapsed/1000;
}

/**
<summary>
Method that clears the <STX> and the ending characters to make it easier to process
messages.
</summary>
<returns>Formatted string</returns>
<param name = "inp">String to parse for formatting</param>
*/
string TCPConnection::parseFormatting(string inp)
{
	if (inp.find('|') != string::npos && inp.find(CR) != string::npos)
	{
		int start = inp.find_first_of('|') - 1;
		int end = inp.find_first_of(char(13));
		return inp.substr(start, end - start);
	}
	return inp;
}

/**
<summary>
Method that adds <STX>, line number, checksum, and other characters for server to
send back to a client.
messages.
</summary>
<returns>Formatted message from server</returns>
<param name = "lineNum">Message number to add as line number</param>
<param name = "inp">String to add formatting for</param>
*/
string TCPConnection::makeFormattedMessage(int lineNum, string inp)
{
	string formatted = sSTX + to_string(lineNum % 8) + inp
		+ sCR + sETX + "00" + sCR + sLF;
	string checksum = generateChecksum(formatted);
	formatted = sSTX + to_string(lineNum) + inp
		+ sCR + sETX + checksum + sCR + sLF;
	return formatted;
}
/**
<summary>
Parses the patient and assay database to return a vector of the string representations
for the relevant patient/assay messages corresponding to the sampleID given.
</summary>
<returns>Vector of patient/assay messages </returns>
<param name = "sampleID">sampleID to process against the database</param>
*/
vector<string> TCPConnection::getLISMessage(string sampleID)
{
	vector<string> output;
	//locker.try_lock();
	if (patientMap.find(sampleID) != patientMap.end())
	{
		output.push_back(patientMap.at(sampleID).makePatientMessage());
	}
	else
	{
		string fakePatientMessage = "P|1||||^^||^^Year|Other||||||||||||||||||";
		output.push_back(fakePatientMessage);
	}
	if (patientMap.find(sampleID) != patientMap.end())
	{
		vector<Assay> vectorAssay = assayMap.at(sampleID);
		for (Assay a : vectorAssay)
		{
			output.push_back(a.getSampleMessage());
		}
	}
	else
	{
		const __int64 requestTime = 19980506090909;
		string fakeAssayMessage = "O|1|NOASSAY-000002|||R|" + 
			Header::getCurrDateTime() + "|||||||||Serum||||||||||Y|";
		output.push_back(fakeAssayMessage);
	}
	//locker.unlock();
	return output;
}

/**
<summary>
Takes in a line of input from the client and based on whether it's a header, query, or L (termination),
creates vector of responses.
</summary>
<returns>String vector of responses </returns>
<param name = "messager">Client for which to process the message</param>
<param name = "inp">String input line to process</param>
*/
vector<string> TCPConnection::parsePantherMessage(SOCKET messager, string inp)
{
	vector<string> responses;
	if (inp[0] == 'H')
	{
		Header h(inp);
		h.setReceiver(getSocketFromHeader(messager));
		h.setSender(getSocketToHeader(messager));
		h.setDelimiters(getSocketDelimiters(messager));
		responses.push_back(h.getHeaderResponse());
		return responses;
	}
	else if (inp[0] == 'Q')
	{
		int start = Header::findNthOccur(inp, '|', 2) + 2;
		int len = Header::findNthOccur(inp, '|', 3) - start;
		string sampleID = inp.substr(start, len);
		return parseQuery(sampleID);
	}
	else if (inp[0] == 'L')
	{
		responses.push_back(inp);
		return responses;
	}
	else
	{
		responses.push_back("Invalid input");
		return responses;
	}
}

/**
<summary>
Takes in a query line from processPantherMessage and return an output to support
the compressed query.
</summary>
<returns>String vector of responses to all queries (even compressed) </returns>
<param name = "compressedQuery">Input to break down into sampleIDs</param>
*/
vector<string> TCPConnection::parseQuery(string compressedQuery)
{
	vector<string> sampleIDs;
	vector<string> allMessages;
	int index;
	string ID;
	while ((index = compressedQuery.find('\\')) != string::npos)
	{
		ID = compressedQuery.substr(0, index);
		sampleIDs.push_back(ID);
		compressedQuery.erase(0, index + 2);
	}
	sampleIDs.push_back(compressedQuery);
	for (string ID : sampleIDs)
	{
		vector<string> patientSampleMessages = getLISMessage(ID);
		for (string message : patientSampleMessages)
		{
			allMessages.push_back(message);
		}
	}
	return allMessages;
}

/**
<summary>
Initialize Winsock for the client--print error if failure on startup
or getting addressinfo.
</summary>
<param name = ipAddress>IP Address for the client application</param>
<returns></returns>
*/
void TCPConnection::initWinsock(string ipAddress)
{
	_works = WSAStartup(MAKEWORD(2, 2), &_wsaData); // Same as client
	if (_works != 0)
	{
		cout << "Error on WSAStartup" << endl;
		return; // Indicates error encountered
	}
	ZeroMemory(&_hints, sizeof(_hints)); // Fills block of memory w/ 0's
	_hints.ai_family = AF_INET;
	_hints.ai_socktype = SOCK_STREAM;
	_hints.ai_protocol = IPPROTO_TCP;

	/* Getting local address/port */
	_works = getaddrinfo(ipAddress.c_str(), "1787", &_hints, &_result);
	if (_works != 0)
	{
		cout << "Error on getaddrinfo" << endl;
		WSACleanup();
		return;
	}
}

/**
<summary>
Creates a connecting socket and connects to the server from a client.
Prints an error message if error occurs during socket creation.
</summary>
<returns> -1 if an error occurs, 0 if no error occurs </returns>
*/
int TCPConnection::findClientMessager()
{

	_messager = INVALID_SOCKET;
	string formattedMsg1, formattedMsg2, formattedMsg3;
	for (_ptr = _result; _ptr != NULL; _ptr = _ptr->ai_next)
	{

		/* Create the connecting socket */
		_messager = socket(_ptr->ai_family, _ptr->ai_socktype, _ptr->ai_protocol); // Create socket
		if (_messager == INVALID_SOCKET)
		{
			cout << "Error during socket creation" << endl;
			WSACleanup();
			return -1;
		}
		
		formattedMsg1 = getTime() + "Created connecting socket";
		//printTime();
		//cout << "Created connecting socket" << endl;
		/* Connecting to the server */
		_works = connect(_messager, _ptr->ai_addr, (int)_ptr->ai_addrlen);
		if (_works == SOCKET_ERROR)
		{
			closesocket(_messager);
			_messager = INVALID_SOCKET;
			continue;
		}
		else
		{
			//printTime();
			//cout << "Connected to the server" << endl;
			formattedMsg2 = getTime() + "Connected to the server";
		}
		break;
	}
	freeaddrinfo(_result);
	if (_messager == INVALID_SOCKET)
	{
		cout << "Error during server connection" << endl;
		WSACleanup();
		return -1;
	}
	//printTime();
	//cout << "Success: client initiated!" << endl;
	formattedMsg3 = getTime() + "Success: client initiated!";
	printMessage(formattedMsg1);
	printMessage(formattedMsg2);
	printMessage(formattedMsg3);
	addSocketLog(_messager, formattedMsg1 + "\n" + formattedMsg2 + "\n" + formattedMsg3 + "\n");


	return 0;
}


/**
<summary>
Runs initWinsock() and findClientMessager()
</summary>
<return></return>
*/
void TCPConnection::startClient()
{
	/* 10.9.74.48 is the IP address passed--can be configured */
	// Work IP: 10.9.74.48
	// Home IP: 192.168.1.32
	// Local IP: 127.0.0.1
	initWinsock("127.0.0.1");
	findClientMessager();
}

/**
<summary>
Make the broadcast database for patient ID's and assay vectors,
which will be used to test broadcasting
</summary>
<param name = "numIDs">Number of ID's to create</param>
*/
void TCPConnection::makeBroadcastDatabase(const int numIDs)
{
	int lineNumber = 1;
	for (int i = 0; i < numIDs; i++)
	{
		PatientInformation patientInfo{ "PatID" + to_string(i), "1", "Meier^Anna^", "19741001", "F", "Martinez" };
		Assay assayInfo{ "SAMPLE" + to_string(i), "1", "CT/GC", "", "1", "1", "R", "19980506090909", "N", "Serum", "O" };
		_broadcastPatientMap.insert({ "SAMPLE" + to_string(i), patientInfo });
		vector<Assay> aVector{ assayInfo };
		_broadcastAssayMap.insert({ "SAMPLE" + to_string(i), aVector });
	}
}


void TCPConnection::addToDatabase(string ID, PatientInformation patient, vector<Assay> aVector)
{
	if (patientMap.size() != _maxDatabaseSize)
	{
		patientMap.insert({ ID, patient });
		assayMap.insert({ ID, aVector });
	}
}

void TCPConnection::deleteFromDatabase(string ID)
{
	patientMap.erase(ID);
	assayMap.erase(ID);
}

/**
<summary>
Initialize Winsock. Then, create, bind, and start listening from a listener
socket, printing any errors if encountered along the way.
</summary>
<param name = "ipAddress">IP address on which to start the server</param>
<param name = "port">Port number on which to start listening on</param>
<return></return>
*/
void TCPConnection::startServer(string ipAddress, int port)
{
	/* Initialize Winsock */
	//cout << "Is this working 1?" << endl;
	int start;
	string startupMessage;
	sockaddr_in SERVER;
	SERVER.sin_family = AF_INET;
	wstring ipResult = wstring(ipAddress.begin(), ipAddress.end());
	const wchar_t* formattedIP = ipResult.c_str();
	InetPton(AF_INET, formattedIP, &SERVER.sin_addr.s_addr);
	//SERVER.sin_port = htons(1787);
	SERVER.sin_port = htons(port);
	start = WSAStartup(MAKEWORD(2, 2), &_wsaData);
	if (start != 0)
	{
		cout << "Error on WSAStartup: " << start << endl;
	}
	//cout << "Is this working 2?" << endl;
	/* Create socket that will connect to server */
	_listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (_listener == INVALID_SOCKET)
	{
		cout << "Error creating socket to connect to server: " << WSAGetLastError() << endl;
		WSACleanup();
	}

	//cout << "Is this working 3?" << endl;

	/* Bind the socket */
	start = bind(_listener, (sockaddr*)&SERVER, sizeof(SERVER));
	if (start == SOCKET_ERROR)
	{
		cout << "Error on bind:" << WSAGetLastError() << endl;
		closesocket(_listener);
		WSACleanup();
	}

	//cout << "Is this working 4?" << endl;

	/* Create the listener socket */
	start = listen(_listener, 16);
	if (start == SOCKET_ERROR)
	{
		cout << "Error on entering the listening state: " << start << endl;
		closesocket(_listener);
		WSACleanup();
	}

	//cout << "Is this working 5?" << endl;
	//printTime();
	//cout << "Server entered listening state" << endl;
	_startupMessage += getTime() + ": Server entered listening state";

	
	/* Uncomment to test broadcast */
	makeBroadcastDatabase(50000);

	/* Create client pipe for server to send sent/received messages through*/
	//makeServerPipe();
	makeClientPipe();

	cout << "Client pipe has successfully connected!" << endl;

	/* Create the thread */
	sockaddr_in client;
	int clientSize = sizeof(client);
	while (true)
	{
		SOCKET messager = accept(_listener, (struct sockaddr*)&client, &clientSize);
		//locker.lock();
		//printTime();
		//cout << "Is this working 7?" << endl;
		if (messager != SOCKET_ERROR)
		{
			//cout << "Is this working 8?" << endl;
			//int x1;
			//cin >> x1;
			// cout << "Client Connection success!" << endl;
			printMessage(_startupMessage);
			// addSocketLog(messager, _startupMessage + '\n');
			_startupMessage = getTime() + "Client Connection success!";
			printMessage(_startupMessage);
			addSocketLog(messager, _startupMessage + '\n');
			//locker.unlock();
			std::thread newThread([&, messager] {this->exchange(messager); });
			newThread.detach();
		}
		else
		{
			//locker.unlock();
		}
	}
}


/**
<summary>
Create a server pipe through which the sent/received messages can be sent through
</summary>
<return>Return 0 upon success, 1 on failure</return>
*/
int TCPConnection::makeServerPipe()
{
	/* Creating an instance of a named pipe */
	// Pipe names start with: \\.pipe\
	
	_serverPipe = CreateNamedPipe(L"\\\\.\\pipe\\pipeName", PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE,
		1, 0, 0, 0, NULL);
	if (_serverPipe == NULL || _serverPipe == INVALID_HANDLE_VALUE)
	{
		cout << "Failed to create server pipe." << endl;
		return 1;
	}

	/* Waiting for client program to connect */
	bool works = ConnectNamedPipe(_serverPipe, NULL); // Wait for client to connect
	if (!works)
	{
		cout << "Client connection failed" << endl;
		CloseHandle(_serverPipe);
		return 1;
	}
	else
	{
		cout << "Success! Client has connected." << endl;
		return 0;
	}
}

/**
<summary>
Create a client pipe to receive the messages of the server pipe
</summary>
<return>Return 0 upon success, 1 on failure</return>
*/
int TCPConnection::makeClientPipe()
{
	while (true)
	{
		_clientPipe = CreateFile(L"\\\\.\\pipe\\pipeName", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (_clientPipe == INVALID_HANDLE_VALUE)
		{
			cout << "Failed to connect to server pipe." << endl;
			CloseHandle(_clientPipe);
			return 1;
		}
		return 0;
	}
	
}

/**
<summary>
Send a message through the "server" pipe to indicate that a message has been exchanged
b/w server + client.
</summary>
<param name = "pipe">HANDLE which provides the pipe to send through</param>
<param name = "msg">Message that was sent/received by the server</param>
<return>True if no errors occurred, false if problem encountered</return>
*/
bool TCPConnection::sendPipeMessage(HANDLE pipe, string msg)
{
	
	int counter = 0;
	while (true)
	{
		locker.lock();
		DWORD bytesWritten = 0;
		bool works = WriteFile(pipe, msg.c_str(), msg.length(), &bytesWritten, NULL);
		locker.unlock();
		if (works)
		{
			//cout << "Success! Bytes sent: " << bytesWritten << endl;
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

/**
<summary>
Receive a message through the "server" pipe to indicate that a message has been exchanged
b/w server + client.
</summary>
<param name = "pipe">HANDLE which provides the pipe to send through</param>
<param name = "msg">Message that was sent/received by the server</param>
<return>True IFF no errors were encountered</return>
*/
bool TCPConnection::receivePipeMessage(HANDLE pipe)
{
	DWORD bytesRead;
	char messageReceiver[DEFAULT_LENGTH];
	bool works = ReadFile(pipe, messageReceiver, DEFAULT_LENGTH, &bytesRead, NULL);
	if (works)
	{
		//cout << "Bytes read: " << bytesRead << endl;
		//printf("Message read:\n%.*s", bytesRead, messageReceiver);
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

/**
<summary>
Account for the desired time delay of message. Send the message specified to the desired
socket for both client/server. Print the time message was sent.
</summary>
<returns> Returns -1 if error encountered, return length of message if successful </returns>
<param name = "messager">The socket through which to send a message</param>
<param name = "message">Message to be sent</param>
*/
int TCPConnection::sendMessage(SOCKET messager, string message)
{
	timeDelay(_sendDelay);
	_works = send(messager, message.c_str(), message.length(), 0);
	//locker.lock();
	//printTime();

	if (_works == SOCKET_ERROR)
	{
		cout << "Error on sending message" << endl;
		//locker.unlock();
		return -1;
	}
	else
	{

		string formattedMsg;
		if (_isServer)
		{
			//cout << "SERVER sends to client " << messager << " : ";
			/* Timer support */
			makeTimeReceived(messager);
			if (message == sENQ)
			{
				setSocketPhase(messager, 2);
				setTransferSender(messager, false);
			}
			//locker.lock();
			formattedMsg = getLogMessage(getTime() + "SERVER sent to client " + to_string(messager) + ": " + message);
			sendPipeMessage(getClientPipe(), formattedMsg + "\n");
			//locker.unlock();
		}
		else
		{
			formattedMsg = getLogMessage(getTime() + "CLIENT sent to server: " + message);
		}
		//formattedMsg = getLogMessage(formattedMsg);
		printMessage(formattedMsg);
		addSocketLog(messager, formattedMsg + "\n");
		/*if (formattedMsg.back() == '\n')
		{
			addSocketLog(messager, formattedMsg);
		}
		else
		{
			addSocketLog(messager, formattedMsg + "\n");
		}*/
	}
	//locker.unlock();
	_sender = false;
	_sendCounter++;
	return _works;
}

/**
<summary>
Use sendMessage() to send EOT character as a message.
</summary>
<return> Returns -1 if error encountered, return 1 if successful </return>
<param name = "messager">The socket through which to send EOT message</param>
*/
int TCPConnection::sendEOT(SOCKET messager)
{
	return sendMessage(messager, sEOT);
}

/**
<summary>
Use sendMessage() to send ENQ character as a message.
</summary>
<return> Returns -1 if error encountered, return 1 if successful </return>
<param name = "messager">The socket through which to send ENQ message</param>
*/
int TCPConnection::sendENQ(SOCKET messager)
{
	return sendMessage(messager, sENQ);
}

/**
<summary>
Use sendMessage() to send ACK character as a message.
</summary>
<return> Returns -1 if error encountered, return 1 if successful </return>
<param name = "messager">The socket through which to send ACK message</param>
*/
int TCPConnection::sendACK(SOCKET messager)
{
	return sendMessage(messager, sACK);
}

/**
<summary>
Client/Server attempts to receive a message. Print time and message if successful.
If received 0 bytes, print/indicate a disconnection.
</summary>
<return> Returns -1 if error encountered, return 1 if successful </return>
<param name = "messager">The socket through which to receive a message</param>
*/
int TCPConnection::receiveMessage(SOCKET messager)
{
	char receiveMsg[DEFAULT_LENGTH] ;
	int rWorks = recv(messager, receiveMsg, (int)strlen(receiveMsg), 0);
	//locker.lock();
	if (rWorks > 0)
	{
		string time = getTime();
		string msg;
		msg.assign(receiveMsg, rWorks);
		string formattedMsg;
		if (_isServer)
		{
			//locker.lock();
			formattedMsg = getLogMessage(getTime() + "SERVER received from client " + to_string(messager) + ": " + msg);
			sendPipeMessage(getClientPipe(), formattedMsg + "\n");
			//locker.unlock();
		}
		else
		{
			formattedMsg = getLogMessage(getTime() + "CLIENT received from server: " + msg);
		}
		setRMessage(msg);
		//formattedMsg = getLogMessage(formattedMsg);
		addSocketLog(messager, formattedMsg + "\n");
		printMessage(formattedMsg);
		/*printMessage(msg);
		printTime();
		if (_isServer)
		{
			cout << "SERVER received from client " << messager << ": ";
		}
		else
		{
			cout << "CLIENT received from server: ";
		}*/
		
		if (_isServer)
		{
			//addSocketLog(messager, getTime() + "SERVER received: " + msg + "\n");
			/* Mechanisms to support timeout */
			double timeToReceive = makeTimeReceived(messager);
			//cout << "Phase: " << getSocketPhase(messager) << endl;
			//cout << "Time received: " << timeToReceive << endl;
			if (getSocketPhase(messager) == 1)
			{
				if (timeToReceive > 15)
				{
					cout << "Timeout during establishment phase." << endl;
					setSocketPhase(messager, 3);
				}
			}
			else if (getSocketPhase(messager) == 2)
			{
				if (getTransferSender(messager) && timeToReceive > 15 || (!_transferSender && timeToReceive > 30))
				{
					cout << "Timeout during transfer phase." << endl;
					setSocketPhase(messager, 3);
				}
			}
			if (msg == sENQ)
			{
				setSocketPhase(messager, 2);
				setTransferSender(messager, true);
			}
			else if (msg == sEOT)
			{
				setSocketReceivedEOT(messager, true);
			}
		}

		_sender = true;
		string cleanedMessage = "";
		if (msg.find(CR) != string::npos && msg.find('|') != string::npos)
		{
			cleanedMessage = parseFormatting(msg);
		}
		if (cleanedMessage != "")
		{
			if (_isServer && (cleanedMessage[0] == 'H' || cleanedMessage[0] == 'Q' || cleanedMessage[0] == 'L'))
			{
				addHostQuery(messager, cleanedMessage);
			}
		}
		//locker.unlock();
		return rWorks;
	}
	else if (rWorks == 0)
	{
		printTime();
		if (_isServer)
		{
			cout << "Client " << messager << " disconnected" << endl;
		}
		else
		{
			cout << "SERVER disconnected." << endl;
		}
		//locker.unlock();
		return 0;
	}
	//locker.unlock();
	return -1;
}

/**
<summary>
Set the default settings for a given client.
</summary>
<param name = "messager">Socket ID for which to set the defaults</param>
<return></return>
*/
void TCPConnection::setMessagerDefaults(SOCKET messager)
{
	_socketReceivedEOT[messager] = false;
	//_socketTimers[messager] = GetTickCount();
	_socketTimers[messager] = (DWORD) GetTickCount64();
	_socketPhase[messager] = 1;
	_socketLog[messager] = _startupMessage;
	_socketToHeaders[messager] = _toHeader;
	_socketFromHeaders[messager] = _fromHeader;
	_socketDelimiters[messager] = _delimiters;
	_socketAutoOrders[messager] = _autoOrders;
	_socketBidPriority[messager] = _bidPriority;
	_socketNumLineBids[messager] = _numLineBids;
	_socketTimeout[messager] = _msTimeout;
}

/**
<summary>
Server's send/receive method: continually send/receive data through messager
socket while client is available. Also can test broadcast.
</summary>
<param name = "messager">Socket through which exchange can occur</param>
<return> ExitsThread(0) if client closed or error on send/receive </return>
*/

DWORD TCPConnection::exchange(SOCKET messager)
{
	setMessagerDefaults(messager);
	/* Uncomment lines below to test normal send/receive query + response */
	receiverExchange(messager);
	return senderExchange(messager);
	/* Uncomment the lines below to test broadcast */
	//return sendBroadcast(messager);
}


/**
<summary>
Protocol for the server as the sender for a response to a query. 
1. Send ENQ, receive ACK
2. Send responses, receive ACK
3. Send EOT when finished-then enter serverIdle state
</summary>
<return> Returns 0 if error encountered or when finished </return>
<param name = "messager">Client through which exchange occurs </param>
*/
DWORD TCPConnection::senderExchange(SOCKET messager)
{
	//cout << "When do we get here?\n\n" << endl;
	int working = sendENQ(messager);
	if (working == SOCKET_ERROR)
	{
		cout << "Error on send from socket " << messager << endl;
		closesocket(messager);
		return 0;
	}
	working = receiveMessage(messager);
	if (working == SOCKET_ERROR)
	{
		cout << "Error on receive from socket " << messager << endl;
		closesocket(messager);
		return 0;
	}
	/* Accounting for state of contention */
	if (getSocketReceivedENQ(messager))
	{
		return receiverExchange(messager);
	}

	/* Generating the responses to the received messages */
	unsigned int counter = 1;
	vector<string> allMessagesToSend;
	for (string inpQuery : _socketHostQueries[messager])
	{
		vector<string> response = parsePantherMessage(messager, inpQuery);
		for (string r : response)
		{
			allMessagesToSend.push_back(makeFormattedMessage(counter++, r));
		}
	}

	/* Sending all the responses to the generated messages */
	for (string message : allMessagesToSend)
	{
		int bytesSent = sendMessage(messager, message);
		if (bytesSent <= 0)
		{
			closesocket(messager);
			return 0;
		}
		else
		{
			int bytesReceived = receiveMessage(messager);
			if (bytesReceived == 0)
			{
				closesocket(messager);
				return 0;
			}
			else if (bytesReceived < 0)
			{
				cout << "Error on receive from socket " << messager << endl;
				closesocket(messager);
				return 0;
			}
		}
	}

	/* Send an EOT after providing all patients/queries */
	int bytesSent = sendMessage(messager, sEOT);
	if (bytesSent <= 0)
	{
		closesocket(messager);
		return 0;
	}
	/*string log = getSocketLog(messager);
	cout << "\n\nMESSAGE LOG\n\n" << endl;
	cout << log;*/
	serverIdle(messager);
	return 0;
}

/**
<summary>
Protocol for the server as the sender for a response to a query.
1. Receive ENQ, send ACK
2. Receive message, reply with ACK until receiving EOT
</summary>
<return> Returns 0 if error encountered or when finished </return>
<param name = "messager">Client through which exchange occurs</param>
*/
DWORD TCPConnection::receiverExchange(SOCKET messager)
{
	/* 
	Receive ENQ, send ACK
	Then, receive + reply with ACK until receiving EOT 
	*/
	int working;
	while (true)
	{
		working = receiveMessage(messager);
		if (getRMessage() == string(1, char(4)))
		{
			setSocketReceivedEOT(messager, false);
			return 0;
		}
		if (working <= 0)
		{
			cout << "Error on receive from socket " << messager << endl;
			closesocket(messager);
			return 0;
		}
		working = sendACK(messager);
		if (working <= 0)
		{
			cout << "Error on send from socket " << messager << endl;
			closesocket(messager);
			return 0;
		}
	}
	return 0;
}



/**
<summary>
Server's idle state after receiving query from the client and responding with the patient
and sample information.
</summary>
<param name = "messager">Provide the socket for which message exchange can occur</param>
<return> Returns when server done sending messages </return>
*/

DWORD TCPConnection::serverIdle(SOCKET messager)
{
	/* Receive ENQ, send ACK, receive EOT... repeat */
	while (true)
	{
		int bytesReceived = receiveMessage(messager);
		if (bytesReceived == 0)
		{
			closesocket(messager);
			return 0;
		}
		else if (bytesReceived > 0)
		{
			int bytesSent = sendMessage(messager, sACK);
			if (bytesSent == -1)
			{
				closesocket(messager);
				return 0;
			}
		}
		else
		{
			closesocket(messager);
			return 0;
		}
		bytesReceived = receiveMessage(messager);
		if (bytesReceived == 0)
		{
			closesocket(messager);
			return 0;
		}
		else if (bytesReceived < 0)
		{
			closesocket(messager);
			return 0;
		}
	}
}

/**
<summary>
Create a new client in the _clients table, based on the number
of clients already present in the table and the SOCKET ID.
</summary>
<returns></returns>
<param name = "x">The position in _clients table </param>
<param name = "s">The socket ID</param>
*/
void TCPConnection::makeClient(int x, SOCKET s)
{
	setOneClientClient(x, s);
	setOneClientConnected(x, true);
	setOneClientSendACK(x, false);
	setOneClientReceiveACK(x, false);
	if (_isServer)
	{
		_numClients++;
		_numClientsConnected++;
		//locker.lock();
		printTime();
		cout << "New client: " << s << endl;
		//locker.unlock();
	}
}


/**
<summary>
Function to shutdown a client from the server, raising error if one is encoutnered.
</summary>
<param name = "messager">Client for which to close the socket </param>
<returns>-1 if error encountered, 0 otherwise </returns>
*/

void TCPConnection::closeConnection(SOCKET messager)
{
	string formattedMsg = getTime() + ": Server shutdown connection with CLIENT " + to_string(messager);
	printMessage(formattedMsg);
	closesocket(messager);
	WSACleanup();
	
}

/**
<summary>
Function to shutdown a client, raising error if one is encoutnered.
</summary>
<returns>-1 if error encountered, 0 otherwise </returns>
*/
int TCPConnection::clientShutdown()
{
	/* Shutdown connection for sending */
	string formattedMsg = getTime() + ": Client closed and shutdown";
	cout << formattedMsg << endl;
	addSocketLog(getMessager(), formattedMsg);
	//cout << "\n\n\n\nSOCKET LOG\n\n" << getSocketLog(getMessager()) << endl;
	_works = shutdown(getMessager(), SD_SEND);
	if (_works == SOCKET_ERROR) {
		cout << "Error on shutdown: " << WSAGetLastError() << endl;
		closesocket(getMessager());
		WSACleanup();
		return 1;
	}

	/* Shutdown and cleanup */
	closesocket(getMessager());
	WSACleanup();
	
	//printTime();
	//cout << ": Closed and shutdown" << endl;

	/* Safe exit */
	//printf("Enter any number to exit: ");
	cout << "Enter any number to exit: ";
	int ok;
	cin >> ok;
	return 0;
}


/**
<summary>
Generate the checksum (2-digit hex symbol) that corresponds to
the message issued.
</summary>
<returns>Checksum quantity</returns>
<param name = "message">Message for which to generate a checksum</param>
*/
string TCPConnection::generateChecksum(string message)
{
	int index = message.find_first_of(CR);
	int calculatedChecksum = 0;
	for (int i = 1; i < index + 2; i++)
	{
		calculatedChecksum += (int)message[i];
	}
	calculatedChecksum = calculatedChecksum % 256;
	char check[4];
	sprintf_s(check, "%X", calculatedChecksum);
	if (string(check).length() == 1)
 	{
		return "0" + string(check);
	}
	return string(check);
}

/**
<summary>
Ensure that the check (2-digit hex symbol) is correct for the message
issued, as specified in the CTB documentation.
</summary>
<returns>Whether the checksum in message is corerct</returns>
<param name = "message">Message for which to check the given checksum</param>
*/
char TCPConnection::checkSum(string message)
{
	int index = message.find_first_of(CR);
	int calculatedChecksum = 0;
	for (int i = 1; i < index + 2; i++)
	{
		calculatedChecksum += (int)message[i];
	}
	calculatedChecksum = calculatedChecksum % 256;
	int receivedChecksum = stoi(message.substr(index + 2, index + 4), 0, 16);
	if (receivedChecksum == calculatedChecksum)
	{
		return ACK;
	}
	else
	{
		return NAK;
	}

}

/**
<summary>
Method to test Server's ability to broadcast its database of patient and assay information
to 16 clients simultaneously, with the formatted messages.
</summary>
<param name = "messager">Provide the socket for which broadcast can occur</param>
<return> Returns 0 when server done sending messages or -1 if error encountered </return>
*/

DWORD TCPConnection::sendBroadcast(SOCKET messager)
{
	int lineNumber = 1;
	/* Number of ID's to broadcast: can be configured */
	const int numIDs = 50000;
	//DWORD startTime = GetTickCount();
	DWORD startTime = (DWORD) GetTickCount64();
	for (int i = 0; i < numIDs; i++)
	{
		string ID = "SAMPLE" + to_string(i);

		/* Create and send patient info messages */
		string patientMessage = makeFormattedMessage(lineNumber, _broadcastPatientMap.at(ID).makePatientMessage());
		int bytesSent = sendMessage(messager, patientMessage);
		if (bytesSent <= 0)
		{
			closesocket(messager);
			return -1;
		}
		int bytesReceived = receiveMessage(messager);
		if (bytesReceived <= 0)
		{
			closesocket(messager);
			return -1;
		}
		lineNumber++;
		lineNumber = lineNumber % 8;

		/* Create and send assay messages */
		string assayMessage = makeFormattedMessage(lineNumber, _broadcastAssayMap.at(ID)[0].getSampleMessage());
		bytesSent = sendMessage(messager, assayMessage);
		if (bytesSent <= 0)
		{
			closesocket(messager);
			return -1;
		}
		bytesReceived = receiveMessage(messager);
		if (bytesReceived <= 0)
		{
			closesocket(messager);
			return -1;
		}
		lineNumber++;
		lineNumber = lineNumber % 8;
	}
	//cout << "Time: " << (GetTickCount() - startTime) / 1000.0 << " seconds" << endl;
	cout << "Time: " << (GetTickCount64() - startTime) / 1000.0 << " seconds" << endl;
	
	// Uncomment lines below to see message log
	/*string log = getSocketLog(messager);
	cout << "\n\nMESSAGE LOG\n\n" << endl;
	cout << log;*/
	closesocket(messager);
	return 0;
}

/**
<summary>
Method to test the client's ability to receive the server's broadcasts.
</summary>
<param name = "messager">Provide the socket for which receiving broadcast can occur</param>
*/
void TCPConnection::receiveBroadcast(SOCKET messager)
{
	while (receiveMessage(messager) > 0 && sendACK(messager) > 0);
	string display;
	cin >> display;
}