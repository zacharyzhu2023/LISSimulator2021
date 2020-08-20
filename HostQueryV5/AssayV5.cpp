#include "AssayV5.h"

/**
<summary>
Utility method to get a formatted string representation of current date/time
to get date time requested for an assay.
</summary>
<returns>String representation of date time</returns>
*/
string AssayV5::getDateTime()
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
Formatted output based on assay name, analyte name, number of replicates, 
and the dilution factor
</summary>
*/
string AssayV5::getAssayFormattedName()
{
	string output = "^^^" + getAssayName() + "^" + getAnalyteName() + "^" + getNumReplicates()
		+ "^" + getDilutionFactor();
	return output;
}

/**
<summary>
Formatted overall message for each assay to send from the server.
</summary>
*/
string AssayV5::getSampleMessage()
{
	string output = "O|" + getSequenceNum() + "|" + getSampleID() + "||" + getAssayFormattedName() +
		"|" + getPriority() + "|" + getDateTime() + "|||||" + getActionCode()
		+ "||||" + getSpecimenType() + "||||||||||" + getReportType() + "|";
	return output;
}
