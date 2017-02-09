// DES.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
using namespace std;
int keyType;

//Converts a string to all uppercase characters - ARM
string upCase(string str) {
	transform(str.begin(), str.end(), str.begin(), toupper);
	return str;
}
//Checks valid DES command - ARM
bool validCmd(string cmd) {
	if (cmd == "DES ") {
		return true;
	}
	else {
		cout << "Not a valid command." << endl;
		return false;
	}
}
//Checks valid action - ARM
bool validAction(string action) {
	if (action == "-E " || action == "-D ") {
		return true;
	}
	else {
		cout << "Not a valid action. Valid actions include: -E, -D" << endl;
		return false;
	}
}
//Returns type of key used: 1) Hex, 2) 8-char (no space) 3) 8-char (with spaces). Else 0 if not valid. - ARM
int getKeyType(string strIn) {
	if (strIn == "") {
		cout << "Not a valid key. Must be 16-bit hex or 8-char" << endl;
		return 0;
	}
	if (strIn.find_first_of(" ") == 16) {
		for (int i = 0; i < 16; i++) {
			if (!isxdigit(strIn[i])) {
				cout << "Not a valid key. Must be 16-bit hex or 8-char" << endl;
				return 0;
			}
		}
		return 1;
	}
	else if (strIn.find_first_of(" ") == 10 && strIn.substr(0, 1) == "'" && strIn.substr(9, 1) == "'") {
		return 2;
	}
	else if (strIn.substr(11,1) == " " && strIn.substr(0, 2) == "\"\'" && strIn.substr(10, 2) == "\'\"") {
		return 3;
	}
	else {
		cout << "Not a valid key. Must be 16-bit hex or 8-char" << endl;
		return 0;
	}
}
//Checks valid mode - ARM
bool validMode(string mode) {
	if (mode == "ECB " || mode == "CBC ") {
		return true;
	}
	else{
		cout << "Not a valid mode. Valid modes include: ECB, CBC" << endl;
		return false;
	}
}

int main()
{
	string strIn, cmd, action, key, mode, inFilename, outFilename;
	ifstream inFile;
	ofstream outFile;
	bool validEntry = false;
	cout << "Welcome to Aaron's DES Encrypter/Decrypter!" << endl;
	while (!validEntry) {
		cout << "Accepted input: DES <-action> <key> <mode> <infile> <outfile>" << endl;
		getline(cin, strIn);
		cmd = strIn.substr(0, 4);
		if (validCmd(cmd)) {
			strIn = strIn.substr(4, strIn.length());
			action = upCase(strIn.substr(0, 3));
			if (validAction(action)) {
				action = action.substr(1, 2);
				strIn = strIn.substr(3, strIn.length());
				keyType = getKeyType(strIn);
				if (keyType == 0) {
					break;
				}
				else {
					if (keyType == 1) {
						key = strIn.substr(0, 16);
						strIn = strIn.substr(17, strIn.length());
					}
					else if (keyType == 2) {
						key = strIn.substr(1, 8);
						strIn = strIn.substr(11, strIn.length());
					}
					else if (keyType == 3) {
						key = strIn.substr(2, 8);
						strIn = strIn.substr(13, strIn.length());
					}
					if (validMode(strIn)) {
						mode = strIn.substr(0, 3);
						strIn = strIn.substr(4, strIn.length());
						inFilename = strIn.substr(0, strIn.find_first_of(" "));
						inFile.open(inFilename);
						if (!inFile) {
							cout << "Can't open input file " << inFilename << endl;
							break;
						}
						strIn = strIn.substr(strIn.find_first_of(" "), strIn.length());
						outFilename = strIn.substr(1, strIn.find_first_of(" "));
						outFile.open(outFilename);
						if (!outFile) {
							cout << "Can't open output file " << outFilename << endl;
							break;
						}
						validEntry = true;
					}
				}
			}
		}
	}
	return 0;
}