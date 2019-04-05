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
	hashNode *parent; //= searchEmployees(parentName) 
	if (parent == nullptr) {
		return;
	}
	llNode *pPrev;
	llNode *newNode = new llNode;
	newNode->setNode(clientName, value);
	if (parent->head == nullptr) {
		parent->head = newNode;
		newNode->prev = nullptr;
		newNode->next = nullptr;
		return;
	}
	else {
		pPrev = parent->head;
		while (pPrev->next != nullptr) {
			pPrev = pPrev->next;
		}
		pPrev->next = newNode;
		newNode->prev = pPrev;
		newNode->next = nullptr;
	}
	return;
}
void hashTable::deleteEmployee(std::string name) {
	hashNode *toDel; //= searchemployees(parentName);
	if (toDel == nullptr) {
		return;
	}
	deleteLL(toDel->head);
	delete toDel;
	return;
}
void hashTable::deleteContract(std::string parent, std::string clientName) {
	llNode *toDel; //=searchContract(clientName)
	hashNode *toDelHashNode; //=searchEmployees(parent)
	if (toDel->prev == nullptr) {
		delete toDel;
		toDelHashNode->head == nullptr;
		return;
	}
	toDel->prev->next = toDel->next;
	delete toDel;
}
void hashTable::deleteTable() {
	for (int i = 0; i < tableSize; i++) {
		if (HashTable[i] == nullptr)
		{
			continue;
		}
		dellLL(HashTable[i]->head);
	}
	delete[] HashTable;
	HashTable = nullptr;
}
void hashTable::deleteLL(llNode* head) {
	if (head == nullptr) {
		return;
	}
	llNode* curr, next;
	next = head;
	while (next != nullptr) {
		curr = next;
		next = curr->next;
		delete curr;
	}
}

int main() {
	return 0;
}