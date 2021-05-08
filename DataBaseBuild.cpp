#include "DataBaseBuild.h"
// This function is the constructor
DataBaseBuild::DataBaseBuild(const std::string dbName, const int numOfQuestions)
{
	_numOfQuestions = numOfQuestions;
    std::ofstream outfile(dbName); // opens file with the da
    outfile.close();
	int res = sqlite3_open(dbName.c_str(), &_db); // opens the dataBase to work with it
	if (res != SQLITE_OK) {
		_db = nullptr;
		throw std::exception("Error: Opening the database!");
	}
}
DataBaseBuild::~DataBaseBuild()
{
	sqlite3_close(_db);
	_db = nullptr;
}
//This function create the table for the questions
bool DataBaseBuild::CreateQuestionsTable()
{
	if (_db)
	{
		char* errMessage = nullptr;
		std::string sqlStatement = "CREATE TABLE Questions (Question TEXT NOT NULL PRIMARY KEY, Answer1 TEXT NOT NULL,Answer2 TEXT NOT NULL, Answer3 TEXT NOT NULL, Answer4 TEXT NOT NULL, CORRECT_ANSWER INTEGER  NOT NULL);";
		int checkRes = sqlite3_exec(_db, sqlStatement.c_str(), NULL, NULL, &errMessage);
		if (checkRes != SQLITE_OK) // if there was an error in the sqlite
			throw std::exception("Error: creating questions table!");
		return true;
	}
	return false;
}
// This function generate sql statements according to the rndNum
std::string DataBaseBuild::generateStatementOrder(const Json::Value obj,const int index, const int rndNum) const
{
	std::string sqlStatement[NUMBER_OF_STATEMENTS];

	sqlStatement[0] = "INSERT INTO Questions VALUES('" + obj["results"][index]["question"].asString()
		+ "', '" + obj["results"][index]["correct_answer"].asString() + "', '" + obj["results"][index]["incorrect_answers"][0].asString() + "', '" +
		obj["results"][index]["incorrect_answers"][1].asString() + "', '" + obj["results"][index]["incorrect_answers"][2].asString() + "', " + "1" + ");";

	sqlStatement[1] = "INSERT INTO Questions VALUES('" + obj["results"][index]["question"].asString()
		+ "', '" + obj["results"][index]["incorrect_answers"][0].asString() + "', '" + obj["results"][index]["correct_answer"].asString() + "', '" +
		obj["results"][index]["incorrect_answers"][1].asString() + "', '" + obj["results"][index]["incorrect_answers"][2].asString() + "', " + "2" + ");";

	sqlStatement[2] = "INSERT INTO Questions VALUES('" + obj["results"][index]["question"].asString()
		+ "', '" + obj["results"][index]["incorrect_answers"][0].asString() + "', '" + obj["results"][index]["incorrect_answers"][1].asString() + "', '" +
		obj["results"][index]["correct_answer"].asString() + "', '" + obj["results"][index]["incorrect_answers"][2].asString() + "', " + "3" + ");";

	sqlStatement[3] = "INSERT INTO Questions VALUES('" + obj["results"][index]["question"].asString()
		+ "', '" + obj["results"][index]["incorrect_answers"][0].asString() + "', '" + obj["results"][index]["incorrect_answers"][1].asString() + "', '" +
		obj["results"][index]["incorrect_answers"][2].asString() + "', '" + obj["results"][index]["correct_answer"].asString() + "', " + "4" + ");";

	switch (rndNum)
	{
	case 1:
		return sqlStatement[0];
		break;
	case 2:
		return sqlStatement[1];
		break;
	case 3:
		return sqlStatement[2];
		break;
	case 4:
		return sqlStatement[3];
		break;
	default:
		return sqlStatement[0];
		break;
	}
}
//This function inserts the questions to the database
void DataBaseBuild::InsertQuestions(const Json::Value obj)
{
	if (CreateQuestionsTable())
	{
		char* errMessage = nullptr;
		std::string sqlStatement;
		int checkRes, rndNum = 0;
		srand(time(NULL));
		for (int i = 0; i < _numOfQuestions; i++)
		{
			rndNum = rand() % NUMBER_OF_STATEMENTS + 1; // calculates a random number to make the correct answer in differnet place
			sqlStatement = generateStatementOrder(obj, i, rndNum);
			checkRes = sqlite3_exec(_db, sqlStatement.c_str(), NULL, NULL, &errMessage);
			if (checkRes != SQLITE_OK) // if there was an error in the sqlite
				throw std::exception("Error: creating questions!");
		}
	}
}