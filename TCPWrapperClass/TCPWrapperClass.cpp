#include "pch.h"

#include "TCPWrapperClass.h"

using namespace System::Collections::Generic;

/**
<summary>
Wrapper method to initialize instance of TCPConnection from a C# application.
</summary>
<param name = "isServer">True if server, false if client</param>
<param name = "sendDelay">How long to delay sending a message</param>
<param name = "msg1">First default message</param>
<param name = "msg2">Second default message</param>
<returns></returns>
*/
TCPWrapperClass::TCPWrapperClass(bool isServer, double sendDelay, String^ msg1, String^ msg2)
	: ManagedObject(new TCPConnection(isServer, sendDelay, toCPPString(msg1), toCPPString(msg2)))
{
	m_Instance->printTime();
	Console::WriteLine(" Success! Created wrapper object");
}

/**
<summary>
Utility method for wrapper class to print the time.
</summary>
<returns></returns>
*/
void TCPWrapperClass::printTime()
{
	m_Instance->printTime();
}

/**
<summary>
Utility method for wrapper class to print a char.
</summary>
<param name = "c">Char to be printed</param>
<returns></returns>
*/
void TCPWrapperClass::printChar(char c)
{
	TCPConnection::printChar(c);
}

/**
<summary>
Utility method for wrapper class to print a message.
</summary>
<param name = "message">Message to be printed</param>
<returns></returns>
*/
void TCPWrapperClass::printMessage(String^ message)
{
	TCPConnection::printMessage(toCPPString(message));
}

/**
<summary>
Wrapper class method to start the server for a given IP Address and port #.
</summary>
<param name = "ipAddress">IP Address</param>
<param name = "port">Port # on which to listen</param>
<returns></returns>
*/
void TCPWrapperClass::listen(String^ ipAddress, int port)
{
	Console::WriteLine("Initiating the server socket");
	m_Instance->startServer(toCPPString(ipAddress), port);
	Console::WriteLine("Listening after?");
}

/**
<summary>
Wrapper class method to close connection with a given client.
</summary>
<param name = "handle">Client for which to close connection</param>
<returns></returns>
*/
void TCPWrapperClass::close(int handle)
{
	m_Instance->closeConnection(handle);
}

/**
<summary>
Wrapper class method to add a sample order to the patient and assay vector database.
</summary>
<param name = "sampleID">Sample ID for the database</param>
<param name = "patient">Patient to add to the patient database</param>
<param name = "assays">Assay vector to add to the assay database</param>
<returns></returns>
*/
void TCPWrapperClass::addSampleOrder(String^ sampleID, PatientInformation patient, vector<Assay> assays)
{
	/*
	Design Note: assays would be instantiated in C# as a list
	- Could also just pass arguments as a string for patient,
	and a collection of strings for list of assays
	*/
	m_Instance->addToDatabase(toCPPString(sampleID), patient, assays);
}

/**
<summary>
Wrapper class method to delete a sample order from the patient and assay vector database.
</summary>
<param name = "sampleID">Sample ID for which to remove from the databases</param>
<returns></returns>
*/
void TCPWrapperClass::deleteSampleOrder(String^ sampleID)
{
	m_Instance->deleteFromDatabase(toCPPString(sampleID));
}

/**
<summary>
Wrapper class method to set a maximum number of entries for the database.
</summary>
<param name = "size">Maximum number of entries for the databases</param>
<returns></returns>
*/
void TCPWrapperClass::setMaximumDatabaseSize(int size)
{
	m_Instance->setMaxDatabaseSize(size);
}

/**
<summary>
Wrapper class method to set HeaderFrom for a given client.
</summary>
<param name = "handle">Client to set HeaderFrom for</param>
<param name = "headerFrom">New fromHeader entry</param>
<returns></returns>
*/
void TCPWrapperClass::setHeaderFrom(int handle, String^ headerFrom)
{
	m_Instance->setSocketFromHeader(handle, toCPPString(headerFrom));
}

/**
<summary>
Wrapper class method to set HeaderTo for a given client.
</summary>
<param name = "handle">Client to set HeaderTo for</param>
<param name = "headerTo">New toHeader entry</param>
<returns></returns>
*/
void TCPWrapperClass::setHeaderTo(int handle, String^ headerTo)
{
	m_Instance->setSocketToHeader(handle, toCPPString(headerTo));
}

/**
<summary>
Wrapper class method to set both HeaderFrom and HeaderTo.
</summary>
<param name = "handle">Client to set HeaderFrom for</param>
<param name = "toHeader">New toHeader entry</param>
<param name = "fromHeader">New fromHeader entry</param>
<returns></returns>
*/
void TCPWrapperClass::setHeaders(int handle, String^ toHeader, String^ fromHeader)
{
	setHeaderFrom(handle, fromHeader);
	setHeaderTo(handle, toHeader);
}

/**
<summary>
Wrapper class method to set the delimiters: field, component, repeat, escape for a given client.
</summary>
<param name = "handle">Client to set HeaderFrom for</param>
<param name = "fieldD">Field delimiter</param>
<param name = "componentD">Component delimiter</param>
<param name = "repeatD">Repeat delimiter</param>
<param name = "escapeD">Escape delimiter</param>
<returns></returns>
*/
void TCPWrapperClass::setDelimiters(int handle, String^ fieldD, String^ componentD, String^ repeatD, String^ escapeD)
{
	m_Instance->setSocketDelimiters(handle, toCPPString(fieldD) + toCPPString(componentD) +
		toCPPString(repeatD) + toCPPString(escapeD));
}

/**
<summary>
Wrapper class method to set auto-order assays list.
</summary>
<param name = "handle">Client to set auto-order assays for</param>
<param name = "aVector">Vector of assays to send as a random order</param>
<returns></returns>
*/
void TCPWrapperClass::setAutoOrder(int handle, vector<Assay> aVector)
{
	/*
	Design Note: aVector should be a list of Assay in reality to be instantiated from C#.
	- Perhaps can pass strings to create a vector of assays for the actual argument
	*/
	m_Instance->setSocketAutoOrders(handle, aVector);
}

/**
<summary>
Wrapper class method to set line bid information: priority, number of line bids, and timeout.
< / summary>
<param name = "handle">Client to set HeaderFrom for</param>
<param name = "priority">Who gets priority for contention</param>
<param name = "numLineBids">Max # of line bids before returning to idle</param>
<param name = "msTimeout">Milliseconds to wait before timeout</param>
<returns></returns>
*/
void TCPWrapperClass::setLineBids(int handle, int priority, int numLineBids, int msTimeout)
{
	/* Design Note: priority might be passed as a string 0: LIS, 1: Panther */
	m_Instance->setSocketBidPriority(handle, priority);
	m_Instance->setSocketNumLineBids(handle, priority);
	m_Instance->setSocketTimeout(handle, msTimeout);
}