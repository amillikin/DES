/******************************************************************************************** 
*										DES.cpp 											*
*																							*
*	DESCRIPTION: A DES encrypter and descripter that accepts filesizes up to 31-bytes.		*
*				 Input Parameters: DES <-action> <key> <mode> <infile> <outfile>.			*
*				 Accepted Actions: "-E" (encrypt), "-D" (decrypt)							*
*				 Accepted Keys: 16-Bit Hex string, 8-bit Char string						*
*				 Accepted Modes: EBC (coming soon, CBC)										*
*																							*
*																							*
*	AUTHOR: Aaron Millikin											START DATE: 2/7/2017	*
*********************************************************************************************/

#include "stdafx.h"
#include <intrin.h>
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cstdio>
#include <ctime>

using namespace std;
typedef unsigned long long ull;
int keyType;
FILE *inStream, *outStream;
ull roundkey[16];

//Final Permutation for 64-bit block - ARM
ull fp(ull block) {
	/*		
	More Permutation, check bit on in block, if 1, 
		shift a 1 to its position on out Block

	Straight P-Box using the following table:
		40	8	48	16	56	24	64	32
		39	7	47	15	55	23	63	31
		38	6	46	14	54	22	62	30
		37	5	45	13	53	21	61	29
		36	4	44	12	52	20	60	28
		35	3	43	11	51	19	59	27
		34	2	42	10	50	18	58	26
		33	1	41	9	49	17	57	25
	*/
	ull pBlock = 0;
	if (block & 0x8000000000) pBlock |= 1ULL;					//	40	8	48	16	56	24	64	32
	if (block & 0x80) pBlock |= (1ULL << 1);
	if (block & 0x800000000000) pBlock |= (1ULL << 2);
	if (block & 0x8000) pBlock |= (1ULL << 3);
	if (block & 0x80000000000000) pBlock |= (1ULL << 4);
	if (block & 0x800000) pBlock |= (1ULL << 5);
	if (block & 0x8000000000000000) pBlock |= (1ULL << 6);
	if (block & 0x80000000) pBlock |= (1ULL << 7);
	if (block & 0x4000000000) pBlock |= (1ULL << 8);			//	39	7	47	15	55	23	63	31
	if (block & 0x40) pBlock |= (1ULL << 9);
	if (block & 0x400000000000) pBlock |= (1ULL << 10);
	if (block & 0x4000) pBlock |= (1ULL << 11);
	if (block & 0x40000000000000) pBlock |= (1ULL << 12);
	if (block & 0x400000) pBlock |= (1ULL << 13);
	if (block & 0x4000000000000000) pBlock |= (1ULL << 14);
	if (block & 0x40000000) pBlock |= (1ULL << 15);
	if (block & 0x2000000000) pBlock |= (1ULL << 16);			// 38	6	46	14	54	22	62	30
	if (block & 0x20) pBlock |= (1ULL << 17);
	if (block & 0x200000000000) pBlock |= (1ULL << 18);
	if (block & 0x2000) pBlock |= (1ULL << 19);
	if (block & 0x20000000000000) pBlock |= (1ULL << 20);
	if (block & 0x200000) pBlock |= (1ULL << 21);
	if (block & 0x2000000000000000) pBlock |= (1ULL << 22);
	if (block & 0x20000000) pBlock |= (1ULL << 23);
	if (block & 0x1000000000) pBlock |= (1ULL << 24);			// 37	5	45	13	53	21	61	29
	if (block & 0x10) pBlock |= (1ULL << 25);
	if (block & 0x100000000000) pBlock |= (1ULL << 26);
	if (block & 0x1000) pBlock |= (1ULL << 27);
	if (block & 0x10000000000000) pBlock |= (1ULL << 28);
	if (block & 0x100000) pBlock |= (1ULL << 29);
	if (block & 0x1000000000000000) pBlock |= (1ULL << 30);
	if (block & 0x10000000) pBlock |= (1ULL << 31);
	if (block & 0x800000000) pBlock |= (1ULL << 32);		//	36	4	44	12	52	20	60	28
	if (block & 0x8) pBlock |= (1ULL << 33);
	if (block & 0x80000000000) pBlock |= (1ULL << 34);
	if (block & 0x800) pBlock |= (1ULL << 35);
	if (block & 0x8000000000000) pBlock |= (1ULL << 36);
	if (block & 0x80000) pBlock |= (1ULL << 37);
	if (block & 0x800000000000000) pBlock |= (1ULL << 38);
	if (block & 0x8000000) pBlock |= (1ULL << 39);
	if (block & 0x400000000) pBlock |= (1ULL << 40);		//	35	3	43	11	51	19	59	27
	if (block & 0x4) pBlock |= (1ULL << 41);
	if (block & 0x40000000000) pBlock |= (1ULL << 42);
	if (block & 0x400) pBlock |= (1ULL << 43);
	if (block & 0x4000000000000) pBlock |= (1ULL << 44);
	if (block & 0x40000) pBlock |= (1ULL << 45);
	if (block & 0x400000000000000) pBlock |= (1ULL << 46);
	if (block & 0x4000000) pBlock |= (1ULL << 47);
	if (block & 0x200000000) pBlock |= (1ULL << 48);		// 34	2	42	10	50	18	58	26
	if (block & 0x20) pBlock |= (1ULL << 49);
	if (block & 0x20000000000) pBlock |= (1ULL << 50);
	if (block & 0x200) pBlock |= (1ULL << 51);
	if (block & 0x2000000000000) pBlock |= (1ULL << 52);
	if (block & 0x20000) pBlock |= (1ULL << 53);
	if (block & 0x200000000000000) pBlock |= (1ULL << 57);
	if (block & 0x2000000) pBlock |= (1ULL << 55);
	if (block & 0x100000000) pBlock |= (1ULL << 56);		// 33	1	41	9	49	17	57	25
	if (block & 0x1) pBlock |= (1ULL << 57);
	if (block & 0x10000000000) pBlock |= (1ULL << 58);
	if (block & 0x100) pBlock |= (1ULL << 59);
	if (block & 0x1000000000000) pBlock |= (1ULL << 60);
	if (block & 0x10000) pBlock |= (1ULL << 61);
	if (block & 0x100000000000000) pBlock |= (1ULL << 62);
	if (block & 0x1000000) pBlock |= (1ULL << 63);
	return pBlock;
};

