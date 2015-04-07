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
	void insertintoNode(double key, string lft, string rgt);
	void insertintoLeaf(double key, string fn);
	void splitNode();
	void splitLeaf();
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

void Node::insertintoLeaf(double key, string fn){
	int ind = lower_bound(keys.begin(),keys.end(),key-ERR) - keys.begin();
	vector<double>::iterator it1 = keys.begin();
	keys.insert(it1+ind,key);
	vector<string>::iterator it = fileName.begin();
	fileName.insert(it+ind,fn);
	numKeys++;
	writeNode(fName);
	assert(keys.size() + 1 == fileName.size());
	assert(keys.size() == numKeys);
}

void Node::insertintoNode(double key, string lft, string rgt){
	int i;
	for(i=0;i<fileName.size();i++){
		if(!lft.compare(fileName[i])) break;
	}
	vector<string>::iterator itf = fileName.begin();
	fileName.insert(itf+i+1,rgt);
	vector<double>::iterator it = keys.begin();
	keys.insert(it+i,key);
	numKeys++;
	writeNode(fName);
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
	file.open(s.c_str());
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
	file.open(s.c_str(),ios::trunc);
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

void Node::splitNode(){
	assert(keys.size() + 1 == fileName.size());
	assert(keys.size() == numKeys);
	string newName = getNodename();
	double key;
	Node newNode;
	newNode.fileName.clear();
	newNode.fName = newName;
	key = keys[numKeys/2];
	for(int i=numKeys/2+1;i<numKeys;i++){
		newNode.keys.push_back(keys[i]);
		newNode.fileName.push_back(fileName[i]);
	}
	newNode.fileName.push_back(fileName[numKeys]);
	keys.erase(keys.begin()+numKeys/2,keys.end());
	fileName.erase(fileName.begin()+numKeys/2+1,fileName.end());
	newNode.numKeys = numKeys - numKeys/2 -1;
	numKeys = numKeys/2;
	newNode.setParent();
	Node parNode;
	if(parName=="null"){
		//new parent
		parNode.fileName.clear();
		parNode.fileName.push_back(fName);
		parName = getNodename();
		parNode.fName = parName;
		bpt = parName;
	}
	else parNode.readNode(parName);
	newNode.parName = parName;
	newNode.writeNode(newName);
	writeNode(fName);
	parNode.insertintoNode(key,fName,newName);
	if(parNode.numKeys>M) parNode.splitNode();
}

void Node::splitLeaf(){
	assert(keys.size() + 1 == fileName.size());
	assert(keys.size() == numKeys);
	string newName = getNodename();
	double key;
	string fin = newName;
	Node newNode;
	newNode.fileName.clear();
	newNode.isLeaf = 1;
	for(int i=numKeys/2+1;i<numKeys;i++){
		newNode.keys.push_back(keys[i]);
		newNode.fileName.push_back(fileName[i]);
	}
	newNode.fName = newName;
	newNode.fileName.push_back(fileName[numKeys]);
	keys.erase(keys.begin()+numKeys/2+1,keys.end());
	fileName.erase(fileName.begin()+numKeys/2+1,fileName.end());
	fileName.push_back(newName);
	newNode.numKeys = numKeys - numKeys/2 - 1;
	numKeys = numKeys - numKeys/2;
	Node parNode;
	if(parName=="null"){
		//new parent
		parNode.fileName.clear();
		parNode.fileName.push_back(fName);
		parName = getNodename();
		parNode.fName = parName;
		bpt = parName;
	}
	else parNode.readNode(parName);
	newNode.parName = parName;
	newNode.writeNode(newName);
	writeNode(fName);
	parNode.insertintoNode(keys[numKeys-1],fName,newName);
	if(parNode.numKeys > M) parNode.splitNode();
}
