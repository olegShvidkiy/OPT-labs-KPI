#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <iomanip>
#pragma once



using namespace std;

class Parser {
private:
	char filePath[100];
	ifstream input;
	string fileName;
	int unsintegers = 501;
	int identifiers = 1001;
	int newLexem = 5001;
	
public:
	map <string, int> lexemes =
	{
		{"PROGRAM", 401},
		{"BEGIN",402},
		{"END",403},
		{"VAR",404},
		{"INTEGER",405},
	};
	Parser() {

	}
	
	
	Parser(string inputFile);
	void parse();
};