//Straight Permutation Box to spread bits for following round - ARM
ull sp(ull right) {
	/*	
	More Permutation, check bit on in block, if 1, 
		shift a 1 to its position on out Block

		Straight P-Box using the following table:
			16	7	20	21	29	12	28	17
			1	15	23	26	5	18	31	10
			2	8	24	14	32	27	3	9
			19	13	30	6	22	11	4	25
	*/
	ull pRight = 0;
	if (right & 0x8000) pRight |= 1ULL;			//	16	7	20	21	29	12	28	17
	if (right & 0x40) pRight |= (1ULL << 1);
	if (right & 0x80000) pRight |= (1ULL << 2);
	if (right & 0x100000) pRight |= (1ULL << 3);
	if (right & 0x10000000) pRight |= (1ULL << 4);
	if (right & 0x800) pRight |= (1ULL << 5);
	if (right & 0x8000000) pRight |= (1ULL << 6);
	if (right & 0x10000) pRight |= (1ULL << 7);
	if (right & 0x1) pRight |= (1ULL << 8);		//	1	15	23	26	5	18	31	10
	if (right & 0x4000) pRight |= (1ULL << 9);
	if (right & 0x400000) pRight |= (1ULL << 10);
	if (right & 0x2000000) pRight |= (1ULL << 11);
	if (right & 0x10) pRight |= (1ULL << 12);
	if (right & 0x20000) pRight |= (1ULL << 13);
	if (right & 0x40000000) pRight |= (1ULL << 14);
	if (right & 0x200) pRight |= (1ULL << 15);
	if (right & 0x2) pRight |= (1ULL << 16);		//	2	8	24	14	32	27	3	9
	if (right & 0x80) pRight |= (1ULL << 17);
	if (right & 0x800000) pRight |= (1ULL << 18);
	if (right & 0x2000) pRight |= (1ULL << 19);
	if (right & 0x80000000) pRight |= (1ULL << 20);
	if (right & 0x4000000) pRight |= (1ULL << 21);
	if (right & 0x4) pRight |= (1ULL << 22);
	if (right & 0x100) pRight |= (1ULL << 23);
	if (right & 0x40000) pRight |= (1ULL << 24);	//	19	13	30	6	22	11	4	25
	if (right & 0x1000) pRight |= (1ULL << 25);
	if (right & 0x20000000) pRight |= (1ULL << 26);
	if (right & 0x20) pRight |= (1ULL << 27);
	if (right & 0x200000) pRight |= (1ULL << 28);
	if (right & 0x400) pRight |= (1ULL << 29);
	if (right & 0x8) pRight |= (1ULL << 30);
	if (right & 0x1000000) pRight |= (1ULL << 31);
	return pRight;
};

//Eight 6x4 Substitution Boxes to take 48-bits into 32-bits - ARM
ull sb(ull right) {
	/*	The 48-bits coming in are split into 8 groups of 6-bits each.
		Each group of 6-bits is passed to its respective substitution box.
		The substitution boxes are 64 character arrays comprised of
		4 rows and 16 columns. In each group, the first and last bit are used
		to determine which row to select (0-3) and the middle four bits determine
		which column to select (0-15). The intersection of row and column then selects
		a respective 4-bit output. After this process is completed for each of the 
		eight groups, the outputs are joined together to form a 32-bit output.

		Grab desired row and column bits (rccccr) from input (right)
		by ANDing with 100001 and 011110 to determine which are turned on.
		Shift this result (row/col) over to determine the lookup value
		Or and set result (sRight) with value determined from S-Box
		(subsequent rounds, shift result (sRight) left 4-bits to set desired location for new bits).
	*/	
	ull sRight = 0;
	ull row, column;

	// S-Box 1
	char sb1[64] = {
		14,	4,	13,	1,	2,	15,	11,	8,	3,	10,	6,	12,	5,	9,	0,	7,
		0,	15,	7,	4,	14,	2,	13,	1,	10,	6,	12,	11,	9,	5,	3,	8,
		4,	1,	14,	8,	13,	6,	2,	11,	15,	12,	9,	7,	3,	10,	5,	0,
		15,	12,	8,	2,	4,	9,	1,	7,	5,	11,	3,	14,	10,	0,	6,	13
	};

	row = (right & 0x840000000000) >> 42;
	row = (row >> 4) | (row & 0x1);
	column = (right & 0x780000000000) >> 43;
	sRight |= (sb1[16*row+column] & 0xF);

	// S-Box 2
	char sb2[64] = {
		15,	1,	8,	14,	6,	11,	3,	4,	9,	7,	2,	13,	12,	0,	5,	10,
		3,	13,	4,	7,	15,	2,	8,	14,	12,	0,	1,	10,	6,	9,	11,	5,
		0,	14,	7,	11,	10,	4,	13,	1,	5,	8,	12,	6,	9,	3,	2,	15,
		13,	8,	10,	1,	3,	15,	4,	2,	11,	6,	7,	12,	0,	5,	14,	9
	};

	row = (right & 0x21000000000) >> 36;
	row = (row >> 4) | (row & 0x1);
	column = (right & 0x1E000000000) >> 37;
	sRight <<= 4;
	sRight |= (sb2[16 * row + column] & 0xF);

	// S-Box 3
	char sb3[64] = {
		10,	0,	9,	14,	6,	3,	15,	5,	1,	13,	12,	7,	11,	4,	2,	8,
		13,	7,	0,	9,	3,	4,	6,	10,	2,	8,	5,	14,	12,	11,	15,	1,
		13,	6,	4,	9,	8,	15,	3,	0,	11,	1,	2,	12,	5,	10,	14,	7,
		1,	10,	13,	0,	6,	9,	8,	7,	4,	15,	14,	3,	11,	5,	2,	12
	};

	row = (right & 0x840000000) >> 30;
	row = (row >> 4) | (row & 0x1);
	column = (right & 0x780000000) >> 31;
	sRight <<= 4;
	sRight |= (sb3[16 * row + column] & 0xF);

	// S-Box 4
	char sb4[64] = {
		7,	13,	14,	3,	0,	6,	9,	10,	1,	2,	8,	5,	11,	12,	4,	15,
		13,	8,	11,	5,	6,	15,	0,	3,	4,	7,	2,	12,	1,	10,	14,	9,
		10,	6,	9,	0,	12,	11,	7,	13,	15,	1,	3,	14,	5,	2,	8,	4,
		3,	15,	0,	6,	10,	1,	13,	8,	9,	4,	5,	11,	12,	7,	2,	14
	};

	row = (right & 0x21000000) >> 24;
	row = (row >> 4) | (row & 0x1);
	column = (right & 0x1E000000) >> 25;
	sRight <<= 4;
	sRight |= (sb4[16 * row + column] & 0xF);

	// S-Box 5
	char sb5[64] = {
		2,	12,	4,	1,	7,	10,	11,	6,	8,	5,	3,	15,	13,	0,	14,	9,
		14,	11,	2,	12,	4,	7,	13,	1,	5,	0,	15,	10,	3,	9,	8,	6,
		4,	2,	1,	11,	10,	13,	7,	8,	15,	9,	12,	5,	6,	3,	0,	14,
		11,	8,	12,	7,	1,	14,	2,	13,	6,	15,	0,	9,	10,	4,	5,	3
	};

	row = (right & 0x840000) >> 18;
	row = (row >> 4) | (row & 0x1);
	column = (right & 0x780000) >> 19;
	sRight <<= 4;
	sRight |= (sb5[16 * row + column] & 0xF);

	// S-Box 6
	char sb6[64] = {
		12,	1,	10,	15,	9,	2,	6,	8,	0,	13,	3,	4,	14,	7,	5,	11,
		10,	15,	4,	2,	7,	12,	9,	5,	6,	1,	13,	14,	0,	11,	3,	8,
		9,	14,	15,	5,	2,	8,	12,	3,	7,	0,	4,	10,	1,	13,	11,	6,
		4,	3,	2,	12,	9,	5,	15,	10,	11,	14,	1,	7,	6,	0,	8,	13
	};

	row = (right & 0x21000) >> 12;
	row = (row >> 4) | (row & 0x1);
	column = (right & 0x1E000) >> 13;
	sRight <<= 4;
	sRight |= (sb6[16 * row + column] & 0xF);

	// S-Box 7
	char sb7[64] = {
		4,	11,	2,	14,	15,	0,	8,	13,	3,	12,	9,	7,	5,	10,	6,	1,
		13,	0,	11,	7,	4,	9,	1,	10,	14,	3,	5,	12,	2,	15,	8,	6,
		1,	4,	11,	13,	12,	3,	7,	14,	10,	15,	6,	8,	0,	5,	9,	2,
		6,	11,	13,	8,	1,	4,	10,	7,	9,	5,	0,	15,	14,	2,	3,	12
	};

	row = (right & 0x840) >> 6;
	row = (row >> 4) | (row & 0x1);
	column = (right & 0x780) >> 7;
	sRight <<= 4;
	sRight |= (sb7[16 * row + column] & 0xF);

	// S-Box 8
	char sb8[64] = {
		13,	2,	8,	4,	6,	15,	11,	1,	10,	9,	3,	14,	5,	0,	12,	7,
		1,	15,	13,	8,	10,	3,	7,	4,	12,	5,	6,	11,	0,	14,	9,	2,
		7,	11,	4,	1,	9,	12,	14,	2,	0,	6,	10,	13,	15,	3,	5,	8,
		2,	1,	14,	7,	4,	10,	8,	13,	15,	12,	9,	0,	3,	5,	6,	11
	};

	row = (right & 0x21);
	row = (row >> 4) | (row & 0x1);
	column = (right & 0x1E) >> 1;
	sRight <<= 4;
	sRight |= (sb8[16 * row + column] & 0xF);

	return sRight;
};

