#ifndef DATABASE_CODE_TESTING_2_H
#define DATABASE_CODE_TESTING_2_H value

#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>
#include "DoublyLinkedList.h"

#define LINE_BUFFER_SIZE 50
#define CHARS_ALLOWED_IN_MEMORY 5000

using std::string; using std::to_string; using std::cout; using std::endl; using std::ifstream; using std::ofstream;

//splits the database into temp files and then merges the files in pairs
//takes optional arguments for primary field delimiter and secondary field list separators
bool sortDatabase(string database, const char delim = '\t', const char conn = ',');

//reads a subset of database determined by memory capacity into a DoublyLinkedList
//subsequently sorts the list, iterates through it to merge entries with duplicate primary fields separated by delim
//and concatenate secondary fields using conn as the separator
//and exports it into a temp file
//returns the count of temporary files
int splitDatabase(string &database, const char delim = '\t', const char conn = ',');

//merges two files
//eliminates duplicate entries by comparing substrings up to the first occurance of delim
//then combines the lines after delim by concatenating with conn
//returns a reference to the mergeCount for use by larger programs can update it
//so that mergeFiles will not overwrite externally created merge files
int& mergeFiles(const string &f1, const string &f2, const char delim = '\t', const char conn = ',', string mergeFileName = "merge.db");

#endif