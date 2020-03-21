// Title: Lab 3 - database.h
//
// Purpose: Declares the functions that must be used for any attempt to access the word database.
//
// Class: CSC 2430 Winter 2020
// Author: Max Benson

#ifndef SENTIMENT_ANALYSIS_DATABASE_H
#define SENTIMENT_ANALYSIS_DATABASE_H

//
// Please do not change the code below
//
#include <iostream>

using namespace std;

const int CAPACITY  = 20000;

// The main program should not include any code that uses the data definition
struct Record {
    string word;
    int occurrenceCount;
    int scoreTotal;
};

struct Database {
    Record records[CAPACITY];
    int countWords;
};

void InitDatabase(Database& database);
bool AddWordToDatabase(Database& database, const string& word, int score);
void FindWordInDatabase(const Database &database, const string& word, int& occurrences, double& averageScore);
void GetInfoAboutDatabase(const Database &database,
        int& numberWords, int& maxOccurrences, int& minOccurrences, double& maxScore, double& minScore);
bool CheckDatabaseConsistency(Database& database);

#endif //SENTIMENT_ANALYSIS_DATABASE_H
