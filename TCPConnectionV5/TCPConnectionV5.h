/*
TCPConnectionV5: Contains methods/variables needed to initiate a client and server. Builds
off of AssayV5, HeaderV5, and PatientInformationV5 instances.
*/

#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <map>
#include <tchar.h>
#include "AssayV5.h"
#include "HeaderV5.h"
#include "PatientInformationV5.h"

#ifdef NATIVE_CODE
	#include <mutex>
#endif
//#include <mutex>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_LENGTH 2048
#define DEFAULT_PORT "27015"
#define MAX_CLIENTS 50

using namespace std;
using namespace std::chrono;

/*
Table containing the SOCKET, connected status,
receiveACK, and sentACK
*/
struct clients_b {
	bool connected;
	SOCKET client;
	bool sentACK;
	bool receiveACK;
};


class TCPConnectionV5
{
public:
	/* Sample ID's */
	string s1 = "SAMPLE01";
	string s2 = "SAMPLE02";
	string s3 = "SAMPLE03";
	string s4 = "SAMPLE04";
	string s5 = "SAMPLE05";

	/* Database of Patients */
	PatientInformationV5 p1{ "PatID01", "1", "Meier^Anna^", "19741001", "F", "Martinez" };
	PatientInformationV5 p2{ "PatID02", "2", "Neier^Banna^", "19741002", "M", "Nartinez" };
	PatientInformationV5 p3{ "PatID03", "3", "Oeier^Canna^", "19741003", "F", "Oartinez" };
	PatientInformationV5 p4{ "PatID04", "4", "Peier^Danna^", "19741004", "M", "Partinez" };
	PatientInformationV5 p5{ "PatID05", "5", "Qeier^Eanna^", "19741005", "F", "Qartinez" };

	/* Database of Assays */
	AssayV5 a1{ "SAMPLE01", "1", "CT/GC", "", "1", "1", "R", "19980506090909", "N", "Serum", "O" };
	AssayV5 a1_1{ "SAMPLE01", "1", "CT/GC", "CT", "1", "1", "R", "19980506090909", "N", "Serum", "O" };
	AssayV5 a2{ "SAMPLE02", "2", "BV", "", "1", "1", "R", "19980506090909", "N", "Serum", "O" };
	AssayV5 a2_1{ "SAMPLE02", "2", "CV/TV", "", "1", "1", "R", "19980506090909", "N", "Serum", "O" };
	AssayV5 a2_2{ "SAMPLE02", "2", "CV/TV", "CV", "1", "1", "R", "19980506090909", "N", "Serum", "O" };
	AssayV5 a3{ "SAMPLE03", "3","AdV/hMPV/RV", "", "1", "1", "R", "19980506090909", "N", "Serum", "O" };
	AssayV5 a3_1{ "SAMPLE03", "3","AdV/hMPV/RV", "RV", "1", "1", "R", "19980506090909", "N", "Serum", "O" };
	AssayV5 a4{ "SAMPLE04", "4","dHIV", "HIV", "1", "1", "R", "19980506090909", "N", "Serum", "O" };
	AssayV5 a4_1{ "SAMPLE04", "4","dpHIV", "HIV", "1", "1", "R", "19980506090909", "N", "Serum", "O" };
	AssayV5 a5{ "SAMPLE05", "5","Paraflu", "", "1", "1", "R", "19980506090909", "N", "Serum", "O" };

	/* Vector of assay collections */
	vector<AssayV5> v1{ a1, a1_1 };
	vector<AssayV5> v2{ a2, a2_1, a2_2 };
	vector<AssayV5> v3{ a3, a3_1 };
	vector<AssayV5> v4{ a4, a4_1 };
	vector<AssayV5> v5{ a5 };
	/* Mapping of sampleID's --> patientInfo's */
	map<string, PatientInformationV5> patientMap = { {s1, p1}, {s2, p2}, {s3, p3}, {s4, p4}, {s5, p5} };


