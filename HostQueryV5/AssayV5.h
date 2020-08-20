/*
AssayV5: class to represent an instance of an assay (including their relevant,
non-redacted information) to be added to a map of sample ID --> assay vectors.
*/

#pragma once
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>


using namespace std;

class AssayV5
{
public:
	/* Constructor */

	/**
	<summary>
	Constructor for an Assay from the database based on assay parameters
	</summary>
	<param name = "id">Sample ID</param>
	<param name = "sequenceNum"> Sample Sequence Number</param>
	<param name = "assayName">Assay Name</param>
	<param name = "analyteName">Analyte Name</param>
	<param name = "num">Number of replicates</param>
	<param name = "factor">Dilution factor</param>
	<param name = "priority">Priority</param>
	<param name = "dateTime">Date Time Request Issued</param>
	<param name = "code">Action Code</param>
	<param name = "specimenType">Specimen Type</param>
	<param name = "reportType">Report Type</param>

	*/
	AssayV5(string id, string sequenceNum, string assayName, string analyteName, string num,
		string factor, string priority, string dateTime, string code, string specimenType,
		string reportType)
	{
		setSampleID(id);
		setSequenceNum(sequenceNum);
		setAssayName(assayName);
		setAnalyteName(analyteName);
		setNumReplicates(num);
		setDilutionFactor(factor);
		setPriority(priority);
		setDateTimeRequested(dateTime);
		setActionCode(code);
		setSpecimenType(specimenType);
		setReportType(reportType);
	}

	/** <summary>Default constructor for Assay </summary> */
	AssayV5();

	/* Methods to make outgoing message */
	string getAssayFormattedName();
	string getSampleMessage();

	/* Accessor Methods */
	/** <summary>Return sample ID </summary> */
	string getSampleID() { return _sampleID; }
	/** <summary>Return sequence number</summary> */
	string getSequenceNum() { return _sequenceNum; }
	/** <summary>Return the assay name </summary> */
	string getAssayName() { return _assayName; }
	/** <summary>Return the analyte name (empty string means all) </summary> */
	string getAnalyteName() { return _analyteName; }
	/** <summary>Return number of replicates (empty string means 1)</summary> */
	string getNumReplicates() { return _numReplicates; }
	/** <summary>Return the dilution factor (empty string means 1)</summary> */
	string getDilutionFactor() { return _dilutionFactor; }
	/** <summary>Return the priority </summary> */
	string getPriority() { return _priority; }
	/** <summary>Return the date time request was issued </summary> */
	string getDateTimeRequested() { return _dateTimeRequested; }
	/** <summary>Return the action code</summary> */
	string getActionCode() { return _actionCode; }
	/** <summary>Return the specimen type</summary> */
	string getSpecimenType() { return _specimenType; }
	/** <summary>Return the report type</summary> */
	string getReportType() { return _reportType; }
	string getDateTime();

	/* Setter Methods */
	/** <summary>Set the sample ID</summary> */
	void setSampleID(string id) { _sampleID = id; }
	/** <summary>Set the sample ID</summary> */
	void setSequenceNum(string sequenceNum) { _sequenceNum = sequenceNum; }
	/** <summary>Set the assay name</summary> */
	void setAssayName(string name) { _assayName = name; }
	/** <summary>Set the analyte name</summary> */
	void setAnalyteName(string name) { _analyteName = name; }
	/** <summary>Set the number of replicates</summary> */
	void setNumReplicates(string num) { _numReplicates = num; }
	/** <summary>Set the dilution factor</summary> */
	void setDilutionFactor(string factor) { _dilutionFactor = factor; }
	/** <summary>Set the priority</summary> */
	void setPriority(string priority) { _priority = priority; }
	/** <summary>Set the date & time request was issued</summary> */
	void setDateTimeRequested(string date) { _dateTimeRequested = date; }
	/** <summary>Set the action code</summary> */
	void setActionCode(string code) { _actionCode = code; }
	/** <summary>Set the specimen type</summary> */
	void setSpecimenType(string type) { _specimenType = type; }
	/** <summary>Set the report type</summary> */
	void setReportType(string type) { _reportType = type; }

private:
	/* Relevant variables for each assay */
	string _sampleID;
	string _sequenceNum;
	string _assayName;
	string _analyteName;
	string _numReplicates;
	string _dilutionFactor;
	string _priority;
	string _dateTimeRequested;
	string _actionCode;
	string _specimenType;
	string _reportType;

};