//Expansion Permutation Box to go from 32-bit half of input to 48-bits to be XORed with key - ARM
ull ep(ull right) {
	/*
	More Permutation, check bit on in block, if 1,
		shift a 1 to that position on out Block

		Expansion P-Box using the following table:
			32	 1	 2	 3	 4	 5
			 4	 5	 6	 7	 8	 9
			 8	 9	10	11	12	13
			12	13	14	15	16	17
			16	17	18	19	20	21
			20	21	22	23	24	25
			24	25	26	27	28	29
			28	29	30	31	32	 1
	*/
	ull pRight = 0;
	if (right & 0x80000000) pRight |= 1ULL;
	if (right & 0x1) {
		pRight |= (1ULL << 1);
		pRight |= (1ULL << 47);
	}
	if (right & 0x2) pRight |= (1ULL << 2);
	if (right & 0x4) pRight |= (1ULL << 3);
	if (right & 0x8) {
		pRight |= (1ULL << 4);
		pRight |= (1ULL << 6);
	}
	if (right & 0x10) {
		pRight |= (1ULL << 5);
		pRight |= (1ULL << 7);
	}
	if (right & 0x20) pRight |= (1ULL << 8);
	if (right & 0x40) pRight |= (1ULL << 9);
	if (right & 0x80) {
		pRight |= (1ULL << 10);
		pRight |= (1ULL << 12);
	}
	if (right & 0x100) {
		pRight |= (1ULL << 11);
		pRight |= (1ULL << 13);
	}
	if (right & 0x200) pRight |= (1ULL << 14);
	if (right & 0x400) {
		pRight |= (1ULL << 15);
		pRight |= (1ULL << 17);
	}
	if (right & 0x800) {
		pRight |= (1ULL << 16);
		pRight |= (1ULL << 18);
	}
	if (right & 0x1000) pRight |= (1ULL << 19);
	if (right & 0x2000) pRight |= (1ULL << 20);
	if (right & 0x4000) pRight |= (1ULL << 21);
	if (right & 0x8000) {
		pRight |= (1ULL << 22);
		pRight |= (1ULL << 24);
	}
	if (right & 0x10000) {
		pRight |= (1ULL << 23);
		pRight |= (1ULL << 25);
	}
	if (right & 0x20000) pRight |= (1ULL << 26);
	if (right & 0x40000) pRight |= (1ULL << 27);
	if (right & 0x80000) {
		pRight |= (1ULL << 28);
		pRight |= (1ULL << 30);
	}
	if (right & 0x100000) {
		pRight |= (1ULL << 29);
		pRight |= (1ULL << 31);
	}
	if (right & 0x200000) pRight |= (1ULL << 32);
	if (right & 0x400000) pRight |= (1ULL << 33);
	if (right & 0x800000) {
		pRight |= (1ULL << 34);
		pRight |= (1ULL << 36);
	}
	if (right & 0x1000000) {
		pRight |= (1ULL << 35);
		pRight |= (1ULL << 37);
	}
	if (right & 0x2000000) pRight |= (1ULL << 38);
	if (right & 0x4000000) pRight |= (1ULL << 39);
	if (right & 0x8000000) {
		pRight |= (1ULL << 40);
		pRight |= (1ULL << 42);
	}
	if (right & 0x10000000) {
		pRight |= (1ULL << 41);
		pRight |= (1ULL << 43);
	}
	if (right & 0x20000000) pRight |= (1ULL << 44);
	if (right & 0x40000000) pRight |= (1ULL << 45);
	if (right & 0x80000000) pRight |= (1ULL << 46);
	return pRight;
};

