#pragma once
#include "parser.h"
#include <string>


class generator {
private:
	TreeNode* node;
	string fileName;
	string programName;
	map <string, string> variables;
	int expressionDepth = 0;
public:
	generator(string inputFile, TreeNode* node);

	void generateStatements(TreeNode* node);
	void declaration_list(TreeNode* node, int counter);
	void statement(TreeNode* node);
	string generateExpression(TreeNode* node, string reg);
	string summandList(TreeNode* node,string reg);
	string summandList(TreeNode* node);
	string summand(TreeNode* node);
	string multiplier(TreeNode* node);
	string multiplierList(TreeNode* node);
	string multiplierList(TreeNode* node, string reg);
};