	/* Mapping of sampleID's --> Vector of Assays */
	map<string, vector<AssayV5>> assayMap = { {s1, v1}, {s2, v2}, {s3, v3}, {s4, v4}, {s5, v5} };

	/* Maps of patients/assays for testing broadcast*/
	map<string, PatientInformationV5> _broadcastPatientMap;
	map<string, vector<AssayV5>> _broadcastAssayMap;

	/* Variables: Helpful string representations of EOT, ENQ, ACK */
	string sEOT = string(1, char(4));
	string sENQ = string(1, char(5));
	string sACK = string(1, char(6));
	string sSTX = string(1, char(2));
	string sLF = string(1, char(10));
	string sCR = string(1, char(13));
	string sETX = string(1, char(3));
	const char NUL = char(0), SOH = char(1), STX = char(2), ETX = char(3), LF = char(10),
		EOT = char(4), ENQ = char(5), ACK = char(6), NAK = char(21), CR = char(13),
		ETB = char(27);

	/* Initialization Methods */
	void initWinsock(string ipAddress);
	int findClientMessager();
	void startClient();
	void startServer(string ipAddress, int port);
	void makeBroadcastDatabase(const int numIDs);

	/* Constructor */
	/**
	<summary>
	Initializes variables based on isServer/sendDelay.
	</summary>
	<returns></returns>
	<param name = "isServer">True if server, false if client</param>
	<param name = "sendDelay">How long to delay sending a message</param>
	<param name = "msg1">First default message</param>
	<param name = "msg2">Second default message</param>
	*/
	TCPConnectionV5(bool isServer, double sendDelay, string msg1, string msg2)
	{
		setIsServer(isServer);
		setSendDelay(sendDelay);
		setSender(!isServer);
		setMessage1(msg1);
		setMessage2(msg2);
	}


	/* Printing Information */
	void printTime();
	static void printChar(char c);
	static void printMessage(string msg);
	string getTime();

	/* Accessor Methods */