//Initial Permutation of 64-bit plaintext block - ARM
ull ip(ull block) {
	/*	
	More Permutation, check bit on in block, if 1, 
		shift a 1 to that position on out Block

	Straight P-Box using the following table:
		58	50	42	34	26	18	10	2
		60	52	44	36	28	20	12	4
		62	54	46	38	30	22	14	6
		64	56	48	40	32	24	16	8
		57	49	41	33	25	17	 9	1
		59	51	43	35	27	19	11	3
		61	53	45	37	29	21	13	5
		63	55	47	39	31	23	15	7
	*/
	ull pBlock = 0;
	if (block & 0x200000000000000) pBlock |= 1ULL;				//	58	50	42	34	26	18	10	2
	if (block & 0x2000000000000) pBlock |= (1ULL << 1);
	if (block & 0x20000000000) pBlock |= (1ULL << 2);
	if (block & 0x200000000) pBlock |= (1ULL << 3);
	if (block & 0x2000000) pBlock |= (1ULL << 4);
	if (block & 0x20000) pBlock |= (1ULL << 5);
	if (block & 0x200) pBlock |= (1ULL << 6);
	if (block & 0x2) pBlock |= (1ULL << 7);
	if (block & 0x800000000000000) pBlock |= (1ULL << 8);		//	60	52	44	36	28	20	12	4
	if (block & 0x8000000000000) pBlock |= (1ULL << 9);
	if (block & 0x80000000000) pBlock |= (1ULL << 10);
	if (block & 0x800000000) pBlock |= (1ULL << 11);
	if (block & 0x8000000) pBlock |= (1ULL << 12);
	if (block & 0x80000) pBlock |= (1ULL << 13);
	if (block & 0x800) pBlock |= (1ULL << 14);
	if (block & 0x8) pBlock |= (1ULL << 15);
	if (block & 0x2000000000000000) pBlock |= (1ULL << 16);	// 62	54	46	38	30	22	14	6
	if (block & 0x20000000000000) pBlock |= (1ULL << 17);
	if (block & 0x200000000000) pBlock |= (1ULL << 18);
	if (block & 0x2000000000) pBlock |= (1ULL << 19);
	if (block & 0x20000000) pBlock |= (1ULL << 20);
	if (block & 0x200000) pBlock |= (1ULL << 21);
	if (block & 0x2000) pBlock |= (1ULL << 22);
	if (block & 0x20) pBlock |= (1ULL << 23);
	if (block & 0x8000000000000000) pBlock |= (1ULL << 24);	// 64	56	48	40	32	24	16	8
	if (block & 0x80000000000000) pBlock |= (1ULL << 25);
	if (block & 0x800000000000) pBlock |= (1ULL << 26);
	if (block & 0x8000000000) pBlock |= (1ULL << 27);
	if (block & 0x80000000) pBlock |= (1ULL << 28);
	if (block & 0x800000) pBlock |= (1ULL << 29);
	if (block & 0x8000) pBlock |= (1ULL << 30);
	if (block & 0x80) pBlock |= (1ULL << 31);
	if (block & 0x100000000000000) pBlock |= (1ULL << 32); //	57	49	41	33	25	17	 9	1
	if (block & 0x1000000000000) pBlock |= (1ULL << 33);
	if (block & 0x10000000000) pBlock |= (1ULL << 34);
	if (block & 0x100000000) pBlock |= (1ULL << 35);
	if (block & 0x1000000) pBlock |= (1ULL << 36);
	if (block & 0x10000) pBlock |= (1ULL << 37);
	if (block & 0x100) pBlock |= (1ULL << 38);
	if (block & 0x1) pBlock |= (1ULL << 39);
	if (block & 0x400000000000000) pBlock |= (1ULL << 40); //	59	51	43	35	27	19	11	3
	if (block & 0x4000000000000) pBlock |= (1ULL << 41);
	if (block & 0x40000000000) pBlock |= (1ULL << 42);
	if (block & 0x400000000) pBlock |= (1ULL << 43);
	if (block & 0x4000000) pBlock |= (1ULL << 44);
	if (block & 0x40000) pBlock |= (1ULL << 45);
	if (block & 0x400) pBlock |= (1ULL << 46);
	if (block & 0x40) pBlock |= (1ULL << 47);
	if (block & 0x1000000000000000) pBlock |= (1ULL << 48); // 61	53	45	37	29	21	13	5
	if (block & 0x10000000000000) pBlock |= (1ULL << 49);
	if (block & 0x100000000000) pBlock |= (1ULL << 50);
	if (block & 0x1000000000) pBlock |= (1ULL << 51);
	if (block & 0x10000000) pBlock |= (1ULL << 52);
	if (block & 0x100000) pBlock |= (1ULL << 53);
	if (block & 0x1000) pBlock |= (1ULL << 54);
	if (block & 0x10) pBlock |= (1ULL << 55);
	if (block & 0x4000000000000000) pBlock |= (1ULL << 56); // 63	55	47	39	31	23	15	7
	if (block & 0x40000000000000) pBlock |= (1ULL << 57);
	if (block & 0x400000000000) pBlock |= (1ULL << 58);
	if (block & 0x4000000000) pBlock |= (1ULL << 59);
	if (block & 0x40000000) pBlock |= (1ULL << 60);
	if (block & 0x400000) pBlock |= (1ULL << 61);
	if (block & 0x4000) pBlock |= (1ULL << 62);
	if (block & 0x40) pBlock |= (1ULL << 63);
	return pBlock;
};

