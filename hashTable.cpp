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
			resize(false);
		}
	}
	else if (loadFactor > upperLoadThreshold) {
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

	std::vector<hashNode> oldTable;
	for (int i = 0; i < oldSize; i++) {
		if (HashTable[i] != nullptr) {
			//push all into vector of hashNodes
			oldTable.push_back(*HashTable[i]);
		}
	}
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
			std::cout << "Employee : " << HashTable[i]->employeeName << " Salary : $" << HashTable[i]->baseSalary << " Commission: " << HashTable[i]->commissionRate * 100 << "%" << std::endl;
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
	valueFlag: 0 = comissionRate; 1 = baseSalary 2= contract count
	*/
	std::vector<hashNode> toPrint;
	float toCompare = 0;
	bool  match = false;
	for (int j = 0; j < tableSize; j++) {
		match = false;
		if (HashTable[j] != nullptr) {
			switch (valueFlag) {
			case 0:
				toCompare = HashTable[j]->commissionRate;
				break;
			case 1:
				toCompare = HashTable[j]->baseSalary;
				break;
			case 2:
				toCompare = HashTable[j]->contracts.size();
				break;
			default:
				return;
				break;
			}

			switch (compareFlag) {
			case -1:
				if (toCompare <= searchValue) {
					match = true;
				}
				break;
			case 0:
				if (toCompare == searchValue) {
					match = true;
				}
				break;
			case 1:
				if (toCompare >= searchValue) {
					match = true;
				}
				break;
			default:
				return;
				break;
			}
			if (match) {
				toPrint.push_back(*HashTable[j]);
			}
		}
	}


	std::cout << "Found " << toPrint.size() << " records matching your query:" << std::endl;
	for (int i = 0; i < toPrint.size(); i++) {
		std::cout << toPrint[i].employeeName << " - Base Salary: " << toPrint[i].baseSalary << " Comission Rate: " << toPrint[i].commissionRate << std::endl;
		if (toPrint[i].contracts.size() > 0) {
			std::list<llNode>::iterator it;
			for (it = toPrint[i].contracts.begin(); it != toPrint[i].contracts.end(); ++it) {
				std::cout << "--" << it->clientName << " - Value: " << it->contractValue << std::endl;
			}
		}
	}
}
void hashTable::employeePrintByName(std::string name) {
	hashNode* search = searchEmployeeByName(name);
	if (search != nullptr) {
		std::cout << search->employeeName << "- Base Salary: " << search->baseSalary << " Comission Rate: " << search->commissionRate << std::endl;
		if (search->contracts.size() > 0) {
			std::list<llNode>::iterator it;
			for (it = search->contracts.begin(); it != search->contracts.end(); ++it) {
				std::cout << "--" << it->clientName << " - Value: " << it->contractValue << std::endl;
			}
		}
	}
	else {
		std::cout << "Employee of name " << name << " not found." << std::endl;
	}
}
void hashTable::contractPrintSearchValue(int compareFlag, float searchValue) {
	std::vector<llNode> toPrint;
	std::vector<std::string> owners;
	bool match = false;

	for (int i = 0; i < tableSize; i++) {
		if (HashTable[i] != nullptr) {
			if (HashTable[i]->contracts.size() > 0) {

				std::list<llNode>::iterator it;
				for (it = HashTable[i]->contracts.begin(); it != HashTable[i]->contracts.end(); ++it) {
					match = false;
					switch (compareFlag) {
					case -1:
						if (it->contractValue <= searchValue) {
							match = true;
						}
						break;
					case 0:
						if (it->contractValue == searchValue) {
							match = true;
						}
						break;
					case 1:
						if (it->contractValue >= searchValue) {
							match = true;
						}
						break;
					default:
						break;
					}
					if (match) {
						toPrint.push_back(*it);
						owners.push_back(HashTable[i]->employeeName);
					}
				}
			}
		}
	}
	contractPrintHelp(toPrint, owners);
}
void hashTable::contractPrintSearchName(std::string name) {
	std::vector<llNode> toPrint;
	std::vector<std::string> owners;
	for (int i = 0; i < tableSize; i++) {
		if (HashTable[i] != nullptr) {
			if (HashTable[i]->contracts.size() > 0) {
				std::list<llNode>::iterator it;
				for (it = HashTable[i]->contracts.begin(); it != HashTable[i]->contracts.end(); ++it) {
					if (it->clientName == name) {
						toPrint.push_back(*it);
						owners.push_back(HashTable[i]->employeeName);
					}
				}
			}
		}
	}
	contractPrintHelp(toPrint, owners);
}
void hashTable::contractPrintHelp(std::vector<llNode> toPrint, std::vector<std::string> owners) {
	std::cout << "Found " << toPrint.size() << " contracts matching your qeury:" << std::endl;
	for (int i = 0; i < toPrint.size(); i++) {
		std::cout << "Contract Owner: " << owners[i] << " -- " << toPrint[i].clientName << " - Value: " << toPrint[i].contractValue << std::endl;
	}
}
int menu(int id) {
	if (id == 0) {
		std::cout << "1. Load from file" << std::endl;
		std::cout << "2. Save to file" << std::endl;
		std::cout << "3. Add Employee" << std::endl;
		std::cout << "4. Add Contract" << std::endl;
		std::cout << "5. Delete Employee" << std::endl;
		std::cout << "6. Delete Contract" << std::endl;
		std::cout << "7. Search" << std::endl;
		std::cout << "8. List Data" << std::endl;
		std::cout << "9. Quit" << std::endl;
		std::string response;
		std::cin >> response;
		int flag = 0;
		while (flag == 0) {
			if (response != "1" && response != "2" && response != "3" && response != "4" && response != "5" && response != "6" && response != "7" && response != "8" && response != "9") {
				std::cout << "Please select a digit" << std::endl;
				std::cin >> response;
			}
			else {
				flag = 1;
			}
		}
		return stoi(response);
	}
	if (id == 1) {
		std::cout << "1. Search Employee" << std::endl;
		std::cout << "2. List Range of Employees" << std::endl;
		std::cout << "3. Search Contracts by Value" << std::endl;
		std::cout << "4. Search Contracts by Name" << std::endl;
		std::string response;
		std::cin >> response;
		int flag = 0;
		while (flag == 0) {
			if (response != "1" && response != "2" && response != "3" && response != "4") {
				std::cout << "Please select a digit" << std::endl;
				std::cin >> response;
			}
			else {
				flag = 1;
			}
		}
		return stoi(response);
	}
}
int main() {
	hashTable table;
	for (int i = 0; i > -1; i++) {
		int response = menu(0);
		if (response == 1) {
			std::cout << "Filename?" << std::endl;
			std::string filename;
			std::cin >> filename;
			table.addFromFile(filename);
		}
		if (response == 2) {
			std::cout << "Filename?" << std::endl;
			std::string filename;
			std::cin >> filename;
			table.saveTable(filename);
		}
		if (response == 3) {
			std::cout << "Employee Name?" << std::endl;
			std::string name;
			std::string namee;
			std::cin >> namee;
			std::cout << "Employee Salary?" << std::endl;
			float salary;
			std::cin >> name;
			salary = stof(name);
			std::cout << "Employee Commission?" << std::endl;
			std::cin >> name;
			float commission;
			commission = stof(name);
			table.addEmployee(namee, salary, commission, false);
		}
		if (response == 4) {
			std::cout << "Employee Name?" << std::endl;
			std::string name;
			std::cin >> name;
			std::cout << "Client Name?" << std::endl;
			std::string client;
			std::cin >> client;
			std::cout << "Contract Value?" << std::endl;
			std::string t;
			std::cin >> t;
			float commission;
			commission = stof(t);
			table.addContract(name, client, commission);
		}
		if (response == 5) {
			std::cout << "Employee Name?" << std::endl;
			std::string name;
			std::cin >> name;
			table.deleteEmployee(name);
		}
		if (response == 6) {
			std::cout << "Employee Name?" << std::endl;
			std::string name;
			std::cin >> name;
			std::cout << "Client Name?" << std::endl;
			std::string client;
			std::cin >> client;
			table.deleteContract(name, client);
		}
		if (response == 7) {
			int choice = menu(1);
			if (choice == 1) {
				std::cout << "Employee Name?" << std::endl;
				std::string name;
				std::cin >> name;
				table.employeePrintByName(name);
			}
			if (choice == 2) {
				std::cout << "1. Search by commission rate" << std::endl;
				std::cout << "2. Search by salary" << std::endl;
				std::cout << "3. Search by # of contracts" << std::endl;
				std::string response;
				std::cin >> response;
				int t = stoi(response);
				std::cout << std::endl;
				std::cout << "1. Greater than" << std::endl;
				std::cout << "2. Equal to" << std::endl;
				std::cout << "3. Less than" << std::endl;
				std::cin >> response;
				int b = stoi(response);
				if (b == 2) {
					b = 0;
				}
				if (b == 3) {
					b = -1;
				}
				std::cout << "Value?" << std::endl;
				std::cin >> response;
				float x = stof(response);
				table.employeePrintSearch(b, t, x);
			}
			if (choice == 3) {
				std::cout << "1. Greater than" << std::endl;
				std::cout << "2. Equal to" << std::endl;
				std::cout << "3. Less than" << std::endl;
				std::string response;
				std::cin >> response;
				int b = stoi(response);
				if (b == 2) {
					b = 0;
				}
				if (b == 3) {
					b = -1;
				}
				std::cout << "Value?" << std::endl;
				float value;
				std::cin >> response;
				value = stof(response);
				table.contractPrintSearchValue(b, value);
			}
			if (choice == 4) {
				std::cout << "Name?" << std::endl;
				std::string name;
				std::cin >> name;
				table.contractPrintSearchName(name);
			}
		}
		if (response == 8) {
			table.listTable();
		}
		if (response == 9) {
			i = -2;
		}
	}
	//table.listTable();
	return 0;
}