	/** <summary>Return  whether the most recent message sent by a client/server was ACK.</summary> */
	bool getSentACK() { return _sentACK; }
	/** <summary>Return  whether the most recent message received by a client/server was ACK.</summary> */
	bool getReceiveACK() { return _receiveACK; }
	/** <summary>Return whether the most recent operation by a client/server was successful.</summary> */
	int getWorks() { return _works; }
	/** <summary>Return whether second send/receive by a client/server was successful.</summary> */
	int getWorks2() { return _works2; }
	/** <summary>Return the number of messages sent.</summary> */
	int getSendCounter() { return _sendCounter; }
	/** <summary>Return delay to send a message.</summary> */
	double getSendDelay() { return _sendDelay; }
	/** <summary>Return first default message.</summary> */
	string getMessage1() { return _msg1; }
	/** <summary>Return delay to send a message.</summary> */
	string getMessage2() { return _msg2; }
	/** <summary>Return the received message.</summary> */
	string getRMessage() { return _rMessage; }
	/** <summary>Return the from header field.</summary> */
	string getFromHeader() { return _fromHeader; }
	/** <summary>Return the to header field.</summary> */
	string getToHeader() { return _toHeader; }
	/** <summary>Return the delimiters.</summary> */
	string getDelimiters() { return _delimiters; }
	/** <summary>Return the SOCKET table for given client/server.</summary> */
	clients_b* getClients() { return _clients; }
	/** <summary>Return SOCKET info given position in table.</summary> */
	clients_b getOneClient(int x) { return _clients[x]; }
	/** <summary>Return SOCKET ID for given table entry.</summary> */
	SOCKET getOneClientClient(int x) { return _clients[x].client; }
	/** <summary>Return whether given socket in table is connected.</summary >*/
	bool getClientConnected(int x) { return _clients[x].connected; }
	/** <summary>Return whether given socket most recent message sent was ACK.</summary> */
	bool getClientSendACK(int x) { return _clients[x].sentACK; }
	/** <summary>Return whether given socket most recent message received was ACK.</summary> */
	bool getClientReceiveACK(int x) { return _clients[x].receiveACK; }
	/** <summary>Return client/server's messager socket.</summary> */
	SOCKET getMessager() { return _messager; }
	/** <summary>Return server's listener socket.</summary> */
	SOCKET getListener() { return _listener; }
	/** <summary>Return timing for most recent event for given server/client</summary> */
	DWORD getLastEvent() { return _lastEvent; }
	/** <summary>Return socket address info for the server</summary> */
	sockaddr_in getServer() { return _server; }
	/** <summary>Return number of clients server was introduced to.</summary> */
	int getNumClients() { return _numClients; }
	/** <summary>Return number of clients server is currently connected to.</summary> */
	int getNumClientsConnected() { return _numClientsConnected; }
	/** <summary>Return maximum database size.</summary> */
	int getMaxDatabaseSize() { return _maxDatabaseSize; }
	/** <summary>Return number of host queries server should respond to for given client </summary> */
	vector<string> getSocketQueries(SOCKET s) { return _socketHostQueries.at(s); }
	/** <summary>Return the phase server is currently in with a given client.</summary> */
	int getSocketPhase(SOCKET s) { return _socketPhase[s]; }
	/** <summary>Return whether client/server is the first sender during transfer phase.</summary>*/
	bool getTransferSender(SOCKET s) { return _socketTransferSender[s]; }
	/** <summary>Return whether or not server received EOT for given client </summary> */
	bool getSocketReceivedEOT(SOCKET s) { return _socketReceivedEOT[s]; }
	/** <summary>Return whether or not server received ENQ for given client </summary> */
	bool getSocketReceivedENQ(SOCKET s) { return _socketReceivedENQ[s]; }
	/** <summary>Return log for a given client</summary> */
	string getSocketLog(SOCKET s) { return _socketLog[s]; }
	/** <summary>Return log for a given client</summary> */
	string getStartupMessage() { return _startupMessage; }
	/** <summary>Get server's toHeader for the given client.</summary>*/
	string getSocketToHeader(SOCKET s) { return _socketToHeaders[s]; }
	/** <summary>Get server's fromHeader for the given client.</summary>*/
	string getSocketFromHeader(SOCKET s) { return _socketFromHeaders[s]; }
	/** <summary>Get server's delimiters for the given client.</summary>*/
	string getSocketDelimiters(SOCKET s) { return _socketDelimiters[s]; }
	/** <summary>Get server's bid priority for the given client.</summary>*/
	int getSocketBidPriority(SOCKET s) { return _socketBidPriority[s]; }
	/** <summary>Get server's line bids for the given client.</summary>*/
	int getSocketNumLineBids(SOCKET s) { return _socketNumLineBids[s]; }
	/** <summary>Get server's timeout for the given client.</summary>*/
	int getSocketTimeout(SOCKET s) { return _socketTimeout[s]; }
	/** <summary>Get server's auto orders for the given client.</summary>*/
	vector<AssayV5> getSocketAutoOrders(SOCKET s) { return _socketAutoOrders[s]; }


