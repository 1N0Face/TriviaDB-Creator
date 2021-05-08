#define CURL_STATICLIB
#include <iostream>
#include "curl/curl.h"
#include <json/json.h>
#include "DataBaseBuild.h"
#ifdef _DEBUG
#pragma comment (lib, "curl/libcurl_a_debug.lib")
#else
#pragma comment (lib, "curl/libcurl_a.lib")
#endif

#pragma comment (lib, "Normaliz.lib")
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Wldap32.lib")
#pragma comment (lib, "Crypt32.lib")
#pragma comment (lib, "advapi32.lib")
#pragma warning(disable : 4996)

#define ERROR_MSG "Bad Syntax! USE: DBCreator.exe <DB_BANE> <API_URL>"
#define ARGC_NUM 3
#define AMONT_WORD_LEN 7
#define DB_NAME_PARAM 1
#define URL_PARAM 2

// This function is a call back for the curl 
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}
// This function gets all the json data from the Trivia-API url
Json::Value getJsonDataFromUrl(std::string url)
{
    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    Json::Reader reader;
    Json::Value obj;
    reader.parse(readBuffer, obj);
    return obj;
}


// This function checks if the parameters are correct
bool WrongParams(const std::string dbName, const std::string url)
{
    return (dbName.find(".sqlite") == std::string::npos || url.find("http") == std::string::npos || url.find("amount=") == std::string::npos);
}


// This function gets the amount of questions according to the API URL
int getAmountFromUrl(std::string url)
{
    int amount = stoi(url.substr(url.find("amount=") + AMONT_WORD_LEN, 2)); // gets the amount from 10 to 100
    return amount;
}


int main(int argc, char** argv)
{
    try
    {
        if (argc == ARGC_NUM)
        {
            std::string dbName(argv[DB_NAME_PARAM]);
            std::string url(argv[URL_PARAM]);
            if (WrongParams(dbName, url)) //small check 
                throw std::exception(ERROR_MSG);
            int amountOfQuestions = getAmountFromUrl(url);
            DataBaseBuild* dbBuild = new DataBaseBuild(dbName, amountOfQuestions);
            Json::Value obj = getJsonDataFromUrl(url);
            dbBuild->InsertQuestions(obj);
            delete dbBuild;
        }
        else
            throw std::exception(ERROR_MSG);
    }
    catch (std::exception exc)
    {
        std::cout << exc.what() << std::endl;
    }
    return 0;
}
