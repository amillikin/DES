// DES.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <climits>
using namespace std;
int keyType;
typedef unsigned long long BIG;

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
BIG PC_1(BIG bitString) {
	return 1;
}
int main()
{
	string strIn, cmd, action, key, mode, inFilename, outFilename;
	streampos begF, endF, size;
	BIG x;
	ifstream inFile;
	ofstream outFile;
	bool validEntry = false;
	cout << "Welcome to Aaron's DES Encrypter/Decrypter!" << endl;
	/*while (!validEntry) {
	LoopAgain:
		cout << "Accepted input: DES <-action> <key> <mode> <infile> <outfile>" << endl;
		getline(cin, strIn);
		cmd = strIn.substr(0, 4);
		if (validCmd(cmd)) {
			strIn = strIn.substr(4, strIn.length());
			action = upCase(strIn.substr(0, 3));
			if (validAction(action)) {
				action = action.substr(1, 1);
				strIn = strIn.substr(3, strIn.length());
				keyType = getKeyType(strIn);
				if (keyType == 0) {
					goto LoopAgain;
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
					mode = upCase(strIn.substr(0, 4));
					strIn = strIn.substr(4, strIn.length());
					if (validMode(mode)) {
						mode = mode.substr(0, 3);
						//inFilename = strIn.substr(0, strIn.find_first_of(" "));
						//inFilename = "C:/Users/amillikin/Documents/Visual Studio 2015/Projects/DES/Debug/DEStest.txt";
						inFilename = "C:\\Users\\amillikin\\Desktop\\Spring 17\\EECS 4980 - Inside Cryptography\\Project 1\\Shakespeare.txt";
						inFile.open(inFilename, inFile.binary);
						if (!inFile) {
							cout << "Can't open input file " << inFilename << endl;
							goto LoopAgain;
						}
						strIn = strIn.substr(strIn.find_first_of(" "), strIn.length());
						//outFilename = strIn.substr(1, strIn.find_first_of(" "));
						outFilename = "C:/Users/amillikin/Documents/Visual Studio 2015/Projects/DES/Debug/DESout.txt";
						outFile.open(outFilename, outFile.app);
						if (!outFile) {
							cout << "Can't open output file " << outFilename << endl;
							goto LoopAgain;
						}
						inFile.seekg(0, inFile.beg);
						begF = inFile.tellg();
						inFile.seekg(0, inFile.end);
						endF = inFile.tellg();
						fileSize = endF - begF;
						if (fileSize > 2147483648) {
							cout << "File too large." << endl;
							goto LoopAgain;
						}
						else if (fileSize == 0) {
							cout << "Nothing to encrypt/decrypt." << endl;
							goto LoopAgain;
						}
						else {
							validEntry = true;
						}
					}
				}
			}

		}


	}*/
	inFilename = "C:\\Users\\amillikin\\Desktop\\Spring 17\\EECS 4980 - Inside Cryptography\\Project 1\\Shakespeare.txt";
	inFile.open(inFilename, inFile.binary);
	begF = inFile.tellg();
	inFile.seekg(0, inFile.end);
	endF = inFile.tellg();
	size = endF - begF;
	cout << size << endl;
	inFile.seekg(0, inFile.beg);
	for (BIG buffer; inFile.read(reinterpret_cast<char*>(&buffer), sizeof buffer);) {
		cout << inFile.gcount() << endl;
		if (inFile.gcount() < 64) {
			cout << inFile.gcount() << endl;
		}
		cout << setw(sizeof buffer * CHAR_BIT) << buffer << endl;
	}
	return 0;
}