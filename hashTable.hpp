#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#define BASE_SIZE 53 
#include<string>
#include<fstream>

struct llNode {
	llNode* next;
	llNode* prev;
	std::string clientName;
	float contractValue;
	setNode(std::string name, float value)
	{
		this->clientName = name;
		this->contractValue = value;
	}
};
struct hashNode {
	llNode* head;
	std::string employeeName;
	float baseSalary;
	float commissionRate;
	setNode(std::string name, float salary, float commission) {
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
	void deleteContract(std::string clientName);
	void saveTable(std::string filename);
	void addFromFile(std::string filename);
	template<class T>
	hashNode* searchEmployees(T query, int searchFlag) {
		/*
			query holds search term such as name, salary, comission rate
			searchFlag holds which record to search for such as name or salary
			and based on that set an offset from the struct hashNode to compare it with
			Flag also contains what operation to check again the query <,>,=,<=,>=
		*/
	};
	template<class T>
	llNode* searchContracts(T query, int searchFlag) {
	};

private:
	float lowerLoadThreshold = .65f;
	float upperLoadThreshold = .85f;
	int primes[50] = {
		61, 71, 83, 97, 107, 113, 127, 137, 151, 163, 173, 181, 191, 199, 211, 223, 229, 239, 241,257,269,277,293,307
		313,331,347,353,367,383,397,409,421,433,443,457,467,487,488,509,523,547,791,1009,1223,1553,1999,2273, 2543,3001
	};
	int numPrimes = 50;

	unsigned int tableSize = BASE_SIZE;
	unsigned int currentNodeCount = 0;
	hashNode** HashTable = nullptr;
	float loadFactor = 0.0;

	unsigned int hash(std::string key);
	int quadraticProbe(int startIndex);
	void checkLoad();
	void resize(bool factor);
	void deleteTable();
	void deleteLL();
	void setupTable();
};

#endif