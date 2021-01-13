#include "Header.h"


/**
	<summary>
	Utility method to get a formatted string representation of current date/time
	as component of header response.
	</summary>
	<returns>String representation of date time</returns>
*/
string Header::getCurrDateTime()
{
	SYSTEMTIME currTime;
	GetLocalTime(&currTime);
	string yearString = to_string(currTime.wYear), monthString = to_string(currTime.wMonth), dayString = to_string(currTime.wDay),
		hourString = to_string(currTime.wHour), minuteString = to_string(currTime.wMinute), secondString = to_string(currTime.wSecond);
	if (currTime.wMonth < 10)
	{
		monthString = "0" + to_string(currTime.wMonth);
	}
	if (currTime.wDay < 10)
	{
		dayString = "0" + to_string(currTime.wDay);
	}
	if (currTime.wHour < 10)
	{
		hourString = "0" + to_string(currTime.wHour);
	}
	else if (currTime.wHour >= 22)
	{
		hourString = to_string(currTime.wHour - 12);
	}
	else if (currTime.wHour >= 13)
	{
		hourString = "0" + to_string(currTime.wHour - 12);
	}
	if (currTime.wMinute < 10)
	{
		minuteString = "0" + to_string(currTime.wMinute);
	}
	if (currTime.wSecond < 10)
	{
	    secondString = "0" + to_string(currTime.wSecond);
	}
	string output = yearString + monthString + dayString + hourString + minuteString + secondString;
	return output;
}

/**
	<summary>
	Find the nth occurrence of a character in a string;
	</summary>
	<returns>Index of the nth occurrence</returns>
	<param name = "s">string in which we're looking</param>
	<param name = "c">char to look for</param>
	<param name = "n">nth occurrence</param>
*/
int Header::findNthOccur(string s, char c, int n)
{
	int total = 0;
	for (unsigned int i = 0; i < s.length(); i++)
	{
		if (s[i] == c)
		{
			total++;
		}
		if (total == n)
		{
			return i;
		}
	}
	return -1;
}

/**<summary>Find the string delimiters from the received message.</summary>*/
string Header::getDelimiters()
{
	return _delimiters;
}

/**<summary>Find the sender of the received message.</summary>*/
string Header::getSender()
{
	return _sender;
}

/**<summary>Find the receiver of the received message.</summary>*/
string Header::getReceiver()
{
	return _receiver;
}

/**<summary>Create a formatted response based on delimiters, receiver, sender, and date/time</summary>*/
string Header::getHeaderResponse()
{
	string output = "H" + getDelimiters() + "|||"
		+ getSender() + "|||||" + getReceiver()
		+ "||P|1|" + getCurrDateTime() + "|";
	return output;
}