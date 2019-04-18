#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#define BASE_SIZE 53 
#include<string>
#include<list>
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
	void addEmployee(std::string name, float salary, float comission);
	void addContract(std::string parentName, std::string clientName, float value);
	void deleteEmployee(std::string name);
	void deleteContract(std::string parent, std::string clientName);
	void saveTable(std::string filename);
	void addFromFile(std::string filename);
	void listTable();

private:
	float lowerLoadThreshold = .65f;
	float upperLoadThreshold = .85f;
	float loadFactor = 0.0;
	int primes[19] = {
		61, 97, 151, 211,257,307,
		353,409,457,509,547,791,1009,1223,1553,1999,2273, 2543,3001
	};
	int numPrimes = 19;
	int currPrime = -1; //-1 = BASE_SIZE, otherwise corresponding index to primes array

	unsigned int tableSize = BASE_SIZE;
	unsigned int currentNodeCount = 0;
	hashNode** HashTable = nullptr;

	unsigned int hash(std::string key);
	int quadraticProbe(int startIndex);
	void checkLoad();
	void resize(bool factor);
	void deleteTable();
	void setupTable();
	hashNode* searchEmployeeByName(std::string employeeName);
};

#endif