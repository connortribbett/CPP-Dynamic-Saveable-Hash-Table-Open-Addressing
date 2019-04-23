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
	currentNodeCount = 0;
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
	loadFactor = (float)currentNodeCount / (float)tableSize;

	if (loadFactor < lowerLoadThreshold) {
		if (tableSize != BASE_SIZE) {
			std::cout << "Current Nodes:" << currentNodeCount << " size:" << tableSize << std::endl;
			std::cout << "Load Factor, downsizing: " << loadFactor << std::endl;
			resize(false);
		}
	}
	else if (loadFactor > upperLoadThreshold) {
		std::cout << "Current Nodes:" << currentNodeCount << " size:" << tableSize << std::endl;
		std::cout << "Load Factor, upsizing: " << loadFactor << std::endl;
		resize(true);
	}
	return;
}
void hashTable::resize(bool factor) {
	int optSize = currentNodeCount * 1.33;
	int oldSize = tableSize;
	
	if (optSize <= BASE_SIZE) {
		optSize = BASE_SIZE;
	}
	if (!factor) { //downsizing
		if (tableSize == BASE_SIZE) {
			return;
		}
	}
	if (optSize == tableSize) {
		return;
	}
	tableSize = optSize;

	std::cout << "Resizing from " << oldSize << " to " << tableSize << std::endl;
	std::vector<hashNode> oldTable;
	for (int i = 0; i < oldSize; i++) {
		if (HashTable[i] != nullptr) {
			//push all into vector of hashNodes
			oldTable.push_back(*HashTable[i]);
		}
	}
	std::cout << "Old nodes being transfered: " << oldTable.size() << std::endl;
	deleteTable();
	setupTable();
	std::list<llNode>::iterator it2;
	for (int k = 0; k < oldTable.size(); k++) {
		addEmployee(oldTable[k].employeeName, oldTable[k].baseSalary, oldTable[k].commissionRate, true);
		if (oldTable[k].contracts.size() > 0) {
			for (it2 = oldTable[k].contracts.begin(); it2 != oldTable[k].contracts.end(); ++it2) {
				addContract(oldTable[k].employeeName, it2->clientName, it2->contractValue);
			}
		}
	}
	oldTable.clear();
	//Loop through size of vector, pop and add all into the new table
}
void hashTable::saveTable(std::string filename) {
	std::ofstream write(filename);
	if (write.is_open()) {
		write << tableSize << "\n";
	}
	for (int i = 0; i < tableSize; i++) {
		if (HashTable[i] != NULL) {

			write << "Employee," << HashTable[i]->employeeName << "," << HashTable[i]->baseSalary << "," << HashTable[i]->commissionRate << "\n";
			if (HashTable[i]->contracts.size() > 0) {
				for (auto v : HashTable[i]->contracts) {
					write << "Contract," << HashTable[i]->employeeName << "," << v.clientName << "," << v.contractValue << "\n";
				}

			}
		}
	}
}
void hashTable::addFromFile(std::string filename) {
	std::ifstream load(filename);
	if (load.is_open()) {
		std::string line;
		getline(load, line);
		//line here = tablesize
		while (getline(load, line)) {
			std::string type = "null";
			int pos1 = 0;
			int pos2 = 0;
			for (int i = 0; i < line.length(); i++) {
				if (line[i] == ',') {
					type = line.substr(0, i);
					i = 1000;
				}
				if (type == "Employee") {
					std::string name;
					float salary;
					float commission;
					for (int t = 0; t < line.length(); t++) {
						if (line[t] == ',' && pos1 == 2) {
							pos1++;
							salary = stof(line.substr(pos2 + 1, t - pos2 - 1));
							pos2 = t;
							commission = stof(line.substr(t + 1, 100));
						}
						if (line[t] == ',' && pos1 == 1) {
							pos1++;
							name = line.substr(pos2 + 1, t - pos2 - 1);
							pos2 = t;
						}
						if (line[t] == ',' && pos1 == 0) {

							pos1++;
							pos2 = t;
						}
					}
					addEmployee(name, salary, commission, false);
				}
				if (type == "Contract") {
					std::string name;
					std::string clientName;
					float value;
					for (int t = 0; t < line.length(); t++) {
						if (line[t] == ',' && pos1 == 2) {
							pos1++;
							clientName = line.substr(pos2 + 1, t - pos2 - 1);
							pos2 = t;
							value = stof(line.substr(t + 1, 100));
						}
						if (line[t] == ',' && pos1 == 1) {
							pos1++;
							name = line.substr(pos2 + 1, t - pos2 - 1);
							pos2 = t;
						}
						if (line[t] == ',' && pos1 == 0) {

							pos1++;
							pos2 = t;
						}
					}
					addContract(name, clientName, value);
				}
			}
		}
	}
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
void hashTable::addEmployee(std::string name, float salary, float commission, bool resize) {
	if (HashTable == nullptr) {
		setupTable();
	}
	hashNode *newNode = new hashNode;
	newNode->setNode(name, salary, commission);
	currentNodeCount++;
	
	int index = hash(name);
	if (HashTable[index] != nullptr) {
		index = quadraticProbe(index);
	}
	HashTable[index] = newNode;
	if (!resize) {
		checkLoad();
	}
	
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
	int index = hash(name);
	if (HashTable[index] != toDel) {
		for (int i = 1; (index + (i*i)) < tableSize; i++) {
			if (HashTable[index + (i*i)] == toDel) {
				index = index + (i*i);
				break;
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
	delete HashTable[index];
	HashTable[index] = nullptr;
	//delete toDel;
	//toDel = nullptr;
	currentNodeCount--;
	checkLoad();
	return;
}
void hashTable::deleteContract(std::string parent, std::string clientName) {
	hashNode *parentNode = searchEmployeeByName(parent);
	if (parentNode == nullptr) {
		return;
	}
	if (parentNode->contracts.size() > 0) {
		std::list<llNode>::iterator it; 
		for (it = parentNode->contracts.begin(); it != parentNode->contracts.end(); ++it) {
			if (it->clientName == clientName) {
				parentNode->contracts.erase(it);
				break;
			}
		}
	}
}
void hashTable::deleteTable() {
	delete[] HashTable;
	HashTable = nullptr;
}
void hashTable::listTable() {
	std::cout << std::endl << "Database List:" << std::endl;
	for (int i = 0; i < tableSize; i++) {
		if (HashTable[i] != nullptr) {
			std::cout << "==============" << std::endl;
			std::cout << "Employee : " << HashTable[i]->employeeName << " Salary : $" << HashTable[i]->baseSalary << " Commission: " << HashTable[i]->commissionRate * 100 << "%"<< std::endl;
			if (HashTable[i]->contracts.size() > 0) {
				std::list<llNode>::iterator it;
				for (it = HashTable[i]->contracts.begin(); it != HashTable[i]->contracts.end(); ++it) {
					std::cout << "--Client Name: " << it->clientName << " Value: $" << it->contractValue << std::endl;
				}
				std::cout << "---------------------" << std::endl;
			}
		}
	}
	std::cout << "==============" << std::endl;
}
void hashTable::employeePrintSearch(int compareFlag, int valueFlag, float searchValue) {
	/*compareFlag: -1 = '<'; 0 = '='; 1 = '>'
	valueFlag: 0 = comissionRate; 1 = baseSalary 2= contract count 3 = net value
	*/
	std::vector<hashNode> toPrint;
}
void hashTable::employeePrintByName(std::string name) {
	hashNode* search = searchEmployeeByName(name);
	if (search != nullptr) {
		std::cout << search->employeeName << "- Base Salary: " << search->baseSalary << " Comission Rate: " << search->commissionRate << std::endl;
		if (search->contracts.size() > 0) {
			std::list<llNode>::iterator it;
			for (it = search->contracts.begin(); it != search->contracts.end(); ++it) {
				std::cout << "-- " << it->clientName << "- Value: " << it->contractValue << std::endl;
			}
		}
	}
	else {
		std::cout << "Employee of name " << name << " not found." << std::endl;
	}
}
int main() {
	hashTable table;
	/*table.addEmployee("jerry", 0.0, 0.1, false);
	table.addEmployee("berry", 50000, 0.1, false);
	table.addEmployee("gerry", 50000, 0.12, false);
	table.addEmployee("terry", 40000, 0.12, false);
	table.addEmployee("lerry", 45000, 0.15, false);
	table.addEmployee("herry", 45000, 0.2, false);
	table.addEmployee("aak", 100000, .15, false);
	table.addEmployee("bak", 100000, .15, false);
	table.addEmployee("cak", 100000, .15, false);
	table.addEmployee("dak", 100000, .15, false);
	table.addEmployee("eak", 100000, .15, false);
	table.addEmployee("fak", 100000, .15, false);
	table.addEmployee("gak", 100000, .15, false);
	table.addEmployee("hak", 100000, .15, false);
	table.addEmployee("iak", 100000, .15, false);
	table.addEmployee("jak", 100000, .15, false);
	table.addEmployee("kak", 100000, .15, false);
	table.addEmployee("lak", 100000, .15, false);
	table.addEmployee("mak", 100000, .15, false);
	table.addEmployee("nak", 100000, .15, false);
	table.addEmployee("oak", 100000, .15, false);
	table.addEmployee("pak", 100000, .15, false);
	table.addEmployee("qak", 100000, .15, false);
	table.addEmployee("rak", 100000, .15, false);
	table.addEmployee("sak", 100000, .15, false);
	table.addEmployee("tak", 100000, .15, false);
	table.addEmployee("uak", 100000, .15, false);
	table.addEmployee("vak", 100000, .15, false);
	table.addEmployee("wak", 100000, .15, false);
	table.addEmployee("xak", 100000, .15, false);
	table.addEmployee("zk", 100000, .15, false);
	table.addEmployee("bzak", 100000, .15, false);
	table.addEmployee("zbbak", 100000, .15, false);
	table.addEmployee("zabak", 100000, .15, false);
	table.addEmployee("zdsbdsbak", 100000, .15, false);
	table.addEmployee("zasssk", 100000, .15, false);
	table.addEmployee("zahhk", 100000, .15, false);
	table.addEmployee("zaggk", 100000, .15, false);
	table.addEmployee("zakzzd", 100000, .15, false);
	table.addEmployee("zakfsa", 100000, .15, false);
	table.addEmployee("zagddk", 100000, .15, false);
	table.addEmployee("zaewtk", 100000, .15, false);
	table.addEmployee("zayywk", 100000, .15, false);
	table.addEmployee("zakrt3", 100000, .15, false);
	table.addEmployee("zakwqr", 100000, .15, false);
	table.addEmployee("zakasz", 100000, .15, false);
	table.addEmployee("zakfffff", 100000, .15, false);
	table.addEmployee("zakdsfdd3", 100000, .15, false);
	table.addEmployee("zak43", 100000, .15, false);
*/
	table.addEmployee("zak", 15000, .25, false);
	table.addContract("zak", "Valve", 50000);
	table.addContract("zak", "Oracle", 1000000);
	table.employeePrintByName("zak");
	//table.addFromFile("file.txt");
	//table.saveTable("saved.txt");
	table.listTable();
	//table.listTable();
	return 0;
}