//Compression Permutation Box to go from 56-bit key to 48-bit key - ARM
void pc2(ull key, int round) {
	/* 
	Works much like pc1, but determines 16 keys to be
	used in the 16 rounds of the algorithm and places them in a global 
	array. This p-box uses the following table (subtracted from 56):
	
	Process is more permutation. Checks bit on in key, 
	if 1, shifts a 1 to that position on out key.

	Compression P-Box using the following table:
			14	17	11	24	1	5
			3	28	15	6	21	10
			23	19	12	4	26	8
			16	7	27	20	13	2
			41	52	31	37	47	55
			30	40	51	45	33	48
			44	49	39	56	34	53
			46	42	50	36	29	32
	*/
	roundkey[round] = 0;
	if (key & 0x2000) roundkey[round] |= 1ULL;							//	14	17	11	24	1	5
	if (key & 0x10000) roundkey[round] |= (1ULL << 1);
	if (key & 0x400) roundkey[round] |= (1ULL << 2);
	if (key & 0x800000) roundkey[round] |= (1ULL << 3);
	if (key & 0x1) roundkey[round] |= (1ULL << 4);
	if (key & 0x10) roundkey[round] |= (1ULL << 5);
	if (key & 0x4) roundkey[round] |= (1ULL << 7);						//	3	28	15	6	21	10
	if (key & 0x8000000) roundkey[round] |= (1ULL << 8);
	if (key & 0x4000) roundkey[round] |= (1ULL << 9);
	if (key & 0x20) roundkey[round] |= (1ULL << 10);
	if (key & 0x100000) roundkey[round] |= (1ULL << 11);
	if (key & 0x200) roundkey[round] |= (1ULL << 12);
	if (key & 0x400000) roundkey[round] |= (1ULL << 14);				//	23	19	12	4	26	8
	if (key & 0x40000) roundkey[round] |= (1ULL << 15);
	if (key & 0x800) roundkey[round] |= (1ULL << 16);
	if (key & 0x8) roundkey[round] |= (1ULL << 17);
	if (key & 0x2000000) roundkey[round] |= (1ULL << 18);
	if (key & 0x80) roundkey[round] |= (1ULL << 19);
	if (key & 0x8000) roundkey[round] |= (1ULL << 21);					//	16	7	27	20	13	2
	if (key & 0x40) roundkey[round] |= (1ULL << 22);
	if (key & 0x4000000) roundkey[round] |= (1ULL << 23);
	if (key & 0x80000) roundkey[round] |= (1ULL << 24);
	if (key & 0x1000) roundkey[round] |= (1ULL << 25);
	if (key & 0x2) roundkey[round] |= (1ULL << 26);
	if (key & 0x10000000000) roundkey[round] |= (1ULL << 28);			//	41	52	31	37	47	55
	if (key & 0x8000000000000) roundkey[round] |= (ull)(1 << 29);
	if (key & 0x40000000) roundkey[round] |= (1ULL << 30);
	if (key & 0x1000000000) roundkey[round] |= (1ULL << 31);
	if (key & 0x400000000000) roundkey[round] |= (1ULL << 32);
	if (key & 0x40000000000000) roundkey[round] |= (1ULL << 33);
	if (key & 0x20000000) roundkey[round] |= (1ULL << 35);		//	30	40	51	45	33	48
	if (key & 0x8000000000) roundkey[round] |= (1ULL << 36);
	if (key & 0x4000000000000) roundkey[round] |= (1ULL << 37);
	if (key & 0x100000000000) roundkey[round] |= (1ULL << 38);
	if (key & 0x100000000) roundkey[round] |= (1ULL << 39);
	if (key & 0x800000000000) roundkey[round] |= (1ULL << 40);
	if (key & 0x80000000000) roundkey[round] |= (1ULL << 42);		//	44	49	39	56	34	53
	if (key & 0x1000000000000) roundkey[round] |= (1ULL << 43);
	if (key & 0x4000000000) roundkey[round] |= (1ULL << 44);
	if (key & 0x80000000000000) roundkey[round] |= (1ULL << 45);
	if (key & 0x200000000) roundkey[round] |= (1ULL << 46);
	if (key & 0x10000000000000) roundkey[round] |= (1ULL << 47);
	if (key & 0x200000000000) roundkey[round] |= (1ULL << 42);	//	46	42	50	36	29	32
	if (key & 0x20000000000) roundkey[round] |= (1ULL << 43);
	if (key & 0x2000000000000) roundkey[round] |= (1ULL << 44);
	if (key & 0x800000000) roundkey[round] |= (1ULL << 45);
	if (key & 0x10000000) roundkey[round] |= (1ULL << 46);
	if (key & 0x80000000) roundkey[round] |= (1ULL << 47);
};

