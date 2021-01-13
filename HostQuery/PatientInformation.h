/*
PatientInformation: class to represent an instance of a patient (including their relevant,
non-redacted information) to be added to a map of sample ID --> patient instances.
*/

#pragma once
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>


using namespace std;

class PatientInformation
{
public:
	/* Constructor */
	/**
	<summary>
	Constructor for an assay based on the patient database (mapping).
	</summary>
	<param name = "id">Patient ID</param>
	<param name = "sequenceNum">Patient Sequence Number</param>
	<param name = "name">Patient Name</param>
	<param name = "date">Patient Date of Birth</param>
	<param name = "gender">Patient Gender</param>
	<param name = "phys">Patient Physician Name</param>
	*/
	PatientInformation(string id, string sequenceNum, string name, string date, string gender, string phys)
	{
		setPatientID(id);
		setSequenceNum(sequenceNum);
		setPatientName(name);
		setBirthDate(date);
		setGender(gender);
		setPhysician(phys);
	}
	/** <summary>Default constructor for a patient</summary> */
	PatientInformation();

	/* Setter Methods */

	/** <summary>Set the patient ID.</summary> */
	void setPatientID(string ID) { _patientID = ID; }
	/** <summary>Set the sequence number.</summary> */
	void setSequenceNum(string num) { _sequenceNum = num; }
	/** <summary>Set the patient name.</summary> */
	void setPatientName(string name) { _patientName = name; }
	/** <summary>Set the patient birth date.</summary> */
	void setBirthDate(string date) { _birthDate = date; }
	/** <summary>Set the patient gender.</summary> */
	void setGender(string gender) { _gender = gender; }
	/** <summary>Set the patient physician.</summary> */
	void setPhysician(string phys) { _physician = phys; }

	/* Accessor Methods */

	/** <summary>Return the patient's ID.</summary> */
	string getPatientID() { return _patientID; }
	/** <summary>Return the patient's ID.</summary> */
	string getSequenceNum() { return _sequenceNum; }
	/** <summary>Return the patient's name</summary> */
	string getPatientName() { return _patientName; }
	/** <summary>Return the patient's ID.</summary> */
	string getBirthDate() { return _birthDate; }
	/** <summary>Return the patient's gender.</summary> */
	string getGender() { return _gender; }
	/** <summary>Return the patient's physician.</summary> */
	string getPhysician() { return _physician; }

	/* Method to get the string representation */
	string makePatientMessage();

private:
	string _patientID;
	string _sequenceNum;
	string _patientName;
	string _birthDate;
	string _gender;
	string _physician;
};

