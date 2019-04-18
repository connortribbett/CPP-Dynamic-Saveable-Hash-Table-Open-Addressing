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
	if (startIndex >= tableSize) {
		return quadraticProbe(0);
	}
	for (int i = 1; startIndex + (i*i) < tableSize; i++)
	{
		if (HashTable[startIndex + (i*i)] == nullptr) {
			return startIndex + (i*i);
		}
	}
	return quadraticProbe(startIndex + 1);
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
void hashTable::resize(bool factor) {
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
hashNode* hashTable::searchEmployeeByName(std::string employeeName) {
	index = hash(employeeName);
	if (hashTable[index]->employeeName == employeeName) {
		return hashTable[index];
	}
	for (int i = 1; (index + (i*i)) < tableSize; i++) {
		if (hashTable[index + (i*i)]->employeename == employeeName) {
			return hashTable[index + (i*i)];
		}

		if (index + (i*i) >= tableSize) {
			if (index + i >= tableSize) {
				index = 0;
				i = 0;
			}
			else {
				index++;
				i = 0;
			}
		}
	}
}
void hashTable::addEmployee(std::string name, float salary, float commission) {
	if (HashTable == nullptr) {
		setupTable();
	}
	hashNode *newNode = new hasNode;
	newNode->setNode(name, salary, commission);
	currentNodeCount++;
	checkLoad();
	int index = hash(name);
	if (HashTable[index] != nullptr) {
		index = quadraticProbe(index);
	}
	HashTable[index] = newNode;
}
void hashTable::addContract(std::string parentName, std::string clientName, float value) {
	if (HashTable == nullptr) {
		return;
	}
	hashNode *parent = searchEmployeeByName(parentName);
	if (parent == nullptr) {
		return;
	}
	llNode newNode;
	newNode.setNode(clientName, value);
	parent->contracts.push_back(newNode);
	return;
}
void hashTable::deleteEmployee(std::string name) {
	hashNode *toDel = searchEmployeeByName(parentName);
	if (toDel == nullptr) {
		return;
	}
	delete toDel;
	return;
}
void hashTable::deleteContract(std::string parent, std::string clientName) {

}
void hashTable::deleteTable() {
	delete[] HashTable;
	HashTable = nullptr;
	tableSize = BASE_SIZE;
}

int main() {
	return 0;
}