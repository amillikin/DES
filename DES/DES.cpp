// DES.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;
typedef unsigned long long ull;
typedef unsigned long ul;
typedef const unsigned char uc;
int keyType;
ull roundkey[16];
const ull oneULL = 0x0000000000000001;
const ull right32f = 0x00000000ffffffff;
const ull right28f = 0x000000000fffffff;
const ul oneUL = 0x00000001;

//Final Permutation for 64-bit block - ARM
ull fp(ull block) {
	/*	More Permutation, this time just a straight
	P-Box using the following table (subtracted from 64):
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
	pBlock <<= 1;
	pBlock |= ((block >> 24) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 56) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 16) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 48) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 8) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 40) & oneULL);
	pBlock <<= 1;
	pBlock |= (block & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 32) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 25) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 57) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 17) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 49) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 9) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 41) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 1) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 33) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 26) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 58) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 18) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 50) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 10) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 42) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 2) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 34) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 27) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 59) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 19) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 51) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 11) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 43) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 3) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 35) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 28) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 60) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 20) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 52) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 12) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 44) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 4) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 36) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 29) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 61) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 21) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 53) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 13) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 45) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 5) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 37) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 30) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 62) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 22) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 54) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 14) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 46) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 6) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 38) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 31) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 63) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 23) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 55) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 15) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 47) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 7) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 39) & oneULL);
	return pBlock;
};

//Straight Permutation Box to spread bits for following round - ARM
ull sp(ull right) {
	/*	More permutation using straight P-box 
		with the following table (subtracted from 32):
	16	7	20	21	29	12	28	17
	1	15	23	26	5	18	31	10
	2	8	24	14	32	27	3	9
	19	13	30	6	22	11	4	25
	*/
	ul pRight = 0;
	pRight <<= 1;
	pRight |= ((right >> 16) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 25) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 12) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 11) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 3) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 20) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 4) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 15) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 31) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 17) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 9) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 6) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 27) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 14) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 1) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 22) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 30) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 24) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 8) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 18) & oneUL);
	pRight <<= 1;
	pRight |= (right & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 5) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 29) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 23) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 13) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 19) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 2) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 26) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 10) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 21) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 28) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 7) & oneUL);
	return pRight;
};

//Eight 6x4 Substitution Boxes to take 48-bits into 32-bits - ARM
ull sb(ul right) {
	ul sRight = 0;
	/* S-Box 1
		14	4	13	1	2	15	11	8	3	10	6	12	5	9	0	7
		0	15	7	4	14	2	13	1	10	6	12	11	9	5	3	8
		4	1	14	8	13	6	2	11	15	12	9	7	3	10	5	0
		15	12	8	2	4	9	1	7	5	11	3	14	10	0	6	13
	*/
	sRight <<= 4;
	sRight |=

	/* S-Box 2
		15	1	8	14	6	11	3	4	9	7	2	13	12	0	5	10
		3	13	4	7	15	2	8	14	12	0	1	10	6	9	11	5
		0	14	7	11	10	4	13	1	5	8	12	6	9	3	2	15
		13	8	10	1	3	15	4	2	11	6	7	12	0	5	14	9
	*/

	/* S-Box 3
		10	0	9	14	6	3	15	5	1	13	12	7	11	4	2	8
		13	7	0	9	3	4	6	10	2	8	5	14	12	11	15	1
		13	6	4	9	8	15	3	0	11	1	2	12	5	10	14	7
		1	10	13	0	6	9	8	7	4	15	14	3	11	5	2	12
	*/

	/* S-Box 4
		7	13	14	3	0	6	9	10	1	2	8	5	11	12	4	15
		13	8	11	5	6	15	0	3	4	7	2	12	1	10	14	9
		10	6	9	0	12	11	7	13	15	1	3	14	5	2	8	4
		3	15	0	6	10	1	13	8	9	4	5	11	12	7	2	14
	*/

	/* S-Box 5
		2	12	4	1	7	10	11	6	8	5	3	15	13	0	14	9
		14	11	2	12	4	7	13	1	5	0	15	10	3	9	8	6
		4	2	1	11	10	13	7	8	15	9	12	5	6	3	0	14
		11	8	12	7	1	14	2	13	6	15	0	9	10	4	5	3
	*/

	/* S-Box 6
		12	1	10	15	9	2	6	8	0	13	3	4	14	7	5	11
		10	15	4	2	7	12	9	5	6	1	13	14	0	11	3	8
		9	14	15	5	2	8	12	3	7	0	4	10	1	13	11	6
		4	3	2	12	9	5	15	10	11	14	1	7	6	0	8	13
	*/

	/* S-Box 7
		4	11	2	14	15	0	8	13	3	12	9	7	5	10	6	1
		13	0	11	7	4	9	1	10	14	3	5	12	2	15	8	6
		1	4	11	13	12	3	7	14	10	15	6	8	0	5	9	2
		6	11	13	8	1	4	10	7	9	5	0	15	14	2	3	12
	*/

	/* S-Box 8
		13	2	8	4	6	15	11	1	10	9	3	14	5	0	12	7
		1	15	13	8	10	3	7	4	12	5	6	11	0	14	9	2
		7	11	4	1	9	12	14	2	0	6	10	13	15	3	5	8
		2	1	14	7	4	10	8	13	15	12	9	0	3	5	6	11
	*/

	return sRight;
};

