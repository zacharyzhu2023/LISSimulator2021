/*
HeaderV5: class to represent an instance of a header, to send as a header message.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <Windows.h>
using namespace std;

class HeaderV5
{
public:
	/* Constructor */
	/**
	<summary>
	Constructor for header response based on header received.
	</summary>
	<returns></returns>
	<param name = "pantherHeader">Header message received</param>
	*/
	HeaderV5(string pantherHeader)
	{
		setPantherHeader(pantherHeader);
	}
	/** <summary>Default constructor for header response</summary> */
	HeaderV5() {}

	/* Other Methods */
	static int findNthOccur(string s, char c, int n);

	/* Setter Methods */
	/** <summary>Set the delimiters</summary> */
	void setDelimiters(string d) { _delimiters = d; }
	/** <summary>Set the sender</summary> */
	void setSender(string sender) { _sender = sender; }
	/** <summary>Set the receiver</summary> */
	void setReceiver(string receiver) { _receiver = receiver; }
	/** <summary>Set the received panther header message</summary> */
	void setPantherHeader(string header) { _pantherHeader = header; }

	/* Accessor Methods */
	static string getCurrDateTime();
	/** <summary>Return the header message received from Panther.</summary> */
	string getPantherMessage() { return _pantherHeader; }
	string getDelimiters();
	string getSender();
	string getReceiver();
	string getHeaderResponse();

private:
	string _pantherHeader;
	string _delimiters = "\\^&";
	string _sender = "LISHost";
	string _receiver = "Panther";
};