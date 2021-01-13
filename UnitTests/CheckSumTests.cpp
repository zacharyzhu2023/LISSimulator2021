/*
CheckSumTests: application that contains the tests for checksum and creates the checksum values
for the test strings.
*/

#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "TCPConnection.h"

using namespace std;

/* Useful chars for testing */
const char stx = char(2), etx = char(3), lf = char(10),
cr = char(13), etb = char(27), ack = char(6);

/* Test cases for checksum */

// 1. Original string: "<STX>1H|\^&|||LIS sim|||||||P|NCCLS LIS2-A|20120829151825<CR><ETX>3F<CR><LF>"
string test1 = string(1, stx) + "1H|\\^&|||LIS sim|||||||P|NCCLS LIS2-A|20120829151825"
+ string(1, cr) + string(1, etx) + "3F" + string(1, cr) + string(1, lf);

// 2. Original string: <STX>2P|1||PatientID1||Flinstone^Fred^G||020552^55^Y|M|||||Zus||9.9|32^In|122^In|||||||San Diego|<CR><ETX>D2<CR><LF>
string test2 = string(1, stx) + "2P|1||PatientID1||Flinstone^Fred^G||020552^55^Y|M|||||Zus||9.9|32^In|122^In|||||||San Diego|"
+ string(1, cr) + string(1, etx) + "D2" + string(1, cr) + string(1, lf);

// 3. <STX>1H|\^&|||Panther|||||Host||P|1|<CR><ETX>A6<CR><LF>
string test3 = string(1, stx) + "1H|\\^&|||Panther|||||Host||P|1|"
+ string(1, cr) + string(1, etx) + "A6" + string(1, cr) + string(1, lf);

// 4. <STX>0O|1|FEB20-1|f503d70f-e29a-4580-b028-870ae5221c93^123569^001283-20200220-153|^^^GI Bac 1^Salmonella Campy Shigella/EIEC Stx 1/2^^1|R|20200220105036|||||||||Specimen||||||||||F|^18991231160000<CR><ETX>3A<CR><LF>
string test4 = string(1, stx) + "0O|1|FEB20-1|f503d70f-e29a-4580-b028-870ae5221c93^123569^001283-20200220-153|^^^GI Bac 1^Salmonella Campy Shigella"
+ "/EIEC Stx 1/2^^1|R|20200220105036|||||||||Specimen||||||||||F|^18991231160000" + string(1, cr) + string(1, etx) + "3A" + string(1, cr) + string(1, lf);

// 5. <STX>3P|3||||^^|||||||||||^|^|||||||||||||||^|^|<CR><ETX>72<CR><LF>
string test5 = string(1, stx) + "3P|3||||^^|||||||||||^|^|||||||||||||||^|^|"
+ string(1, cr) + string(1, etx) + "72" + string(1, cr) + string(1, lf);

/* 6. <STX>2Q|1|^NOASSAY-000002||ALL||||||||O<CR><ETX>87<CR><LF> */
string test6 = string(1, stx) + "2Q|1|^NOASSAY-000002||ALL||||||||O"
+ string(1, cr) + string(1, etx) + "A6" + string(1, cr) + string(1, lf);

/* 7. <STX>3L|1|N<CR><ETX>06<CR><LF> */
string test7 = string(1, stx) + "3L|1|N" + string(1, cr) + string(1, etx) 
+ "06" + string(1, cr) + string(1, lf);

/* Input lines to generate checksums for from client */

string inpL1 = string(1, char(2)) + "1H|\\^&|||Panther|||||LISHost||P|1|"
+ string(1, char(13)) + string(1, char(3)) + "00" + string(1, char(13)) + string(1, char(10));
string inpL2 = string(1, char(2)) + "2Q|1|^SAMPLE01||ALL||||||||O"
+ string(1, char(13)) + string(1, char(3)) + "00" + string(1, char(13)) + string(1, char(10));
string inpL3 = string(1, char(2)) + "3Q|2|^SAMPLE02||ALL||||||||O"
+ string(1, char(13)) + string(1, char(3)) + "00" + string(1, char(13)) + string(1, char(10));
string inpL4 = string(1, char(2)) + "4Q|3|^SAMPLE03||ALL||||||||O"
+ string(1, char(13)) + string(1, char(3)) + "00" + string(1, char(13)) + string(1, char(10));
string inpL5 = string(1, char(2)) + "5Q|4|^SAMPLE04||ALL||||||||O"
+ string(1, char(13)) + string(1, char(3)) + "00" + string(1, char(13)) + string(1, char(10));
string inpL6 = string(1, char(2)) + "6Q|5|^SAMPLE05||ALL||||||||O"
+ string(1, char(13)) + string(1, char(3)) + "00" + string(1, char(13)) + string(1, char(10));
string inpL7 = string(1, char(2)) + "7L|1|N|"
+ string(1, char(13)) + string(1, char(3)) + "00" + string(1, char(13)) + string(1, char(10));
string compressedQuery = string(1, char(2)) + "2Q|1|^SAMPLE01\\^SAMPLE02\\^SAMPLE03\\^SAMPLE04\\^SAMPLE05\\^SAMPLE06||ALL||||||||O"
+ string(1, char(13)) + string(1, char(3)) + "00" + string(1, char(13)) + string(1, char(10));

/* Function to test checksum sample strings */
void testChecksum()
{
	TCPConnection UnitTest(true, 0, "", "");
	char t1 = UnitTest.checkSum(test1);
	char t2 = UnitTest.checkSum(test2);
	char t3 = UnitTest.checkSum(test3);
	char t4 = UnitTest.checkSum(test4);
	char t5 = UnitTest.checkSum(test5);
	cout << "Is ack? " << (t1 == ack) << endl;
	cout << "Is ack? " << (t2 == ack) << endl;
	cout << "Is ack? " << (t3 == ack) << endl;
	cout << "Is ack? " << (t4 == ack) << endl;
	cout << "Is ack? " << (t5 == ack) << endl;

	cout << "Checksum: " << UnitTest.generateChecksum(test6) << endl;
	cout << "Checksum: " << UnitTest.generateChecksum(test7) << endl;
}

/* Generating checksums for input lines */
void generateChecksums()
{
	TCPConnection UnitTest(true, 0, "", "");
	cout << "Generate checksums for input lines for CLIENT\n" << endl;
	cout << "Checksum: " << UnitTest.generateChecksum(inpL1) << endl;
	cout << "Checksum: " << UnitTest.generateChecksum(inpL2) << endl;
	cout << "Checksum: " << UnitTest.generateChecksum(inpL3) << endl;
	cout << "Checksum: " << UnitTest.generateChecksum(inpL4) << endl;
	cout << "Checksum: " << UnitTest.generateChecksum(inpL5) << endl;
	cout << "Checksum: " << UnitTest.generateChecksum(inpL6) << endl;
	cout << "Checksum: " << UnitTest.generateChecksum(inpL7) << endl;
	cout << "Compressed Query Checksum: " << UnitTest.generateChecksum(compressedQuery) << endl;
}

int main()
{
	testChecksum();
	generateChecksums();
}
	

