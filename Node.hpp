#include <bits/stdc++.h>
using namespace std;

#ifndef IO_FILE
#define IO_FILE
#include "io.hpp"
#endif

extern int M;
class Node{
public:
	bool isLeaf;
	vector<double> keys;
	int numKeys;
	vector<string> fileName;
	string parName;
	string fName;
	Node(bool = 0);
	void printNode();
	void readNode(string s);
	void writeNode(string s);
	void insertintoNode(double key, string fn, int flag,int = 0);
	string splitNode();
	void setParent();
};

Node::Node(bool flag){
	numKeys = 0;
	isLeaf = flag;
	parName = "null";
	fileName.push_back("null");
	assert(keys.size() + 1 == fileName.size());
}

void Node::printNode(){
	assert(keys.size() + 1 == fileName.size());
	assert(keys.size() == numKeys);
	cout << "\nPrinted node: \n";
	cout << "fileName: " << fName << "\n" << "isLeaf: " << isLeaf << "\n" << "parName: " << parName << "\n" << "numKeys: " << numKeys << "\n";
	for(int i=0;i<keys.size();i++) cout << keys[i] << "\n";
	for(int i=0;i<=numKeys;i++) cout << fileName[i] << "\n";
}

void Node::insertintoNode(double key, string fn, int flag, int is_up){
	int ind = lower_bound(keys.begin(),keys.end(),key - ERR) - keys.begin();
	if(is_up && !keys.empty()){
		ind = upper_bound(keys.begin(),keys.end(),key + ERR) - keys.begin();
	}
	//cout << ind << " : this is madness\n";
	vector<double>::iterator it1 = keys.begin();
	keys.insert(it1+ind,key);
	vector<string>::iterator it = fileName.begin();
	fileName.insert(it+ind+flag,fn);
	numKeys++;
	assert(keys.size() + 1 == fileName.size());
	assert(keys.size() == numKeys);
}

void Node::readNode(string s){
	s = "./data/" + s;
    fileAccess++;
	double temp;
	string ts;
	keys.clear();
	fileName.clear();
	ifstream file;
	strcpy(filestr, s.c_str());
	file.open(filestr);
	file >> fName >> isLeaf >> parName >> numKeys;
	for(int i=0;i<numKeys;i++){
		file >> temp;
		keys.push_back(temp);
	}
	for(int i=0;i<=numKeys;i++){
		file >> ts;
		fileName.push_back(ts);
	}
	file.close();
}

void Node::writeNode(string s){
	s = "./data/" + s;
    fileAccess++;
	ofstream file;
	strcpy(filestr, s.c_str());
	file.open(filestr,ios::trunc);
	file << fName << "\n" << isLeaf << "\n" << parName << "\n" << numKeys << "\n";
	for(int i=0;i<numKeys;i++) file << keys[i] << "\n";
	for(int i=0;i<=numKeys;i++) file << fileName[i] << "\n";
	file.close();
}

void Node::setParent(){
	Node ch;
	for(int i=0;i<fileName.size();i++){
		ch.readNode(fileName[i]);
		ch.parName = fName;
		ch.writeNode(ch.fName);
	}
}

string Node::splitNode(){
	assert(keys.size() + 1 == fileName.size());
	assert(keys.size() == numKeys);
	string newName = getNodename();
	double key;
	string fin = newName;
	Node newNode;
	newNode.fileName.clear();
	if(isLeaf){
		newNode.isLeaf = 1;
		for(int i=(numKeys-1)/2;i<numKeys;i++){
			newNode.keys.push_back(keys[i]);
			newNode.fileName.push_back(fileName[i]);
		}
		newNode.fileName.push_back(fileName[numKeys]);
		keys.erase(keys.begin() + (numKeys-1)/2,keys.end());
		fileName.erase(fileName.begin() + (numKeys-1)/2, fileName.end());
		fileName.push_back(newName);
		newNode.numKeys = numKeys - numKeys/2;
		numKeys = numKeys/2;
		key = keys[numKeys-1];
	}
	else{
		key = keys[(numKeys-1)/2];
		for(int i=(numKeys+1)/2;i<numKeys;i++){
			newNode.keys.push_back(keys[i]);
			newNode.fileName.push_back(fileName[i]);
		}
		newNode.fileName.push_back(fileName[numKeys]);
		keys.erase(keys.begin() + (numKeys-1)/2,keys.end());
		fileName.erase(fileName.begin() + (numKeys+1)/2, fileName.end());
		newNode.numKeys = numKeys/2;
		numKeys = (numKeys-1)/2;
	}
	newNode.fName = newName;
	Node parNode;
	if(parName == "null"){
		parNode.fileName.clear();
		parNode.fileName.push_back(fName);
		parName = getNodename();
		parNode.fName = parName;
		bpt = parName;
	}
	else parNode.readNode(parName);
	if(key < newNode.keys[newNode.numKeys - 1] + ERR) parNode.insertintoNode(key,newName,1,1);
	else parNode.insertintoNode(key,newName,1);
	newNode.parName = parName;
	//cout << "\nsplitnode : \n";
	if(parNode.numKeys > M) newNode.parName = parNode.splitNode();
	//newNode.printNode();
	if(!isLeaf) newNode.setParent();
	newNode.writeNode(newName);
	parNode.writeNode(parName);
	return fin;
}
