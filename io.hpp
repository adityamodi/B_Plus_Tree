#include <bits/stdc++.h>
using namespace std;

string getNodename(){
	nodeCount++;
	char name[8];
	name[0] = 'f';
	name[7] = '\0';
	int num = nodeCount;
	int mod = 100000;
	for(int i=1;i<7;i++){
		name[i] = (num / mod) + '0';
		num %= mod;
		mod /= 10;
	}
	return name;
}

string getDataFilename(){
	keyCount++;
	char name[8];
	name[0] = 'd';
	name[7] = '\0';
	int num = keyCount;
	int mod = 100000;
	for(int i=1;i<7;i++){
		name[i] = (num / mod) + '0';
		num %= mod;
		mod /= 10;
	}
	return name;
}