//Compression Permutation Box to go from 64-bit key to 56-bit key - ARM
ull pc1(ull key) {
	/*	
	First Permutation of key. Checks bit on in key, if 1, 
		shifts a 1 to that position on out key.

	Compression P-Box using the following table:
			57	49	41	33	25	17	9
			1	58	50	42	34	26	18
			10	2	59	51	43	35	27
			19	11	3	60	52	44	36
			63	55	47	39	31	23	15
			7	62	54	46	38	30	22
			14	6	61	53	45	37	29
			21	13	5	28	20	12	4
	*/
	
	ull pKey = 0;
	//if (key & 0x80) pKey |= 1ULL;			//	57	49	41	33	25	17	9
	//if (key & 0x8000) pKey |= (1ULL << 1);
	//if (key & 0x800000) pKey |= (1ULL << 2);
	//if (key & 0x80000000) pKey |= (1ULL << 3);
	//if (key & 0x8000000000) pKey |= (1ULL << 4);
	//if (key & 0x800000000000) pKey |= (1ULL << 5);
	//if (key & 0x80000000000000) pKey |= (1ULL << 6);
	//if (key & 0x8000000000000000) pKey |= (1ULL << 7);					//	1	58	50	42	34	26	18
	//if (key & 0x40) pKey |= (1ULL << 8);
	//if (key & 0x4000) pKey |= (1ULL << 9);
	//if (key & 0x400000) pKey |= (1ULL << 10);
	//if (key & 0x40000000) pKey |= (1ULL << 11);
	//if (key & 0x4000000000) pKey |= (1ULL << 12);
	//if (key & 0x400000000000) pKey |= (1ULL << 13);
	//if (key & 0x40000000000000) pKey |= (1ULL << 14);				//	10	2	59	51	43	35	27
	//if (key & 0x4000000000000000) pKey |= (1ULL << 15);
	//if (key & 0x20) pKey |= (1ULL << 16);
	//if (key & 0x2000) pKey |= (1ULL << 17);
	//if (key & 0x200000) pKey |= (1ULL << 18);
	//if (key & 0x20000000) pKey |= (1ULL << 19);
	//if (key & 0x2000000000) pKey |= (1ULL << 20);
	//if (key & 0x200000000000) pKey |= (1ULL << 21);				//	19	11	3	60	52	44	36
	//if (key & 0x20000000000000) pKey |= (1ULL << 22);
	//if (key & 0x2000000000000000) pKey |= (1ULL << 23);
	//if (key & 0x10) pKey |= (1ULL << 24);
	//if (key & 0x1000) pKey |= (1ULL << 25);
	//if (key & 0x100000) pKey |= (1ULL << 26);
	//if (key & 0x10000000) pKey |= (1ULL << 27);
	//if (key & 0x2) pKey |= (1ULL << 28);	//	63	55	47	39	31	23	15
	//if (key & 0x200) pKey |= (1ULL << 29);
	//if (key & 0x20000) pKey |= (1ULL << 30);
	//if (key & 0x2000000) pKey |= (1ULL << 31);
	//if (key & 0x200000000) pKey |= (1ULL << 32);
	//if (key & 0x20000000000) pKey |= (1ULL << 33);
	//if (key & 0x2000000000000) pKey |= (1ULL << 34);
	//if (key & 0x200000000000000) pKey |= (1ULL << 35);					//	7	62	54	46	38	30	22
	//if (key & 0x4) pKey |= (1ULL << 36);
	//if (key & 0x400) pKey |= (1ULL << 37);
	//if (key & 0x40000) pKey |= (1ULL << 38);
	//if (key & 0x4000000) pKey |= (1ULL << 39);
	//if (key & 0x400000000) pKey |= (1ULL << 40);
	//if (key & 0x40000000000) pKey |= (1ULL << 41);
	//if (key & 0x4000000000000) pKey |= (1ULL << 42);				//	14	6	61	53	45	37	29
	//if (key & 0x400000000000000) pKey |= (1ULL << 43);
	//if (key & 0x8) pKey |= (1ULL << 44);
	//if (key & 0x800) pKey |= (1ULL << 45);
	//if (key & 0x80000) pKey |= (1ULL << 46);
	//if (key & 0x8000000) pKey |= (1ULL << 47);
	//if (key & 0x800000000) pKey |= (1ULL << 48);
	//if (key & 0x80000000000) pKey |= (1ULL << 49);				//	21	13	5	28	20	12	4
	//if (key & 0x8000000000000) pKey |= (1ULL << 50);
	//if (key & 0x800000000000000) pKey |= (1ULL << 51);
	//if (key & 0x1000000000) pKey |= (1ULL << 52);
	//if (key & 0x100000000000) pKey |= (1ULL << 53);
	//if (key & 0x10000000000000) pKey |= (1ULL << 54);
	//if (key & 0x1000000000000000) pKey |= (1ULL << 55);
	if (key & 0x100000000000000) pKey |= 1ULL;			//	57	49	41	33	25	17	9
	if (key & 0x1000000000000) pKey |= (1ULL << 1);
	if (key & 0x10000000000) pKey |= (1ULL << 2);
	if (key & 0x100000000) pKey |= (1ULL << 3);
	if (key & 0x1000000) pKey |= (1ULL << 4);
	if (key & 0x10000) pKey |= (1ULL << 5);
	if (key & 0x100) pKey |= (1ULL << 6);
	if (key & 0x1) pKey |= (1ULL << 7);					//	1	58	50	42	34	26	18
	if (key & 0x200000000000000) pKey |= (1ULL << 8);
	if (key & 0x2000000000000) pKey |= (1ULL << 9);
	if (key & 0x20000000000) pKey |= (1ULL << 10);
	if (key & 0x200000000) pKey |= (1ULL << 11);
	if (key & 0x2000000) pKey |= (1ULL << 12);
	if (key & 0x20000) pKey |= (1ULL << 13);
	if (key & 0x200) pKey |= (1ULL << 14);				//	10	2	59	51	43	35	27
	if (key & 0x2) pKey |= (1ULL << 15);
	if (key & 0x400000000000000) pKey |= (1ULL << 16);
	if (key & 0x4000000000000) pKey |= (1ULL << 17);
	if (key & 0x40000000000) pKey |= (1ULL << 18);
	if (key & 0x400000000) pKey |= (1ULL << 19);
	if (key & 0x4000000) pKey |= (1ULL << 20);
	if (key & 0x40000) pKey |= (1ULL << 21);				//	19	11	3	60	52	44	36
	if (key & 0x400) pKey |= (1ULL << 22);
	if (key & 0x4) pKey |= (1ULL << 23);
	if (key & 0x800000000000000) pKey |= (1ULL << 24);
	if (key & 0x8000000000000) pKey |= (1ULL << 25);
	if (key & 0x80000000000) pKey |= (1ULL << 26);
	if (key & 0x800000000) pKey |= (1ULL << 27);
	if (key & 0x4000000000000000) pKey |= (1ULL << 28);	//	63	55	47	39	31	23	15
	if (key & 0x40000000000000) pKey |= (1ULL << 29);
	if (key & 0x400000000000) pKey |= (1ULL << 30);
	if (key & 0x4000000000) pKey |= (1ULL << 31);
	if (key & 0x40000000) pKey |= (1ULL << 32);
	if (key & 0x400000) pKey |= (1ULL << 33);
	if (key & 0x4000) pKey |= (1ULL << 34);
	if (key & 0x40) pKey |= (1ULL << 35);					//	7	62	54	46	38	30	22
	if (key & 0x2000000000000000) pKey |= (1ULL << 36);
	if (key & 0x20000000000000) pKey |= (1ULL << 37);
	if (key & 0x200000000000) pKey |= (1ULL << 38);
	if (key & 0x2000000000) pKey |= (1ULL << 39);
	if (key & 0x20000000) pKey |= (1ULL << 40);
	if (key & 0x200000) pKey |= (1ULL << 41);
	if (key & 0x2000) pKey |= (1ULL << 42);				//	14	6	61	53	45	37	29
	if (key & 0x20) pKey |= (1ULL << 43);
	if (key & 0x1000000000000000) pKey |= (1ULL << 44);
	if (key & 0x10000000000000) pKey |= (1ULL << 45);
	if (key & 0x100000000000) pKey |= (1ULL << 46);
	if (key & 0x1000000000) pKey |= (1ULL << 47);
	if (key & 0x10000000) pKey |= (1ULL << 48);
	if (key & 0x100000) pKey |= (1ULL << 49);				//	21	13	5	28	20	12	4
	if (key & 0x1000) pKey |= (1ULL << 50);
	if (key & 0x10) pKey |= (1ULL << 51);
	if (key & 0x8000000) pKey |= (1ULL << 52);
	if (key & 0x80000) pKey |= (1ULL << 53);
	if (key & 0x800) pKey |= (1ULL << 54);
	if (key & 0x8) pKey |= (1ULL << 55);
	return pKey;
};