//Expansion Permutation Box to go from 32-bit half of input to 48-bits to be XORed with key - ARM
ull ep(ul right) {
	/*	More permutation using expansion P-box 
		with the following table (subtracted from 32):
	32	1	2	3	4	5
	4	5	6	7	8	9
	8	9	10	11	12	13
	12	13	14	15	16	17
	16	17	18	19	20	21
	20	21	22	23	24	25
	24	25	26	27	28	29
	28	29	30	31	32	1
	*/
	ul pRight = 0;
	pRight <<= 1;
	pRight |= (right & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 31) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 30) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 29) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 28) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 27) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 28) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 27) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 26) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 25) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 24) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 23) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 24) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 23) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 22) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 21) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 20) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 19) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 20) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 19) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 18) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 17) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 16) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 15) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 16) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 15) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 14) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 13) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 12) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 11) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 12) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 11) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 10) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 9) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 8) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 7) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 8) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 7) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 6) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 5) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 4) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 3) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 4) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 3) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 2) & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 1) & oneUL);
	pRight <<= 1;
	pRight |= (right & oneUL);
	pRight <<= 1;
	pRight |= ((right >> 31) & oneUL);
	return pRight;
};

//Initial Permutation of 64-bit plaintext block - ARM
ull ip(ull block) {
	/*	More Permutation, this time just a straight
		P-Box using the following table (subtracted from 64):
		58	50	42	34	26	18	10	2
		60	52	44	36	28	20	12	4
		62	54	46	38	30	22	14	6
		64	56	48	40	32	24	16	8
		57	49	41	33	25	17	9	1
		59	51	43	35	27	19	11	3
		61	53	45	37	29	21	13	5
		63	55	47	39	31	23	15	7
	*/
	ull pBlock = 0;
	pBlock <<= 1;
	pBlock |= ((block >> 6) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 14) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 22) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 30) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 38) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 46) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 54) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 62) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 4) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 12) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 20) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 28) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 36) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 44) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 52) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 60) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 2) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 10) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 18) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 26) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 34) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 42) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 50) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 58) & oneULL);
	pBlock <<= 1;
	pBlock |= (block & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 8) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 16) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 24) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 32) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 40) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 48) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 56) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 7) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 15) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 23) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 31) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 39) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 47) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 55) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 63) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 5) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 13) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 21) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 29) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 37) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 45) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 53) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 61) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 3) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 11) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 19) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 27) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 35) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 43) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 51) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 59) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 1) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 9) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 17) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 25) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 33) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 41) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 49) & oneULL);
	pBlock <<= 1;
	pBlock |= ((block >> 57) & oneULL);
	return pBlock;
};

