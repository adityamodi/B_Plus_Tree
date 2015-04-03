#include <bits/stdc++.h>
using namespace std;
#define INF 100.0

typedef long long ll;

int M,keyCount=0,nodeCount=0;
char filestr[16];
string bpt;

#include "Node.hpp"
#ifndef IO_FILE
#define IO_FILE
#include "io.hpp"
#endif

using chrono::duration_cast;
using chrono::high_resolution_clock;
using chrono::microseconds;

string findDestNode(string nod,double key){	//returns file name for the candidate leaf Node
	Node N;
	N.readNode(nod);
	if(N.keys.size()==0) return nod;
	//cout << "\nsearching this node : \n";
	//N.printNode();
	if(!N.isLeaf){
		int i = lower_bound(N.keys.begin(),N.keys.end(),key) - N.keys.begin();
		if(i == N.numKeys-1 && N.keys[i] < key) i++;
		return findDestNode(N.fileName[i],key);
	}
	return nod;
}

void addKey(string root, double key, string data){
	string leaf = findDestNode(root,key);	//returns destination leaf Node for given key
	Node N;
	N.readNode(leaf);
	string fn = getDataFilename();
	N.insertintoNode(key,fn,0);
	//N.printNode();
	if(N.numKeys > M){
		root = N.splitNode();		//split the Node since more keys than capacity
	}
	N.writeNode(leaf);
	fn = "./data/" + fn;
	ofstream file;
	strcpy(filestr, fn.c_str());
	file.open(filestr);
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

void pointQuery(string root, double a, vector<double>& qkey, vector<string> &qdata){
	string leaf = findDestNode(root,a);
	string data,fn;
	Node N;
	N.readNode(leaf);
	if(N.keys.empty()){
		return;
	}
	int i = lower_bound(N.keys.begin(),N.keys.end(),a) - N.keys.begin();
	if(i == N.numKeys || N.keys[i]!=a){
		return;	
	}
	else{
		qkey.push_back(N.keys[i]);
		fn = "./data/" + N.fileName[i];
		ifstream file;
		file.open(fn.c_str());
		file >> data;
		file.close();
		qdata.push_back(data);
	}
}

void rangeQuery(string root, double a, double b, vector<double>& qkey, vector<string> &qdata){
	string leaf = findDestNode(root,a);
	string data,fn;
	Node N;
	N.readNode(leaf);
	if(N.keys.empty()){
		return;
	}
	int i = lower_bound(N.keys.begin(),N.keys.end(),a) - N.keys.begin();
	while(1){
		if(i == N.numKeys && N.fileName[i]!="null"){
			N.readNode(N.fileName[i]);
			i=0;
		}
		if(N.keys[i] > b or i == N.numKeys) break;
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

int main(){
	int query,fresh;
	double key,key2;
	vector<double> qkey;
	vector<string> qdata;
	vector<ll> insertTime, pointTime, rangeTime;
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
		init.open("assgn2_bplus_data1.txt");
		while(!init.eof() && init >> key){
			init >> data;
			auto t0 = high_resolution_clock::now();
			addKey(bpt,key,data);
			auto t1 = high_resolution_clock::now();
			insertTime.push_back(ll(duration_cast<microseconds>(t1-t0).count()));
			cout << "Inserted key: " << key << "\n";
			cout << "Number of nodes: " << nodeCount << "\n\n";	
		}
	}
	config.close();
	cout << "maxkeys : " << M << "\n";
	ifstream pnts;
	pnts.open("querysample.txt");
	while(!pnts.eof() && pnts >> query){
		pnts >> key;
		if(query == 0){
			pnts >> data;
			auto t0 = high_resolution_clock::now();
			addKey(bpt,key,data);
			auto t1 = high_resolution_clock::now();
			insertTime.push_back(ll(duration_cast<microseconds>(t1-t0).count()));
			cout << "Inserted key: " << key << "\n";
			cout << "Number of nodes: " << nodeCount << "\n\n";
		}
		else if(query == 1){
			qkey.clear();
			qdata.clear();
			cout << "When searched for " << key << " we get:\n";
			auto t0 = high_resolution_clock::now();
			pointQuery(bpt,key,qkey,qdata);
			auto t1 = high_resolution_clock::now();
			pointTime.push_back(ll(duration_cast<microseconds>(t1-t0).count()));
			if(!qkey.size()) cout << "No matching key found\n";
			else cout << qkey[0] << " : " << qdata[0] << "\n";
			cout << "\n";
		}
		else if(query == 2){
			pnts >> key2;
			qkey.clear();
			qdata.clear();
			cout << "Keys in given range " << key - key2 << " and " << key + key2 << " are: \n";
			auto t0 = high_resolution_clock::now();
			rangeQuery(bpt,key - key2,key + key2,qkey,qdata);
			auto t1 = high_resolution_clock::now();
			rangeTime.push_back(ll(duration_cast<microseconds>(t1-t0).count()));
			if(!qkey.size()) cout << "No matching keys found.\n";
			else{
				for(int i=0;i<qkey.size();i++){
					cout << qkey[i] << " : " << qdata[i] << "\n";
				}
			}
			cout << "\n";
		}
		else if(query == 4){
			pnts >> key2;
			continue;
		}
	}
	pnts.close();
	ofstream out;
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
	cout << "Insert query stats:\n";
	cout << "Minimum: " << insertTime[0] << " us\nMaximum : " << insertTime[insertTime.size()-1] << " us\n";
	cout << "Average: " << mean << " us\nStd. dev. : " << std_dev << " us\n";
	sum = accumulate(pointTime.begin(),pointTime.end(),0.0);
	mean = sum / pointTime.size();
	sq_sum = inner_product(pointTime.begin(),pointTime.end(),pointTime.begin(),0.0);
	std_dev = sqrt(sq_sum/pointTime.size() - mean*mean);
	cout << "Point query stats:\n";
	cout << "Minimum: " << pointTime[0] << " us\nMaximum : " << pointTime[pointTime.size()-1] << " us\n";
	cout << "Average: " << mean << " us\nStd. dev. : " << std_dev << " us\n";
	sum = accumulate(rangeTime.begin(),rangeTime.end(),0.0);
	mean = sum / rangeTime.size();
	sq_sum = inner_product(rangeTime.begin(),rangeTime.end(),rangeTime.begin(),0.0);
	std_dev = sqrt(sq_sum/rangeTime.size() - mean*mean);
	cout << "Range query stats:\n";
	cout << "Minimum: " << rangeTime[0] << " us\nMaximum : " << rangeTime[rangeTime.size()-1] << " us\n";
	cout << "Average: " << mean << " us\nStd. dev. : " << std_dev << " us\n";
	return 0;
}