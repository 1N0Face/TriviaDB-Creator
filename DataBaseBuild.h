#pragma once
#include "sqlite3.h"
#include <iostream>
#include <exception>
#include "curl/curl.h"
#include <json/json.h>
#include <iostream>
#include <fstream> 
#include <stdlib.h>
#include <time.h>
#define NUMBER_OF_STATEMENTS 4
class DataBaseBuild
{
public:
	DataBaseBuild(const std::string dbName, const int numOfQuestions); //constructor
	~DataBaseBuild(); //destructor
	void InsertQuestions(const Json::Value obj); // This function generate sql statements according to the rndNum
private:
	bool CreateQuestionsTable(); // This function create the table for the questions
	std::string generateStatementOrder(const Json::Value obj, const int index, const int rndNum) const; // This function generate sql statements according to the rndNum
	int _numOfQuestions;
	sqlite3* _db;
};