//Compression Permutation Box to go from 56-bit key to 48-bit key - ARM
void pc2(ull key, int round) {
	/* Works much like pc1, but determines 16 keys to be
	used in the 16 rounds of the algorithm and places them in a global 
	array. This p-box uses the following table (subtracted from 56):
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
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 42) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 39) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 45) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 32) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 55) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 51) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 53) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 28) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 41) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 50) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 35) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 46) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 33) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 37) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 44) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 52) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 30) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 48) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 40) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 49) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 29) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 36) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 43) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 54) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 15) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 4) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 25) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 19) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 9) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 1) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 26) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 16) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 5) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 11) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 23) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 8) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 12) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 7) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 17) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= (key & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 22) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 3) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 10) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 14) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 6) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 20) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 27) & oneULL);
	roundkey[round] <<= 1;
	roundkey[round] |= ((key >> 24) & oneULL);
};

//Compression Permutation Box to go from 64-bit key to 56-bit key - ARM
ull pc1(ull key) {
	/*	Initializes new pKey to 0 and sets each bit to value of ORing 
	with the appropriate value from the original key by using 
	a shift determined by taking 64 minus the following table:
	57	49	41	33	25	17	9
	1	58	50	42	34	26	18
	10	2	59	51	43	35	27
	19	11	3	60	52	44	36
	63	55	47	39	31	23	15
	7	62	54	46	38	30	22
	14	6	61	53	45	37	29
	21	13	5	28	20	12	4
	The shift puts the correct value in place to be ANDed with 1.
	If the key value is also 1, this will evaluate the right side to 1
	and since pKey is initialized to 0, the XOR will see the values are
	different and correctly turn that bit on.
	*/
	ull pKey = 0;
	pKey <<= 1;
	pKey |= ((key >> 7) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 15) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 23) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 31) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 39) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 47) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 55) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 63) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 6) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 14) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 22) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 30) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 38) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 46) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 54) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 62) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 5) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 13) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 21) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 29) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 37) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 45) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 53) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 61) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 4) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 12) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 20) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 28) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 1) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 9) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 17) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 25) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 33) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 41) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 49) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 57) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 2) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 10) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 18) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 26) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 34) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 42) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 50) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 58) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 3) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 11) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 19) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 27) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 35) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 43) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 51) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 59) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 36) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 44) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 52) & oneULL);
	pKey <<= 1;
	pKey |= ((key >> 60) & oneULL);
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
	*/
	ul C = (ul)((key >> 28) & right28f);
	ul D = (ul)(key & right28f);

	//iteration 1
	C = (right28f & (C << 1)) | (oneUL & (C >> 27));
	D = (right28f & (D << 1)) | (oneUL & (D >> 27));
	pc2((((ull)C) << 28) | ((ull)D), 0);

	//iteration 2
	C = (right28f & (C << 1)) | (oneUL & (C >> 27));
	D = (right28f & (D << 1)) | (oneUL & (D >> 27));
	pc2((((ull)C) << 28) | ((ull)D), 1);

	//iteration 3
	C = (right28f & (C << 1)) | (oneUL & (C >> 27));
	D = (right28f & (D << 1)) | (oneUL & (D >> 27));
	C = (right28f & (C << 1)) | (oneUL & (C >> 27));
	D = (right28f & (D << 1)) | (oneUL & (D >> 27));
	pc2((((ull)C) << 28) | ((ull)D), 2);

	//iteration 4
	C = (right28f & (C << 1)) | (oneUL & (C >> 27));
	D = (right28f & (D << 1)) | (oneUL & (D >> 27));
	C = (right28f & (C << 1)) | (oneUL & (C >> 27));
	D = (right28f & (D << 1)) | (oneUL & (D >> 27));
	pc2((((ull)C) << 28) | ((ull)D), 3);

	//iteration 5
	C = (right28f & (C << 1)) | (oneUL & (C >> 27));
	D = (right28f & (D << 1)) | (oneUL & (D >> 27));
	C = (right28f & (C << 1)) | (oneUL & (C >> 27));
	D = (right28f & (D << 1)) | (oneUL & (D >> 27));
	pc2((((ull)C) << 28) | ((ull)D), 4);

	//iteration 6
	C = (right28f & (C << 1)) | (oneUL & (C >> 27));
	D = (right28f & (D << 1)) | (oneUL & (D >> 27));
	C = (right28f & (C << 1)) | (oneUL & (C >> 27));
	D = (right28f & (D << 1)) | (oneUL & (D >> 27));
	pc2((((ull)C) << 28) | ((ull)D), 5);

	//iteration 7
	C = (right28f & (C << 1)) | (oneUL & (C >> 27));
	D = (right28f & (D << 1)) | (oneUL & (D >> 27));
	C = (right28f & (C << 1)) | (oneUL & (C >> 27));
	D = (right28f & (D << 1)) | (oneUL & (D >> 27));
	pc2((((ull)C) << 28) | ((ull)D), 6);

	//iteration 8
	C = (right28f & (C << 1)) | (oneUL & (C >> 27));
	D = (right28f & (D << 1)) | (oneUL & (D >> 27));
	C = (right28f & (C << 1)) | (oneUL & (C >> 27));
	D = (right28f & (D << 1)) | (oneUL & (D >> 27));
	pc2((((ull)C) << 28) | ((ull)D), 7);

	//iteration 9
	C = (right28f & (C << 1)) | (oneUL & (C >> 27));
	D = (right28f & (D << 1)) | (oneUL & (D >> 27));
	pc2((((ull)C) << 28) | ((ull)D), 8);

	//iteration 10
	C = (right28f & (C << 1)) | (oneUL & (C >> 27));
	D = (right28f & (D << 1)) | (oneUL & (D >> 27));
	C = (right28f & (C << 1)) | (oneUL & (C >> 27));
	D = (right28f & (D << 1)) | (oneUL & (D >> 27));
	pc2((((ull)C) << 28) | ((ull)D), 9);

	//iteration 11
	C = (right28f & (C << 1)) | (oneUL & (C >> 27));
	D = (right28f & (D << 1)) | (oneUL & (D >> 27));
	C = (right28f & (C << 1)) | (oneUL & (C >> 27));
	D = (right28f & (D << 1)) | (oneUL & (D >> 27));
	pc2((((ull)C) << 28) | ((ull)D), 10);

	//iteration 12
	C = (right28f & (C << 1)) | (oneUL & (C >> 27));
	D = (right28f & (D << 1)) | (oneUL & (D >> 27));
	C = (right28f & (C << 1)) | (oneUL & (C >> 27));
	D = (right28f & (D << 1)) | (oneUL & (D >> 27));
	pc2((((ull)C) << 28) | ((ull)D), 11);

	//iteration 13
	C = (right28f & (C << 1)) | (oneUL & (C >> 27));
	D = (right28f & (D << 1)) | (oneUL & (D >> 27));
	C = (right28f & (C << 1)) | (oneUL & (C >> 27));
	D = (right28f & (D << 1)) | (oneUL & (D >> 27));
	pc2((((ull)C) << 28) | ((ull)D), 12);

	//iteration 14
	C = (right28f & (C << 1)) | (oneUL & (C >> 27));
	D = (right28f & (D << 1)) | (oneUL & (D >> 27));
	C = (right28f & (C << 1)) | (oneUL & (C >> 27));
	D = (right28f & (D << 1)) | (oneUL & (D >> 27));
	pc2((((ull)C) << 28) | ((ull)D), 13);

	//iteration 15
	C = (right28f & (C << 1)) | (oneUL & (C >> 27));
	D = (right28f & (D << 1)) | (oneUL & (D >> 27));
	C = (right28f & (C << 1)) | (oneUL & (C >> 27));
	D = (right28f & (D << 1)) | (oneUL & (D >> 27));
	pc2((((ull)C) << 28) | ((ull)D), 14);

	//iteration 16
	C = (right28f & (C << 1)) | (oneUL & (C >> 27));
	D = (right28f & (D << 1)) | (oneUL & (D >> 27));
	pc2((((ull)C) << 28) | ((ull)D), 15);

	for (int i = 0; i < 16; i++) {
		cout << roundkey[i] << endl;
	}
}