//Creates all 16 round keys - ARM
void keygen(ull key) {
	//Pass 64-bit key to Permutation Compression Box (PC1) to permute and reduce to 56-bits
	key = pc1(key);

	/*	Split key into left and right halves to be shifted left using the following number
		of shifts on a given iteration for 16 iterations:
			1, 1, 2, 2, 2, 2, 2, 2, 1,  2,  2,  2,  2,  2,  2,  1
			1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
		Bitwise shift does not rotate, so in order to pick back up 
		the the bit shifted off, we need to OR that bit into the LSB.
		Each iteration sends C and D off to PC-2 which creates 
		that round's 48-bit key and saves it to a global roundKey array.
	*/
	ull C = (ull)((key >> 28) & 0xfffffff);
	ull D = (ull)(key & 0xfffffff);

	//iteration 1
	C = (0xfffffff & (C << 1)) | (0x1 & (C >> 27));
	D = (0xfffffff & (D << 1)) | (0x1 & (D >> 27));
	pc2(((C << 28) | D), 0);

	//iteration 2
	C = (0xfffffff & (C << 1)) | (0x1 & (C >> 27));
	D = (0xfffffff & (D << 1)) | (0x1 & (D >> 27));
	pc2(((C << 28) | D), 1);

	//iteration 3
	C = (0xfffffff & (C << 1)) | (0x1 & (C >> 27));
	D = (0xfffffff & (D << 1)) | (0x1 & (D >> 27));
	C = (0xfffffff & (C << 1)) | (0x1 & (C >> 27));
	D = (0xfffffff & (D << 1)) | (0x1 & (D >> 27));
	pc2(((C << 28) | D), 2);

	//iteration 4
	C = (0xfffffff & (C << 1)) | (0x1 & (C >> 27));
	D = (0xfffffff & (D << 1)) | (0x1 & (D >> 27));
	C = (0xfffffff & (C << 1)) | (0x1 & (C >> 27));
	D = (0xfffffff & (D << 1)) | (0x1 & (D >> 27));
	pc2(((C << 28) | D), 3);

	//iteration 5
	C = (0xfffffff & (C << 1)) | (0x1 & (C >> 27));
	D = (0xfffffff & (D << 1)) | (0x1 & (D >> 27));
	C = (0xfffffff & (C << 1)) | (0x1 & (C >> 27));
	D = (0xfffffff & (D << 1)) | (0x1 & (D >> 27));
	pc2(((C << 28) | D), 4);

	//iteration 6
	C = (0xfffffff & (C << 1)) | (0x1 & (C >> 27));
	D = (0xfffffff & (D << 1)) | (0x1 & (D >> 27));
	C = (0xfffffff & (C << 1)) | (0x1 & (C >> 27));
	D = (0xfffffff & (D << 1)) | (0x1 & (D >> 27));
	pc2(((C << 28) | D), 5);

	//iteration 7
	C = (0xfffffff & (C << 1)) | (0x1 & (C >> 27));
	D = (0xfffffff & (D << 1)) | (0x1 & (D >> 27));
	C = (0xfffffff & (C << 1)) | (0x1 & (C >> 27));
	D = (0xfffffff & (D << 1)) | (0x1 & (D >> 27));
	pc2(((C << 28) | D), 6);

	//iteration 8
	C = (0xfffffff & (C << 1)) | (0x1 & (C >> 27));
	D = (0xfffffff & (D << 1)) | (0x1 & (D >> 27));
	C = (0xfffffff & (C << 1)) | (0x1 & (C >> 27));
	D = (0xfffffff & (D << 1)) | (0x1 & (D >> 27));
	pc2(((C << 28) | D), 7);

	//iteration 9
	C = (0xfffffff & (C << 1)) | (0x1 & (C >> 27));
	D = (0xfffffff & (D << 1)) | (0x1 & (D >> 27));
	pc2(((C << 28) | D), 8);

	//iteration 10
	C = (0xfffffff & (C << 1)) | (0x1 & (C >> 27));
	D = (0xfffffff & (D << 1)) | (0x1 & (D >> 27));
	C = (0xfffffff & (C << 1)) | (0x1 & (C >> 27));
	D = (0xfffffff & (D << 1)) | (0x1 & (D >> 27));
	pc2(((C << 28) | D), 9);

	//iteration 11
	C = (0xfffffff & (C << 1)) | (0x1 & (C >> 27));
	D = (0xfffffff & (D << 1)) | (0x1 & (D >> 27));
	C = (0xfffffff & (C << 1)) | (0x1 & (C >> 27));
	D = (0xfffffff & (D << 1)) | (0x1 & (D >> 27));
	pc2(((C << 28) | D), 10);

	//iteration 12
	C = (0xfffffff & (C << 1)) | (0x1 & (C >> 27));
	D = (0xfffffff & (D << 1)) | (0x1 & (D >> 27));
	C = (0xfffffff & (C << 1)) | (0x1 & (C >> 27));
	D = (0xfffffff & (D << 1)) | (0x1 & (D >> 27));
	pc2(((C << 28) | D), 11);

	//iteration 13
	C = (0xfffffff & (C << 1)) | (0x1 & (C >> 27));
	D = (0xfffffff & (D << 1)) | (0x1 & (D >> 27));
	C = (0xfffffff & (C << 1)) | (0x1 & (C >> 27));
	D = (0xfffffff & (D << 1)) | (0x1 & (D >> 27));
	pc2(((C << 28) | D), 12);

	//iteration 14
	C = (0xfffffff & (C << 1)) | (0x1 & (C >> 27));
	D = (0xfffffff & (D << 1)) | (0x1 & (D >> 27));
	C = (0xfffffff & (C << 1)) | (0x1 & (C >> 27));
	D = (0xfffffff & (D << 1)) | (0x1 & (D >> 27));
	pc2(((C << 28) | D), 13);

	//iteration 15
	C = (0xfffffff & (C << 1)) | (0x1 & (C >> 27));
	D = (0xfffffff & (D << 1)) | (0x1 & (D >> 27));
	C = (0xfffffff & (C << 1)) | (0x1 & (C >> 27));
	D = (0xfffffff & (D << 1)) | (0x1 & (D >> 27));
	pc2(((C << 28) | D), 14);

	//iteration 16
	C = (0xfffffff & (C << 1)) | (0x1 & (C >> 27));
	D = (0xfffffff & (D << 1)) | (0x1 & (D >> 27));
	pc2(((C << 28) | D), 15);

	//for (int i = 0; i < 16; i++) {
	//	cout << hex << roundkey[i] << endl;
	//}
	
}

//Organizes the steps for encryption - ARM
ull des(ull block, string actionType) {
	/*	Passes block through initial permutation
		Splits block into 32-bit leftIn and rightIn halves
		leftOut set to rightIn
		rightIn passed through expansion permutation
		rightIn XORed with round key
		rightIn passed through substitution boxes
		rightIn passed through straight permutation box
		rightOut = result of rightIn XORed with LeftIn
		leftOut and rightOut are joined back together to form 64-bit output
		Output passed through final permutation before returning to be saved in the outFile
	*/
	//cout << hex << block << endl;
	ull left, right, tempR;
	block = ip(block);
	//cout << hex << block << endl;
	left = ((block >> 32) & 0xffffffff);
	right = (block & 0xffffffff);
	for (int i = 0; i <= 15; i++) {
		//Direction keys are applied is determined by actionType passed in
		if (actionType == "E"){ 
			tempR = right;
			right = ep(right);
			right ^= roundkey[i];
			right = sb(right);
			right = sp(right);
			right = right ^ left;
			left = tempR;
		}
		else if (actionType == "D") {
			tempR = right;
			right = ep(right);
			right ^= roundkey[15-i];
			right = sb(right);
			right = sp(right);
			right = right ^ left;
			left = tempR;
		}
	}
	block = ((right << 32) | left);
	//cout << hex << block << endl;
	block = fp(block);
	//cout << hex << block << endl;
	return block;
}

//Checks valid mode - ARM
bool validMode(string mode) {
	if (mode == "ECB" || mode == "CBC") {
		return true;
	}
	else {
		cout << "Not a valid mode. Valid modes include: ECB, CBC" << endl;
		return false;
	}
}

