// Title: Lab 3 - database.cpp
//
// Purpose: Implement functions to initialize database, add words, scores and occurrences to database,
//          find information about a review in the database, and find information about the database as a whole.
//
// Class: CSC 2430 Winter 2020
// Author: Andrew McLain

#include "database.h"
#include <string>
#include <iostream>
#include <sstream>

// Forward declaration

// Finds the index of a given word in the database, if the word is not found find the index where the
// word should be positioned according to alphabetical order
// Parameters:
//      database - the word database
//      word - word being searched for
//      insertionIndex - index of the word or index where the word should be placed
// Returns:
//      true indicates the word was found in the database, false indicates it was not
// Possible Errors:
//      no errors to handle
bool GetIndex(const Database& database, const string& word, int& insertionIndex){
    int left = 0;
    int right = database.countWords - 1;
    int mid;
    bool found = false;

    while(left <= right){
        mid = (left+ right) / 2;
        if(database.records[mid].word == word){
            insertionIndex = mid;
            found = true;
            return found;
        }
        else if(database.records[mid].word < word){
            left = mid + 1;
        }
        else{
            right = mid - 1;
        }
    }
    insertionIndex = left;
    return found;
}

// This is the value used to score a word not found in the database
const double neutral = 2.0;

// Checks the database for consistency, namely that the words are stored in sorted order
//      and the counts are positive
// Parameters:
//      database - the word database
// Returns:
//      true indicates the words are sored, false  false indicates they are not sorted
bool CheckDatabaseConsistency(Database& database) {
    bool passed;

    passed = true;
    if (database.countWords > 0) {
        // Check counts/scores for first word
        if (database.records[0].occurrenceCount <= 0 || database.records[0].scoreTotal == 0) {
            passed = false;
        }

        // Now do the rest in a loop including checking words are sorted
        for (int i = 1; i < database.countWords; i ++) {
            if (database.records[i-1].word >= database.records[i].word
                || database.records[i].occurrenceCount <= 0) {
                passed = false;
            }
        }
    }
    return passed;
}

// This function initializes the database with a word count of 0
// Parameters:
//      database - database struct containing array records and int countWords
// Returns:
//      returns nothing, modifies database.countWords
// Possible Errors:
//      no errors to handle
void InitDatabase(Database& database){
    database.countWords = 0;
}

// This function adds words and scores from a text file (passed as strings) to the database and
// increments occurrence counts appropriately.
// Parameters:
//      database - database struct containing array records and int countWords
//      word - current word being added to the database
//      score - score of current word being added to the database
// Returns:
//      true indicates a word was successfully added, false indicates the database has exceeded capacity
// Possible Errors:
//      Database full, return false in that case
bool AddWordToDatabase(Database& database, const string& word, int score){
    int insertionIndex = 0;
    if(database.countWords >= CAPACITY){
        return false;
    }
    if(GetIndex(database, word, insertionIndex)){
        database.records[insertionIndex].occurrenceCount += 1;
        database.records[insertionIndex].scoreTotal += score;
    }
    else{
        for(int i = database.countWords; i > insertionIndex; i--){
            database.records[i] = database.records[i -1];
        }
        database.records[insertionIndex].word = word;
        database.records[insertionIndex].occurrenceCount = 1;
        database.records[insertionIndex].scoreTotal = score;
        database.countWords++;
    }

    return database.countWords <= CAPACITY;
}

// This function finds a word in the database and its corresponding total score, average score
// and occurrence count
// Parameters:
//      database - database struct containing array records and int countWords
//      word - word being searched
//      occurrences - number of occurrences, which will be determined and modified by function
//      averageScore - average review score for the word based on tot. score and # of occurrences
// Returns:
//      no return, modifies occurrences and averageScore
// Possible Errors:
//      No errors to handle
void FindWordInDatabase(const Database &database, const string& word, int& occurrences, double& averageScore){
    if(database.countWords == 0){
        occurrences = 0;
        averageScore = neutral;
    }
    else{
        int insertionIndex = 0;
        if(GetIndex(database, word, insertionIndex)){
            occurrences = database.records[insertionIndex].occurrenceCount;
            averageScore = static_cast<double>(database.records[insertionIndex].scoreTotal) / occurrences;
        }
        else{
            occurrences = 0;
            averageScore = neutral;
        }
    }
}

// This function reports information about the database as a whole, including # of words, as well
// as max # of occurrences, min # of occurrences, max score, and min score for any word.
// Parameters:
//      database - database struct containing array records and int countWords
//      numberWords - number of words (to be determined and modified by the function)
//      maxOccurrences - maximum # of occurrences of a word (to be determined and modified by the function)
//      minOccurrences - minimum # of occurrences of a word (to be determined and modified by the function)
//      maxScore - maximum avg. score for any word in the database
//      minScore - minimum avg. score for any word in the database
// Returns:
//      no return, modifies numberWords, maxOccurrences, minOccurrences, maxScore, and minScore
// Possible Errors:
//      no errors to handle
void GetInfoAboutDatabase(const Database &database,
                          int& numberWords, int& maxOccurrences, int& minOccurrences, double& maxScore, double& minScore){
    numberWords = database.countWords;
    if(numberWords == 0){
        maxOccurrences = 0;
        minOccurrences = 0;
        maxScore = 0;
        minScore = 0;
    }
    else{
        maxOccurrences = database.records[0].occurrenceCount;
        minOccurrences = database.records[0].occurrenceCount;
        maxScore = static_cast<double>(database.records[0].scoreTotal) / database.records[0].occurrenceCount;
        minScore = static_cast<double>(database.records[0].scoreTotal) / database.records[0].occurrenceCount;

        for(int i = 0; i < database.countWords - 1; i++){
            if(database.records[i].occurrenceCount > maxOccurrences){
                maxOccurrences = database.records[i].occurrenceCount;
            }
            if(database.records[i].occurrenceCount < minOccurrences){
                minOccurrences = database.records[i].occurrenceCount;
            }
            if(static_cast<double>(database.records[i].scoreTotal) / database.records[i].occurrenceCount > maxScore){
                maxScore = static_cast<double>(database.records[i].scoreTotal) / database.records[i].occurrenceCount;
            }
            if(static_cast<double>(database.records[i].scoreTotal) / database.records[i].occurrenceCount < minScore){
                minScore = static_cast<double>(database.records[i].scoreTotal) / database.records[i].occurrenceCount;
            }
        }
    }
}