//Organizes the steps for encryption - ARM
ull des(ull block, ull key, string actionType) {
	/*	Passes key to keygen to generate the 16 round keys
		Passes block through initial permutation
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
	ul leftIn, rightIn, rightOut, leftOut;
	keygen(key);
	block = ip(block);
	leftIn = (ul)((block >> 32) & right32f);
	rightIn = (ul)(block & right32f);
	leftOut = rightIn;
	for (int i = 0; i <= 15; i++) {
		//Direction keys are applied is determined by actionType passed in
		if (actionType == "E") {
			rightIn = ep(rightIn);
			rightIn ^ roundkey[i];
			rightIn = sb(rightIn);
			rightIn = sp(rightIn);
			rightOut = rightIn ^ leftIn;
		}
		else if (actionType == "D") {
			rightIn = ep(rightIn);
			rightIn ^ roundkey[15-i];
			rightIn = sb(rightIn);
			rightIn = sp(rightIn);
			rightOut = rightIn ^ leftIn;
		}
	}
	block = ((ull)right << 32) | ((ull)left);
	block = fp(block);
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
	string action, mode, key, inFilename, outFilename;
	streampos begF, endF, size;
	ull hKey;
	ifstream inFile;
	ofstream outFile;
	bool validEntry = false;


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
	key = argv[2];
	if (getKeyType(key) == 2) {
		hKey = (((ull)key[1] << 56) + ((ull)key[2] << 48) + ((ull)key[3] << 40) + ((ull)key[4] << 32) + ((ull)key[5] << 24) + ((ull)key[6] << 16) + ((ull)key[7] << 8) + (ull)key[8]);
	}
	else { 
		hKey = strtoull(argv[2], nullptr, 16);
	}

	mode = upCase(argv[3]);
	if (!validMode(mode)){
		prompt();
		return 1;
	}

	inFilename = argv[4];
	inFile.open(inFilename, inFile.binary);
	if (!inFile) {
		cout << "Can't open input file " << inFilename << endl;
		prompt();
		return 1;
	}

	outFilename = argv[5];
	outFile.open(outFilename, outFile.app);
	if (!outFile) {
		cout << "Can't open output file " << outFilename << endl;
		prompt();
		return 1;
	}

	hKey = 1383827165325090801;
	keygen(hKey);
	/* FIX - Needs to determine filesize so it knows how to handle 
		and pad if last block is less than 64 bits.

		begF = inFile.tellg();
		inFile.seekg(0, inFile.end);
		endF = inFile.tellg();
		size = endF - begF;
		cout << size << endl;
		inFile.seekg(0, inFile.beg);
		cout << hex << endl;
		for (ull buffer; inFile.read(reinterpret_cast<char*>(&buffer), sizeof buffer);) {
			if (inFile.gcount() < 64) {
				cout << inFile.gcount() << endl;
			}
			cout << setw(sizeof buffer * CHAR_BIT) << buffer << endl;
		}

		des(block, hKey, action);
	*/
	return 0;
}

