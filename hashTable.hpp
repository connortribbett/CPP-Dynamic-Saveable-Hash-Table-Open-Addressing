#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#define BASE_SIZE 53 
#include<string>
#include<list>
#include<vector>
#include<fstream>
#include<iostream>

struct llNode {
	std::string clientName;
	float contractValue;
	void setNode(std::string name, float value)
	{
		this->clientName = name;
		this->contractValue = value;
	}
};
struct hashNode {
	std::list<llNode> contracts;
	std::string employeeName;
	float baseSalary;
	float commissionRate;
	void setNode(std::string name, float salary, float commission) {
		this->baseSalary = salary;
		this->commissionRate = commission;
		this->employeeName = name;
	}
};

class hashTable {
public:
	hashTable();
	hashTable(std::string filename);
	~hashTable();
	void addEmployee(std::string name, float salary, float comission, bool resize);
	void addContract(std::string parentName, std::string clientName, float value);
	void deleteEmployee(std::string name);
	void deleteContract(std::string parent, std::string clientName);
	void saveTable(std::string filename);
	void addFromFile(std::string filename);
	void listTable();
	void employeePrintByName(std::string name);
	void employeePrintSearch(int compareFlag, int valueFlag, float searchValue);
	void contractPrintSearchValue(int compareFlag, float searchValue);
	void contractPrintSearchName(std::string name);
	void contractPrintHelp(std::vector<llNode> toPrint, std::vector<std::string> owners);

private:
	float lowerLoadThreshold = .65f;
	float upperLoadThreshold = .90f;
	float loadFactor = 0.0;

	unsigned int tableSize = BASE_SIZE;
	unsigned int currentNodeCount = 0;
	hashNode** HashTable = nullptr;

	hashNode* searchEmployeeByName(std::string employeeName);
	unsigned int hash(std::string key);
	int quadraticProbe(int startIndex);
	void checkLoad();
	void resize(bool factor);
	void deleteTable();
	void setupTable();

};

#endif