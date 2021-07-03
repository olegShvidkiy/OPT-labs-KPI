#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "lexer.h"
using namespace std;

struct TreeNode {
	string keyword;
	vector<Lexeme*> lexemes;
	vector<TreeNode*> next;
};

class Parser {
private:
	string fileName;
	vector<Lexeme> lexemes;
	int i = 0;
public:
	Parser(string file, vector<Lexeme> input);
	TreeNode* parse();
	void program(TreeNode*, string);
	void procedure_identifier(TreeNode*, string);
	void block(TreeNode*, string);
	void variable_declarations(TreeNode*, string);
	void declaration_list(TreeNode*, string);
	void empty(TreeNode*, string);
	void identifier(TreeNode*, string);
	void statements_list(TreeNode*, string);
	void declaration(TreeNode*, string);
	void variable_identifier(TreeNode*, string);
	void statement(TreeNode*, string);

	void expression(TreeNode*, string);
	void summands_list(TreeNode*, string);
	void summand(TreeNode*, string);
	void multiplier(TreeNode*, string);
	void multipliers_list(TreeNode*, string);
	void unsigned_integer(TreeNode*, string);


};
