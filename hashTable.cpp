#include "hashTable.hpp"

hashTable::hashTable() {
	setupTable();
}
hashTable::hashTable(std::string filename) {
	std::ifstream inFile;
	inFile.open(filename.c_str());
	if (inFile.is_open()) {
		std::string input;
		int size;
		getline(inFile, input, '\n');
		//inFile << input;
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
	if (factor) { //upsizing

	}
	else { //downsizing

	}
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
	int index = hash(employeeName);
	if (HashTable[index]->employeeName == employeeName) {
		return HashTable[index];
	}
	for (int i = 1; (index + (i*i)) < tableSize; i++) {
		if (HashTable[index + (i*i)]->employeeName == employeeName) {
			return HashTable[index + (i*i)];
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
	hashNode *newNode = new hashNode;
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
	hashNode *toDel = searchEmployeeByName(name);
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
void hashTable::listTable() {
	for (int i = 0; i < tableSize; i++) {
		if (HashTable[i] != NULL) {
			std::cout << "==============" << std::endl;
			std::cout << "Employee : " << HashTable[i]->employeeName << " Salary : $" << HashTable[i]->baseSalary << " Commission: " << HashTable[i]->commissionRate * 100 << "%"<< std::endl;
			if (HashTable[i]->contracts.size() > 0) {
				for (auto v : HashTable[i]->contracts) {
					std::cout << "Client Name: " << v.clientName << " Value: $" << v.contractValue << std::endl;
				}
				std::cout << "==============" << std::endl;
			}
		}
	}
}
int main() {
	hashTable table;
	table.addEmployee("jerry", 0.0, 0.1);
	table.addEmployee("berry", 50000, 0.1);
	table.addEmployee("gerry", 50000, 0.12);
	table.addEmployee("terry", 40000, 0.12);
	table.addEmployee("lerry", 45000, 0.15);
	table.addEmployee("herry", 45000, 0.2);
	table.addEmployee("zak", 100000, .15);
	table.addContract("jerry", "Rick", 100);
	table.addContract("berry", "Mick", 300);
	table.addContract("gerry", "Tick", 500);
	table.addContract("terry", "Lick", 1000);
	table.addContract("lerry", "Pick", 0.0);
	table.addContract("herry", "Wick", 0.0);
	table.addContract("zak", "Rick", 0.0);
	table.addContract("zak", "Rick", 200000);
	table.addContract("zak", "Sun Systems", 200000);
	table.addContract("zak", "Eating Tofu", 400000);
	table.addContract("zak", "Chipotle", 600000);
	table.listTable();
	return 0;
}