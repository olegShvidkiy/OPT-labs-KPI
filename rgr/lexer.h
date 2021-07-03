#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#pragma once
using namespace std;
struct Lexeme
{
	int row;
	int col;
	int category;
	string token;
};



class Lexer {
private:
	char filePath[100];
	ifstream input;
	string fileName;
	int unsintegers = 501;
	int identifiers = 1001;
	
public:
	map <string, int> lexemes =
	{
		{"PROGRAM", 401},
		{"BEGIN",402},
		{"END",403},
		{"VAR",404},
		{"INTEGER",405},
		{"WHILE",406},
		{"DO",407},
		{"ENDWHILE",408},
		{"RETURN",409},
	};
	Lexer() {

	}
	Lexer(string inputFile);
	vector<Lexeme> parse();
};