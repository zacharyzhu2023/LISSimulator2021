#include "PatientInformation.h"

/** <summary>Return a formatted patient instance message.</summary> */
string PatientInformation::makePatientMessage()
{
	string output = "P|" + getSequenceNum() + "|" + getPatientID() + "|||" + getPatientName()
		+ "||" + getBirthDate() + "^^Year|" + getGender() + "|||||" + getPhysician() + "|||||||||||||";
	return output;
}