	/* Setter Methods */
	/** <summary>Set the messager socket for a given client/server.</summary>*/
	void setMessager(SOCKET messager) { _messager = messager; }
	/** <summary>Set whether a client/server was most recently a sender or a receiver.</summary>*/
	void setSender(bool sender) { _sender = sender; }
	/** <summary>Set delay to send a message.</summary>*/
	void setSendDelay(double sendDelay) { _sendDelay = sendDelay; }
	/** <summary>Set a default for the first message.</summary>*/
	void setMessage1(string msg1) { _msg1 = msg1; }
	/** <summary>Set a default for the second message.</summary>*/
	void setMessage2(string msg2) { _msg2 = msg2; }
	/** <summary>Set receive message.</summary>*/
	void setRMessage(string msg) { _rMessage = msg; }
	/** <summary>Change the most recent event that occurred.</summary>*/
	void setLastEvent(DWORD event) { _lastEvent = event; }
	/** <summary>Set whether or not given connection is server.</summary>*/
	void setIsServer(bool b) { _isServer = b; }
	/** <summary>Set the maximum database size.</summary>*/
	void setMaxDatabaseSize(int size) { _maxDatabaseSize = size; }
	/** <summary>Set from header.</summary>*/
	void setFromHeader(string header) { _fromHeader = header; }
	/** <summary>Set to header.</summary>*/
	void setToHeader(string header) { _toHeader = header; }
	/** <summary>Set the delimiters: field, component, repeat, escape delimiters.</summary>*/
	void setDelimiters(string f, string c, string r, string e) { _delimiters = f + c + r + e; }
	/** <summary>Set a socket in the _clients table.</summary>*/
	void setOneClientClient(int x, SOCKET messager) { _clients[x].client = messager; }
	/** <summary>Set a sendACK status for a given socket in the _clients table.</summary>*/
	void setOneClientSendACK(int x, bool s) { _clients[x].sentACK = s; }
	/** <summary>Set a connected status for a given socket in the _clients table.</summary>*/
	void setOneClientConnected(int x, bool c) { _clients[x].connected = c; }
	/** <summary>Set a receiveACK status for a given socket in the _clients table.</summary>*/
	void setOneClientReceiveACK(int x, bool r) { _clients[x].receiveACK = r; }
	/** <summary>Decrease # clients connected for a given server by 1.</summary>*/
	void decrementClientsConnected() { _numClientsConnected--; }
	/** <summary>Add to host queries to respond to for a given client.</summary>*/
	void addHostQuery(SOCKET s, string query) { _socketHostQueries[s].push_back(query); }
	/** <summary>Set the phase server is currently in with a given client.</summary> */
	void setSocketPhase(SOCKET s, int phase) { _socketPhase[s] = phase; }
	/** <summary>Set whether client/server is the first sender during transfer phase.</summary>*/
	void setTransferSender(SOCKET s, bool ts) { _socketTransferSender[s] = ts; }
	/** <summary>Set whether server received EOT for the given client.</summary>*/
	void setSocketReceivedEOT(SOCKET s, bool t) { _socketReceivedEOT[s] = t; }
	/** <summary>Set whether server received ENQ for the given client.</summary>*/
	void setSocketReceivedENQ(SOCKET s, bool t) { _socketReceivedENQ[s] = t; }
	/** <summary>Set server's toHeader for the given client.</summary>*/
	void setSocketToHeader(SOCKET s, string toHead) { _socketToHeaders[s] = toHead; }
	/** <summary>Set server's fromHeader for the given client.</summary>*/
	void setSocketFromHeader(SOCKET s, string fromHead) { _socketFromHeaders[s] = fromHead; }
	/** <summary>Set server's delimiters for the given client.</summary>*/
	void setSocketDelimiters(SOCKET s, string delim) { _socketDelimiters[s] = delim; }
	/** <summary>Set auto orders for a given client.</summary>*/
	void setSocketAutoOrders(SOCKET s, vector<AssayV5> autoOrders) { _socketAutoOrders[s] = autoOrders; }
	/** <summary>Set server's bid priority for the given client.</summary>*/
	void setSocketBidPriority(SOCKET s, int priority) { _socketBidPriority[s] = priority; }
	/** <summary>Set server's line bids for the given client.</summary>*/
	void setSocketNumLineBids(SOCKET s, int lineBids) { _socketNumLineBids[s] = lineBids; }
	/** <summary>Set server's timeout for the given client.</summary>*/
	void setSocketTimeout(SOCKET s, int timeout) { _socketTimeout[s] = timeout; }
	/** <summary>Add message server sent/received for the given client.</summary>*/
	void addSocketLog(SOCKET s, string msg) { _socketLog[s] += msg; }
	/** <summary>Initiate startup message.</summary>*/
	void makeStartupMessage(string msg) { _startupMessage = msg; }
	/** <summary>Add patient/assay vector to patient and assay databases.</summary>*/
	void addToDatabase(string ID, PatientInformationV5 patient, vector<AssayV5> aVector);
	/** <summary>Delete patient/assay vector from the databases.</summary>*/
	void deleteFromDatabase(string ID);