//Returns type of key used: 1) Hex, 2) 8-char (no space) 3) 8-char (with spaces). Else 0 if not valid. - ARM
int getKeyType(string strIn) {
	if (strIn.length() == 16) {
		for (int i = 0; i < 16; i++) {
			if (!isxdigit(strIn[i])) {
				cout << "Not a valid key. Must be 16-bit hex or 8-char" << endl;
				return 0;
			}
		}
		return 1;
	}
	else if (strIn.length() == 10 && strIn.substr(0, 1) == "'" && strIn.substr(9, 1) == "'") {
		return 2;
	}
	else {
		cout << "Not a valid key. Must be 16-bit hex or 8-char" << endl;
		return 0;
	}
}

//Checks valid action - ARM
bool validAction(string action) {
	if (action == "-E" || action == "-D") {
		return true;
	}
	else {
		cout << "Not a valid action. Valid actions include: -E, -D" << endl;
		return false;
	}
}

//Creates necessary hex bytes to and with buffer that should contain less than 8 bytes
ull getHexfBytes(size_t bytesLeft) {
	ull hexBytes = 0;
	for (size_t i = 0;  i < bytesLeft; i++) {
		hexBytes <<= 8;
		hexBytes |= 0xff;
	}
	return hexBytes;
}

//Creates Random Pad Bits
ull getRandBytes(int numToPad) {
	ull randBytes = 0;
	srand((unsigned int)time(NULL));
	for (int i = 0; i < numToPad; i++) {
		randBytes <<= 8;
		randBytes |= (rand() % 255);
	};
	return randBytes;
}

//
ull swapEndianness(void *X) {
	ull swapped = (ull)X;
	swapped = (swapped & 0x00000000FFFFFFFF) << 32 | (swapped & 0xFFFFFFFF00000000) >> 32;
	swapped = (swapped & 0x0000FFFF0000FFFF) << 16 | (swapped & 0xFFFF0000FFFF0000) >> 16;
	swapped = (swapped & 0x00FF00FF00FF00FF) << 8 | (swapped & 0xFF00FF00FF00FF00) >> 8;
	return swapped;
}

//Converts a string to all uppercase characters - ARM
string upCase(string str) {
	transform(str.begin(), str.end(), str.begin(), toupper);
	return str;
}

void prompt()
{
	cout << "Welcome to Aaron's DES Encrypter/Decrypter!" << endl;
	cout << "Accepted input: DES <-action> <key> <mode> <infile> <outfile>" << endl;
}

int main(int argc, char* argv[]){
	string action, mode, key;
	streampos begF, endF;
	ull hKey, block;
	int bytesLeft, size, shiftAmt, writeSize;
	errno_t err;
	
	if (argc != 6){
		cout << "Incorrect number of arguments supplied." << endl;
		prompt();
		return 1;
	}

	action = upCase(argv[1]);
	if (!validAction(action)){
		prompt();
		return 1;
	}
	action = action.substr(1, 1);
	key = argv[2];
	if (getKeyType(key) == 2) {
		hKey = (((ull)key[1] << 56) + ((ull)key[2] << 48) + ((ull)key[3] << 40) + ((ull)key[4] << 32) + ((ull)key[5] << 24) + ((ull)key[6] << 16) + ((ull)key[7] << 8) + (ull)key[8]);

	} 
	else { 
		hKey = strtoull(argv[2], nullptr, 16);
		//cout << hex << hKey << endl;
	}

	mode = upCase(argv[3]);
	if (!validMode(mode)){
		prompt();
		return 1;
	}

	err = fopen_s(&inStream, argv[4], "rb");
	if (!(err==0)) {
		cout << "Can't open input file " << argv[4] << endl;
		prompt();
		return 1;
	}

	err = fopen_s(&outStream, argv[5], "wb");
	if (!(err == 0)) {
		cout << "Can't open output file " << argv[5] << endl;
		prompt();
		return 1;
	}

	//	Determines length of file. - ARM
		fseek(inStream, 0, SEEK_END);
		size = ftell(inStream);
		rewind(inStream);

	//	Filesize limit of 31 bits. - ARM
		if (size > 2147483647) {
			cout << "File is too large to open. Must be <= 31 bits of data." << endl;
			prompt();
			return 1;
		}
	/*	Initialize block to 0, writeSize to 8, and generate round keys with hKey.
		When encrypting, need to encrypt filesize left padded with 32 random bits.
		When decrypting, need to check filesize by running first block through DES and
		keeping only the right half by ANDing with 0xffffffff. Take inStream filesize 
		subtract 8 bytes (that were added from the filesize on encryption), then
		subtract the newly decrypted filesize to determine any excess bytes.
		This value will be the number of random bytes padded on, so subtracting this number
		from 8-bytes will give the number of padded bytes (8-padded bytes will be what we want to keep)
	*/
		block = 0;
		writeSize = 8;
		keygen(hKey);
		if (action == "E") {
			block = size;
			block = ((getRandBytes(32) << 32) | size);
			block = des(block, action);
			block = _byteswap_uint64(block);
			fwrite(reinterpret_cast<char*>(&block), 8, 1, outStream);
			bytesLeft = (size % 8);
		}
		else {
			fread_s(reinterpret_cast<char*>(&block), 8, 1, 8, inStream);
			block = _byteswap_uint64(block);
			block = des(block, action);
			bytesLeft = ((size - 8) - (block & 0xffffffff));
		};


		// If filesize is less than 8 bytes, only read that amount, padding appropriately before passing through DES.
		// Guaranteed to be encrpytion if this is true because an encrypted file being decrypted would have at least 9 bytes.
		if (size < 8) {
			fread_s(reinterpret_cast<char*>(&block), bytesLeft, 1, bytesLeft, inStream);
			block = _byteswap_uint64(block);
			block = (((block & getHexfBytes(bytesLeft)) << (8 - bytesLeft)) | (getRandBytes(8 - bytesLeft)));
			block = des(block, action);
		}

		// Read file while successfully reading eight 1-byte items, pass through DES, write to outFile.
		while(fread_s(reinterpret_cast<char*>(&block), 8, 1, 8, inStream) == 8){
			block = _byteswap_uint64(block);
			block = des(block, action);
			if (action == "D" && (ftell(inStream) == size )) {
				shiftAmt = (bytesLeft * 8);
				block >>= shiftAmt;
				writeSize = (8-bytesLeft);
			};
			block = _byteswap_uint64(block);
			fwrite(reinterpret_cast<char*>(&block), writeSize, 1, outStream);
			block = 0;
		};

		// Catch any bytes left over, push them left the appropriate amount, pad with random bytes.
		if ((bytesLeft > 0) && (action == "E")) {
			block &= getHexfBytes(bytesLeft);
			shiftAmt = ((8 - bytesLeft) * 8);
			block <<= shiftAmt;
			block |= getRandBytes(8 - bytesLeft);
			block = des(block, action);
			block = _byteswap_uint64(block);
			fwrite(reinterpret_cast<char*>(&block), writeSize, 1, outStream);
		}

		fclose(inStream);
		fclose(outStream);

	return 0;
}

