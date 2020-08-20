#include "PatientInformationV5.h"

/** <summary>Return a formatted patient instance message.</summary> */
string PatientInformationV5::makePatientMessage()
{
	string output = "P|" + getSequenceNum() + "|" + getPatientID() + "|||" + getPatientName()
		+ "||" + getBirthDate() + "^^Year|" + getGender() + "|||||" + getPhysician() + "|||||||||||||";
	return output;
}