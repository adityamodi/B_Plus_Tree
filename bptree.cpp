#include <bits/stdc++.h>
using namespace std;
#define INF 100.0
#define ERR 1e-8

typedef long long ll;

int fileAccess = 0;
int M,keyCount=0,nodeCount=0;
string bpt;

#include "Node.hpp"
#ifndef IO_FILE
#define IO_FILE
#include "io.hpp"
#endif

using chrono::duration_cast;
using chrono::high_resolution_clock;
using chrono::microseconds;

string searchNode(string nod, double key){
	Node N;
	N.readNode(nod);
	assert(N.numKeys == N.keys.size());
	assert(N.numKeys == N.fileName.size()-1);
	if(N.keys.size()==0) return nod;
	//int i;
	if(!N.isLeaf){
		int i = lower_bound(N.keys.begin(),N.keys.end(),key-ERR) - N.keys.begin();
		return searchNode(N.fileName[i],key);
	}
	return nod;
}

void addKey(string root, double key, string data){
	string leaf = searchNode(root,key);	//returns destination leaf Node for given key
	Node N;
	N.readNode(leaf);
	string fn = getDataFilename();
	N.insertintoLeaf(key,fn);
	if(N.numKeys > M){
		N.splitLeaf();		//split the Node since more keys than capacity
	}
	fn = "./data/" + fn;
	ofstream file;
	file.open(fn.c_str(),ios::trunc);
	file << data << "\n";
	file.close();
}

string makeRoot(){
	string root = getNodename();
	Node empt(1);
	empt.fName = root;
	empt.writeNode(root);
	return root;
}

void rangeQuery(string root, double a, double b, vector<double>& qkey, vector<string> &qdata){
	string leaf = searchNode(root,a);
	string data,fn;
	Node N;
	N.readNode(leaf);
	if(N.keys.empty()){
		return;
	}
	int i = lower_bound(N.keys.begin(),N.keys.end(),a-ERR) - N.keys.begin();
	while(1){
		if(i == N.numKeys && N.fileName[i]!="null"){
			N.readNode(N.fileName[i]);
			i=0;
		}
		if(N.keys[i] - ERR > b or i == N.numKeys) break;
		else{
			qkey.push_back(N.keys[i]);
			fn = "./data/" + N.fileName[i];
			ifstream file;
			file.open(fn.c_str());
			file >> data;
			file.close();
			qdata.push_back(data);
			i++;
		}
	}
}

void printTree(string bpt){
	Node N;
	N.readNode(bpt);
	if(N.isLeaf){
		N.printNode();
	}
	else{
		N.printNode();
		for(int i=0;i<N.fileName.size();i++){
			printTree(N.fileName[i]);
		}
	}
}

