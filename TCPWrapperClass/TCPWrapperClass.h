/*
TCPWrapperClass: Class that uses the ManagedObject class that can be used to create an instance of TCPConnection
from C# applications (and calling the associated methods of TCPConnection).
*/

#include "TCPConnection.h"
#include "ManagedObject.h"
#pragma once

using namespace System;

public ref class TCPWrapperClass :  public ManagedObject<TCPConnection>
{
public:
	/* Initialize the Wrapper Object */
	TCPWrapperClass(bool isServer, double sendDelay, String^ msg1, String^ msg2);


	/* Other utility and called methods placed here */
	void printTime();
	void printChar(char c);
	void printMessage(String^ msg);

	void listen(String^ ipAddress, int port);
	void close(int handle);
	void addSampleOrder(String^ sampleID, PatientInformation patient, vector<Assay> assays);
	void deleteSampleOrder(String^ sampleID);
	void setMaximumDatabaseSize(int size);
	void setHeaderFrom(int handle, String^ headerFrom);
	void setHeaderTo(int handle, String^ headerTo);
	void setHeaders(int handle, String^ fromHeader, String^ toHeader);
	void setDelimiters(int handle, String^ fieldD, String^ componentD, String^ repeatD, String^ escapeD);
	void setAutoOrder(int handle, vector<Assay> assays);
	void setLineBids(int handle, int priority, int numLineBids, int msTimeout);
};