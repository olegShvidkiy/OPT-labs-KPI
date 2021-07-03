#include "generator.h"

void showData(string data, string fileName) {
	string tmp = "./" + fileName + "/generated.txt";
	fstream output;

	output.open(tmp, fstream::in | fstream::out | fstream::app);
	output << data << endl;
}

void Error(string error, string fileName) {
	string tmp = "./" + fileName + "/generated.txt";
	fstream output;

	output.open(tmp, fstream::in | fstream::out | fstream::app);
	output << error << endl;
}

generator::generator(string inputFile, TreeNode* node) {
	this->node = node;
	this->fileName = inputFile;

	this->programName = this->node->next[0]->next[0]->next[0]->lexemes[0]->token;

	showData("push rbp\nmov rbp, rsp\n", this->fileName);
	if (this->node->next[0]->next[1]->keyword == "<block>") {
		if (this->node->next[0]->next[1]->next[0]->keyword != "<empty>") {
			declaration_list(this->node->next[0]->next[1]->next[0]->next[0], 1);
		}


		if (!this->node->next[0]->next[1]->next[1]->next.empty() && this->node->next[0]->next[1]->next[1]->keyword != "<empty>") {
			generateStatements(this->node->next[0]->next[1]->next[1]);
		}
		showData("pop rbp\nret\n", this->fileName);
	}



}


void generator::declaration_list(TreeNode* node, int counter) {
	TreeNode* declaration = node->next[0];
	if (declaration->next[0]->next[0]->lexemes[0]->token != this->programName) {
		if (variables.find(declaration->next[0]->next[0]->lexemes[0]->token) == variables.end()) {
			variables[declaration->next[0]->next[0]->lexemes[0]->token] = "DWORD PTR[rbp - " + to_string(counter * 4) + "]";
			showData("mov " + variables[declaration->next[0]->next[0]->lexemes[0]->token] + ", 0", this->fileName);
		}
		else {
			Error("GENERATOR: ERROR! variable " + declaration->next[0]->next[0]->lexemes[0]->token + " already declarated", this->fileName);

		}
	}
	else {
		Error("GENERATOR: ERROR! variable " + declaration->next[0]->next[0]->lexemes[0]->token + " has same name as a program identifier", this->fileName);
	}





	if (node->next[1]->next[0]->keyword != "<empty>") {
		declaration_list(node->next[1], counter + 1);
	}
	else {
		showData("\n", this->fileName);
	}
}

void generator::generateStatements(TreeNode* node) {


	statement(node->next[0]);


	if (node->next[1]->next[0]->keyword != "<empty>") {
		generateStatements(node->next[1]);
	}
	else {

		showData("\n", this->fileName);
	}

}

void generator::statement(TreeNode* node) {

	string variableIdentifier = node->next[0]->next[0]->lexemes[0]->token;

	if (node->lexemes.empty() || node->lexemes[0]->token != ":=") {
		//Error("Generator: Error! missing ':=' in statement in row:"+to_string(node->next[0]->next[0]->lexemes[0]->row)+" col:" + to_string(node->next[0]->next[0]->lexemes[0]->col),this->fileName);
		return;
	}
	//generateExpression(node->next[1]);
	if (variableIdentifier == programName) {
		Error("GENERATOR: ERROR! using program name in statement at row: " + to_string(node->next[0]->next[0]->lexemes[0]->row) + " col: " + to_string(node->next[0]->next[0]->lexemes[0]->col), this->fileName);
		return;
	}
	if (variables.find(variableIdentifier) == variables.end()) {
		Error("GENERATOR: ERROR! undeclarated variable at row: " + to_string(node->next[0]->next[0]->lexemes[0]->row) + " col: " + to_string(node->next[0]->next[0]->lexemes[0]->col), this->fileName);
		return;
	}



	showData("mov " + variables[variableIdentifier] + ", " + generateExpression(node->next[1], "ecx") + "\n", this->fileName);
	this->expressionDepth = 0;
}


string generator::generateExpression(TreeNode* node, string reg) {

	if (!node->lexemes.empty() && node->lexemes[0]->token == "-") {
		string result = summand(node->next[0]);
		showData("mov" + reg + ", 0", this->fileName);
		showData("mov ebx, " + result, this->fileName);
		showData("sub " + reg + ", ebx", this->fileName);
		summandList(node->next[1]);
		return reg;
	}
	else {
		string result = summand(node->next[0]);
		if (result != "edx" && result != "ebx") {
			showData("mov " + reg + ", " + result, this->fileName);
			summandList(node->next[1]);
			return reg;
		}

		summandList(node->next[1], result);
		return result;
	}



}

