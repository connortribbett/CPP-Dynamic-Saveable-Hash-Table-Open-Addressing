#include "hashTable.hpp"

hashTable::hashTable() {
	setupTable();
}
hashTable::hashTable(std::string filename) {
	ifstream inFile;
	inFile.open(filename.c_str());
	if (inFile.is_open()) {
		std::string input;
		int size;
		inFile << input;
		size = stoi(input);
		if (size > BASE_SIZE) {
			tableSize = size;
		}
	}
	setupTable();
	addFromFile(filename);
}
hashTable::~hashTable() {
	deleteTable();
}
void hashTable::setupTable() {
	HashTable = new hashNode*[tableSize];
	for (int i = 0; i < tableSize; i++) {
		HashTable[i] = nullptr;
	}
}
unsigned int hashTable::hash(std::string key) {
	unsigned int hash = 5381;
	int len = key.length();
	for (int i = 0; i < len; i++) {
		hash = ((hash << 5) + hash) + key[i];
	}
	hash %= tableSize;
	return hash;
}
int hashTable::quadraticProbe(int startIndex) {
	/*
		Quadratically probes until an empty node is found
		if no node is found before the end of the table
		call the function again at startIndex+1
	*/
}

void hashTable::checkLoad() {
	loadFactor = (float)tableSize / (float)currentNodeCount;
	if (loadFactor < lowerLoadThreshold) {
		if (tableSize != BASE_SIZE) {
			resize(false);
		}
	}
	else if (loadFactor > upperLoadThreshold) {
		resize(true);
	}
	return;
}
void hashTable::resize(bool factor) { //False = resize to smaller table, True = resize to bigger table
	/*
		If true, resizes the array to the next largest prime that results in a load factor inside the bounds
		if false, resizes the array to the next lowest prime that results in a good load factor
	*/
}
void hashTable::saveTable(std::string filename) {
	/*
		Saves entire current table
		employee records followed by all of their contract records
	*/
}
void hashTable::addFromFile(std::string filename) {
	/*
		Adds all records from a file to the hash table
	*/
}
void hashTable::addEmployee(std::string name, float salary, float comission) {
	/*
		Adds a new node in the hash table for an employee
		if table is null, create one of base size
	*/
}
void hashTable::addContract(std::string parentName, std::string clientName, float value) {
	/*
		Add a contract LL node to an existing employee node based on their name
	*/
}
void hashTable::deleteEmployee(std::string name) {
	/*
		delete an employee record and all contracts based on their name
	*/
}
void hashTable::deleteContract(std::string clientName) {
	/*
		Delete a contract LL node based on the client name
	*/
}
void hashTable::deleteTable() {
	/*
		Iterate through array, delete all LLs present
		and then delete the entire table, freeing memory
	*/
}
void hashTable::deleteLL(llNode* head) {
	/*
		Delete an entire linked list starting from the head
	*/
}

int main() {
	return 0;
}