	/* Send Messages */
	int sendMessage(SOCKET messager, string msg);
	int sendEOT(SOCKET messager);
	int sendENQ(SOCKET messager);
	int sendACK(SOCKET messager);

	/* Receive Messages */
	int receiveMessage(SOCKET messager);

	/* Server's send & receive methods */
	void setMessagerDefaults(SOCKET messager);
	DWORD exchange(SOCKET messager);
	DWORD senderExchange(SOCKET messager);
	DWORD receiverExchange(SOCKET messager);
	DWORD serverIdle(SOCKET messager);

	/* Other Methods */
	static void timeDelay(double timeDelay);
	double makeTimeReceived(SOCKET s);
	void makeClient(int x, SOCKET s);
	string generateChecksum(string message);
	char checkSum(string message);

	/* Shutdown & Cleanup */
	void closeConnection(SOCKET messager);
	int clientShutdown();

	/* Messaging Protocols */
	vector<string> getLISMessage(string sampleID);
	vector<string> parsePantherMessage(SOCKET messager, string inp);
	vector<string> parseQuery(string compressedString);
	string parseFormatting(string inp);
	string makeFormattedMessage(int lineNum, string inp);


	/* Methods to test the broadcast */
	DWORD sendBroadcast(SOCKET messager);
	void receiveBroadcast(SOCKET messager);


private:
	clients_b _clients[MAX_CLIENTS];
	WSADATA _wsaData; // Structure contains info about windows socket implmenetation
	int _works = 0, _works2 = 0;
	int _numClientsConnected = 0, _numClients = 0;
	struct addrinfo* _result = NULL, * _ptr = NULL, _hints; // Contains sockaddr structure
	bool _isServer = false, _sender = true, _sentACK = false, _receiveACK = false;
	int _sendCounter = 0, _receiveCounter = 0;
	bool _oneReceive = true, _oneSend = true;
	bool _transferSender = false;
	string _msg1, _msg2;
	double _sendTime = 0, _receiveTime = 0;
	double _sendDelay;
	sockaddr_in _server;
	DWORD _threadID;
	map<SOCKET, int> _socketPhase; // 0: Resting, 1: Establishment, 2: Transfer, 3: Termination
	map<SOCKET, vector<string>> _socketHostQueries;
	map<SOCKET, DWORD> _socketTimers;
	map<SOCKET, bool> _socketTransferSender;
	map<SOCKET, bool> _socketReceivedEOT;
	map<SOCKET, bool> _socketReceivedENQ; // Used to determine state of contention
	SOCKET _messager, _listener;
	string _rMessage, _sMessage;
	DWORD _lastEvent = GetTickCount();
	map<SOCKET, string> _socketLog;
	string _startupMessage;
	/* Default toHeader, fromHeader, delimiters, and autoOrder */
	string _toHeader = "Panther", _fromHeader = "LISHost", _delimiters = "|\\^&";
	vector<AssayV5> _autoOrders = v1;
	/* Default bid priority, number of lineBids, timeout*/
	int _bidPriority = 0, _numLineBids = 7, _msTimeout = 0;
	map<SOCKET, string> _socketToHeaders;
	map<SOCKET, string> _socketFromHeaders;
	map<SOCKET, string> _socketDelimiters;
	map<SOCKET, vector<AssayV5>> _socketAutoOrders;
	map<SOCKET, int> _socketBidPriority;
	map<SOCKET, int> _socketNumLineBids;
	map<SOCKET, int> _socketTimeout;
	int _maxDatabaseSize = INT_MAX;
	#ifdef NATIVE_CODE
		mutex locker;
	#endif
};