string generator::summand(TreeNode* node) {

	string result = multiplier(node->next[0]);
	//showData("mov eax, " + multiplier(node->next[0]), this->fileName);
	if (node->next[1]->next[0]->keyword != "<empty>") {
		if (result == "edx" || result == "ebx") {
			string buff = multiplierList(node->next[1], result);
			result = buff;
		}
		else {
			showData("mov eax, " + result, this->fileName);
			result = multiplierList(node->next[1]); }

	}

	return  result;
}

string generator::summandList(TreeNode* node) {
	if (!node->lexemes.empty() && node->lexemes[0]->token == "+") {
		showData("add ecx, " + summand(node->next[0]), this->fileName);
		if (!node->next[1]->next.empty() && node->next[1]->next[0]->keyword != "<empty>") {
			summandList(node->next[1]);
		}
		return "";
	}
	else if (!node->lexemes.empty() && node->lexemes[0]->token == "-") {
		showData("sub ecx, " + summand(node->next[0]), this->fileName);

		if (node->next[1]->next[0]->keyword != "<empty>") {
			summandList(node->next[1]);
		}
		return  "";
	}
	else {
		return  "";
	}
}

string generator::summandList(TreeNode* node, string reg) {
	if (!node->lexemes.empty() && node->lexemes[0]->token == "+") {
		showData("add " + reg + ", " + summand(node->next[0]), this->fileName);
		if (!node->next[1]->next.empty() && node->next[1]->next[0]->keyword != "<empty>") {
			summandList(node->next[1], reg);
		}
		return "";
	}
	else if (!node->lexemes.empty() && node->lexemes[0]->token == "-") {
		showData("sub " + reg + ", " + summand(node->next[0]), this->fileName);

		if (node->next[1]->next[0]->keyword != "<empty>") {
			summandList(node->next[1], reg);
		}
		return  "";
	}
	else {
		return  "";
	}
}
string generator::multiplier(TreeNode* node) {
	if (node->next[0]->keyword == "<variable_identifier>") {
		if (variables.find(node->next[0]->next[0]->lexemes[0]->token) == variables.end()) {
			Error("GENERATOR: ERROR! undeclarated variable at row: " + to_string(node->next[0]->next[0]->lexemes[0]->row) + " col: " + to_string(node->next[0]->next[0]->lexemes[0]->col), this->fileName);
			return "ERROR";
		}
		string result = variables[node->next[0]->next[0]->lexemes[0]->token];

		return result;
	}
	else if (node->next[0]->keyword == "<unsigned-integer>") {
		string result = node->next[0]->lexemes[0]->token;
		return result;
	}
	else if (node->next[0]->keyword == "<expression>") {
		this->expressionDepth++;

		string result = generateExpression(node->next[0], "ecx");
		if (this->expressionDepth % 2) {
			showData("mov edx, " + result, this->fileName);
			return "edx";
		}
		else {
			showData("mov ebx, " + result, this->fileName);
			return "ebx";
		}



	}
}
string generator::multiplierList(TreeNode* node) {
	if (!node->lexemes.empty() && node->lexemes[0]->token == "*") {
		showData("imul eax, " + multiplier(node->next[0]), this->fileName);
		if (!node->next[1]->next.empty() && node->next[1]->next[0]->keyword != "<empty>") {
			multiplierList(node->next[1]);
		}
		return "eax";
	}
	else if (!node->lexemes.empty() && node->lexemes[0]->token == "/") {
		showData("mov ecx, " + multiplier(node->next[0]), this->fileName);
		showData("div ecx", this->fileName);
		if (!node->next[1]->next.empty() && node->next[1]->next[0]->keyword != "<empty>") {
			multiplierList(node->next[1]);
		}
		return  "ecx";
	}
	else {
		return  "";
	}
}
string generator::multiplierList(TreeNode* node, string reg) {

	if (!node->lexemes.empty() && node->lexemes[0]->token == "*") {
		showData("imul " + reg + ", " + multiplier(node->next[0]), this->fileName);
		if (!node->next[1]->next.empty() && node->next[1]->next[0]->keyword != "<empty>") {
			multiplierList(node->next[1]);
		}
		return reg;
	}
	else if (!node->lexemes.empty() && node->lexemes[0]->token == "/") {
	
		showData("mov eax, " + multiplier(node->next[0]), this->fileName);
		showData("div "+reg, this->fileName);
		
		if (!node->next[1]->next.empty() && node->next[1]->next[0]->keyword != "<empty>") {
			multiplierList(node->next[1]);
		}
	
		return  reg;
	}
	else {
		return  "";
	}
}