int main(int argc, char * ARGV[]){
	int query,fresh,qc=0;
	double key,key2;
	vector<double> qkey;
	vector<string> qdata;
	vector<ll> insertTime, pointTime, rangeTime;
    vector<int> insertAccess, pointAccess, rangeAccess;
	string data;
	ifstream config;
	config.open("bplustree.config");
	config >> M;
	config >> fresh;
	if(!fresh){
		config >> bpt;
		config >> nodeCount;
		config >> keyCount;
	}
	else{
		bpt = makeRoot();
		ifstream init;
		init.open("assgn2_bplus_data.txt");
		while(!init.eof() && init >> key){
			init >> data;
			addKey(bpt,key,data);
            qc++;
            cerr << qc << endl;
            //cerr << bpt << endl;
		}
		init.close();
	}
	cerr << bpt;
	config.close();
	ofstream out;
	out.open("bplustree.config",ios::trunc);
    out << M << "\n";
    out << "0\n";
    out << bpt << "\n";
    out << nodeCount << "\n";
    out << keyCount << "\n";
    out.close();
    if(argc < 2){
        cout << "No query file given\n";
        return 0;
    }
	ifstream pnts;
	cerr << ARGV[1] << endl;
	pnts.open(ARGV[1]);
    if(!pnts.is_open() || ARGV[1][0] == '>'){
        cout << "No query file given\n";
        return 0;
    }
	while(!pnts.eof() && pnts >> query){
        fileAccess = 0;
		pnts >> key;
		if(query == 0){
			pnts >> data;
			auto t0 = high_resolution_clock::now();
			addKey(bpt,key,data);
			auto t1 = high_resolution_clock::now();
            qc++;
            cerr << qc << endl;
            insertAccess.push_back(fileAccess);
			insertTime.push_back(ll(duration_cast<microseconds>(t1-t0).count()));
			cout << "insert (" << key << "->" << data << "): Success\n";
		}
		else if(query == 1){
			qkey.clear();
			qdata.clear();
			cout << "Point Query (" << key << "): ";
			auto t0 = high_resolution_clock::now();
			rangeQuery(bpt,key,key,qkey,qdata);
			auto t1 = high_resolution_clock::now();
            pointAccess.push_back(fileAccess);
			pointTime.push_back(ll(duration_cast<microseconds>(t1-t0).count()));
			if(!qkey.size()) cout << "Key does not exist!\n";
			else{
				cout << qdata[0];
				for(int i=1;i<qdata.size();i++) cout << ", " << qdata[i];
				cout << endl;
			}
		}
		else if(query == 2){
			pnts >> key2;
			qkey.clear();
			qdata.clear();
			cout << "Range Query (c:" << key <<",r:" << key2 << "): ";
			auto t0 = high_resolution_clock::now();
			rangeQuery(bpt,key - key2,key + key2,qkey,qdata);
			auto t1 = high_resolution_clock::now();
            rangeAccess.push_back(fileAccess);
			rangeTime.push_back(ll(duration_cast<microseconds>(t1-t0).count()));
			if(!qkey.size()) cout << "No such key exists\n";
			else{
				cout << qkey[0] << "->" << qdata[0];
				for(int i=1;i<qkey.size();i++) cout << ", " << qkey[i] << "->" << qdata[i];
				cout << endl;
			}
		}
	}
	pnts.close();
	out.open("bplustree.config",ios::trunc);
	out << M << "\n";
	out << "0\n";
	out << bpt << "\n";
	out << nodeCount << "\n";
	out << keyCount << "\n";
	out.close();
	sort(insertTime.begin(),insertTime.end());
	sort(pointTime.begin(),pointTime.end());
	sort(rangeTime.begin(),rangeTime.end());
	double sum = accumulate(insertTime.begin(),insertTime.end(),0.0);
	double mean = sum / insertTime.size();
	double sq_sum = inner_product(insertTime.begin(),insertTime.end(),insertTime.begin(),0.0);
	double std_dev = sqrt(sq_sum/insertTime.size() - mean*mean);
	cout << "\n###############\n##STATISTICS##\n##############\n\n";
	cout << "Insert query time stats:\n";
	cout << "Minimum: " << insertTime[0] << " us\nMaximum : " << insertTime[insertTime.size()-1] << " us\n";
	cout << "Average: " << mean << " us\nStd. dev. : " << std_dev << " us\n";
	sum = accumulate(pointTime.begin(),pointTime.end(),0.0);
	mean = sum / pointTime.size();
	sq_sum = inner_product(pointTime.begin(),pointTime.end(),pointTime.begin(),0.0);
	std_dev = sqrt(sq_sum/pointTime.size() - mean*mean);
	cout << "Point query time stats:\n";
	cout << "Minimum: " << pointTime[0] << " us\nMaximum : " << pointTime[pointTime.size()-1] << " us\n";
	cout << "Average: " << mean << " us\nStd. dev. : " << std_dev << " us\n";
	sum = accumulate(rangeTime.begin(),rangeTime.end(),0.0);
	mean = sum / rangeTime.size();
	sq_sum = inner_product(rangeTime.begin(),rangeTime.end(),rangeTime.begin(),0.0);
	std_dev = sqrt(sq_sum/rangeTime.size() - mean*mean);
	cout << "Range query time stats:\n";
	cout << "Minimum: " << rangeTime[0] << " us\nMaximum : " << rangeTime[rangeTime.size()-1] << " us\n";
	cout << "Average: " << mean << " us\nStd. dev. : " << std_dev << " us\n";
	sort(insertAccess.begin(),insertAccess.end());
	sort(pointAccess.begin(),pointAccess.end());
	sort(rangeAccess.begin(),rangeAccess.end());
	sum = accumulate(insertAccess.begin(),insertAccess.end(),0.0);
	mean = sum / insertAccess.size();
	sq_sum = inner_product(insertAccess.begin(),insertAccess.end(),insertAccess.begin(),0.0);
	std_dev = sqrt(sq_sum/insertAccess.size() - mean*mean);
	cout << "Insert query file access stats:\n";
	cout << "Minimum: " << insertAccess[0] << "\nMaximum : " << insertAccess[insertAccess.size()-1] << "\n";
	cout << "Average: " << mean << "\nStd. dev. : " << std_dev << "\n";
	sum = accumulate(pointAccess.begin(),pointAccess.end(),0.0);
	mean = sum / pointAccess.size();
	sq_sum = inner_product(pointAccess.begin(),pointAccess.end(),pointAccess.begin(),0.0);
	std_dev = sqrt(sq_sum/pointAccess.size() - mean*mean);
	cout << "Point query file access stats:\n";
	cout << "Minimum: " << pointAccess[0] << "\nMaximum : " << pointAccess[pointAccess.size()-1] << "\n";
	cout << "Average: " << mean << "\nStd. dev. : " << std_dev << "\n";
	sum = accumulate(rangeAccess.begin(),rangeAccess.end(),0.0);
	mean = sum / rangeAccess.size();
	sq_sum = inner_product(rangeAccess.begin(),rangeAccess.end(),rangeAccess.begin(),0.0);
	std_dev = sqrt(sq_sum/rangeAccess.size() - mean*mean);
	cout << "Range query file access stats:\n";
	cout << "Minimum: " << rangeAccess[0] << "\nMaximum : " << rangeAccess[rangeAccess.size()-1] << "\n";
	cout << "Average: " << mean << "\nStd. dev. : " << std_dev << "\n";
    return 0;
}
