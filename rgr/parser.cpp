#include "parser.h"

void generateData(string data, string fileName) {
	string tmp = "./" + fileName + "/generated.txt";
	fstream output;

	output.open(tmp, fstream::in | fstream::out | fstream::app);
	output << data << endl;
}

void generateError(string error, string fileName) {
	string tmp = "./" + fileName + "/generated.txt";
	fstream output;

	output.open(tmp, fstream::in | fstream::out | fstream::app);
	output << error << endl;
}


Parser::Parser(string file, vector<Lexeme> input) {
	this->fileName = file;
	this->lexemes = input;
}

TreeNode* Parser::parse() {
	TreeNode* node = new TreeNode;
	string depth = "";
	node->keyword = "<signal-program>";
	generateData(depth + node->keyword, this->fileName);
	program(node, depth + "..");
	//cout<<node->next[0]->next[0]->next[0]->keyword<<endl;
	return node;
}

void Parser::program(TreeNode* parent, string depth) {
	TreeNode* node = new TreeNode;
	node->keyword = "<program>";
	generateData(depth + node->keyword, this->fileName);
	if (this->lexemes.size() <= this->i) {
		generateError("Syntax-analyzer! ERROR! Unexpected end of file", this->fileName);
		parent->next.push_back(node);
		return;
	}

	if (this->lexemes[this->i].category != 401) {
		generateError("Syntax-analyzer! ERROR! 'PROGRAM' identifier not found at row:" + to_string(this->lexemes[this->i].row) + "col:" + to_string(this->lexemes[this->i].col), this->fileName);
		parent->next.push_back(node);
		return;
	}

	node->lexemes.push_back(&this->lexemes[this->i]);
	generateData(depth + ".." + node->lexemes[0]->token + "  " + to_string(node->lexemes[0]->category), this->fileName);
	this->i++;

	procedure_identifier(node, depth + "..");

	if (this->lexemes.size() <= this->i) {
		generateError("Syntax-analyzer! ERROR! Unexpected end of file", this->fileName);
		parent->next.push_back(node);
		return;
	}

	if (this->lexemes[this->i].category != 59) {
		generateError("Syntax-analyzer! ERROR! semicolon(;) not found at row:" + to_string(this->lexemes[this->i].row) + "col:" + to_string(this->lexemes[this->i].col), this->fileName);
		parent->next.push_back(node);

		return;
	}

	node->lexemes.push_back(&this->lexemes[this->i]);
	generateData(depth + ".." + node->lexemes[1]->token + "  " + to_string(node->lexemes[0]->category), this->fileName);
	this->i++;

	block(node, depth + "..");

	if (this->lexemes.size() <= this->i) {
		generateError("Syntax-analyzer! ERROR! Unexpected end of file", this->fileName);
		parent->next.push_back(node);
		return;
	}

	if (this->lexemes[this->i].category != 46) {
		generateError("Syntax-analyzer! ERROR! '.' not found at row:" + to_string(this->lexemes[this->i].row) + "col:" + to_string(this->lexemes[this->i].col), this->fileName);
		parent->next.push_back(node);

		return;
	}

	node->lexemes.push_back(&this->lexemes[this->i]);
	generateData(depth + ".. " + node->lexemes[2]->token + "  " + to_string(node->lexemes[2]->category), this->fileName);
	this->i++;

	parent->next.push_back(node);
}
void Parser::procedure_identifier(TreeNode* parent, string depth) {
	TreeNode* node = new TreeNode;
	node->keyword = "<procedure-identifier>";
	generateData(depth + node->keyword, this->fileName);
	identifier(node, depth + "..");
	parent->next.push_back(node);


}

void Parser::identifier(TreeNode* parent, string depth) {
	TreeNode* node = new TreeNode;
	node->keyword = "<identifier>";
	generateData(depth + node->keyword, this->fileName);
	if (this->lexemes[this->i].category >= 1001) {
		node->lexemes.push_back(&this->lexemes[this->i]);
		generateData(depth + ".." + node->lexemes[0]->token + "  " + to_string(node->lexemes[0]->category), this->fileName);
	}
	else {
		generateError("Syntax-analyzer! ERROR! identifier not found at row:" + to_string(this->lexemes[this->i].row) + "col:" + to_string(this->lexemes[this->i].col), this->fileName);
	}
	this->i++;

	parent->next.push_back(node);

}
void Parser::block(TreeNode* parent, string depth) {
	TreeNode* node = new TreeNode;
	node->keyword = "<block>";
	generateData(depth + node->keyword, this->fileName);
	variable_declarations(node, depth + "..");
	if (this->lexemes[this->i].category != 402) {
		generateError("Syntax-analyzer! ERROR! 'BEGIN' identifier not found  at row:" + to_string(this->lexemes[this->i].row) + "col:" + to_string(this->lexemes[this->i].col), this->fileName);
		parent->next.push_back(node);
		return;
	}
	else {
		node->lexemes.push_back(&this->lexemes[this->i]);
		generateData(depth + ".." + node->lexemes[0]->token + "  " + to_string(node->lexemes[0]->category), this->fileName);
		this->i++;
		statements_list(node, depth + "..");

		if (this->lexemes.size() <= this->i) { return; }
		if (this->lexemes[this->i].category != 403) {
			generateError("Syntax-analyzer! ERROR! 'END' identifier not found  at row:" + to_string(this->lexemes[this->i].row) + "col:" + to_string(this->lexemes[this->i].col), this->fileName);
			parent->next.push_back(node);
			return;
		}
		node->lexemes.push_back(&this->lexemes[this->i]);
		generateData(depth + ".." + node->lexemes[1]->token + "  " + to_string(node->lexemes[1]->category), this->fileName);
		this->i++;
		parent->next.push_back(node);
	}


}

void Parser::variable_declarations(TreeNode* parent, string depth) {
	TreeNode* node = new TreeNode;
	node->keyword = "<variable-declarations>";
	generateData(depth + node->keyword, this->fileName);
	if (this->lexemes[this->i].category != 404 && this->lexemes[this->i].category != 402) {
		generateError("Syntax-analyzer! ERROR! 'VAR' identifier not found and unexpected token at row:" + to_string(this->lexemes[this->i].row) + "col:" + to_string(this->lexemes[this->i].col), this->fileName);

		parent->next.push_back(node);
		return;
	}
	else if (this->lexemes[this->i].category == 402) {
		empty(node, depth + "..");
		parent->next.push_back(node);
		return;
	}
	else {
		node->lexemes.push_back(&this->lexemes[this->i]);
		generateData(depth + ".." + node->lexemes[0]->token + "  " + to_string(node->lexemes[0]->category), this->fileName);
		this->i++;
		declaration_list(node, depth + "..");
		parent->next.push_back(node);
	}
}

void Parser::empty(TreeNode* parent, string depth) {
	TreeNode* node = new TreeNode;
	node->keyword = "<empty>";
	generateData(depth + node->keyword, this->fileName);
	parent->next.push_back(node);
}


void Parser::declaration_list(TreeNode* parent, string depth) {
	TreeNode* node = new TreeNode;
	node->keyword = "<declaration-list>";
	generateData(depth + node->keyword, this->fileName);

	if (this->lexemes[this->i].category < 1001) {
		empty(node, depth + "..");
		parent->next.push_back(node);
		return;
	}
	else {
		declaration(node, depth + "..");
		declaration_list(node, depth + "..");
	}



	parent->next.push_back(node);

}

void Parser::declaration(TreeNode* parent, string depth) {
	TreeNode* node = new TreeNode;
	node->keyword = "<declaration>";
	generateData(depth + node->keyword, this->fileName);
	variable_identifier(node, depth + "..");
	if (this->lexemes[this->i].category != 58) {
		generateError("Syntax-analyzer! ERROR! ':' not found and unexpected token at row:" + to_string(this->lexemes[this->i].row) + "col:" + to_string(this->lexemes[this->i].col), this->fileName);
		parent->next.push_back(node);
		return;
	}


	node->lexemes.push_back(&this->lexemes[this->i]);
	generateData(depth + ".." + node->lexemes[0]->token + "  " + to_string(node->lexemes[0]->category), this->fileName);
	this->i++;

	if (this->lexemes[this->i].category != 405) {
		generateError("Syntax-analyzer! ERROR! 'INTEGER' not found and unexpected token at row:" + to_string(this->lexemes[this->i].row) + "col:" + to_string(this->lexemes[this->i].col), this->fileName);
		parent->next.push_back(node);
		return;
	}

	node->lexemes.push_back(&this->lexemes[this->i]);
	generateData(depth + ".." + node->lexemes[1]->token + "  " + to_string(node->lexemes[1]->category), this->fileName);
	this->i++;

	if (this->lexemes[this->i].category != 59) {
		generateError("Syntax-analyzer! ERROR! ';'  not found and unexpected token at row:" + to_string(this->lexemes[this->i].row) + "col:" + to_string(this->lexemes[this->i].col), this->fileName);
		parent->next.push_back(node);
		return;
	}

	node->lexemes.push_back(&this->lexemes[this->i]);
	generateData(depth + ".." + node->lexemes[2]->token + "  " + to_string(node->lexemes[1]->category), this->fileName);
	this->i++;



	parent->next.push_back(node);

}
void Parser::variable_identifier(TreeNode* parent, string depth) {
	TreeNode* node = new TreeNode;
	node->keyword = "<variable_identifier>";
	generateData(depth + node->keyword, this->fileName);
	identifier(node, depth + "..");
	parent->next.push_back(node);
}


void Parser::statements_list(TreeNode* parent, string depth) {
	TreeNode* node = new TreeNode;
	node->keyword = "<statements_list>";
	generateData(depth + node->keyword, this->fileName);
	if (this->lexemes.size() <= this->i) { return; }
	if (this->lexemes[this->i].category == 403) {
		empty(node, depth + "..");
		return;
	}
	if (this->lexemes[this->i].category < 1001 && this->lexemes[this->i].category != 406 && this->lexemes[this->i].category != 409 && this->lexemes[this->i].category != 59) {
		empty(node, depth + "..");
		parent->next.push_back(node);
		return;
	}
	else {
		statement(node, depth + "..");
		statements_list(node, depth + "..");
	}


	parent->next.push_back(node);
}

void Parser::statement(TreeNode* parent, string depth) {
	TreeNode* node = new TreeNode;
	node->keyword = "<statement>";
	generateData(depth + node->keyword, this->fileName);
	if (this->lexemes[this->i].category >= 1001) {
		variable_identifier(node, depth + "..");
		if (this->lexemes.size() <= this->i) { return; }
		if (this->lexemes[this->i].category != 301) {
			generateError("Syntax-analyzer! ERROR! ':=' not found in statement at row:" + to_string(this->lexemes[this->i].row) + "col:" + to_string(this->lexemes[this->i].col), this->fileName);
			parent->next.push_back(node);
			return;
		}
		node->lexemes.push_back(&this->lexemes[this->i]);
		generateData(depth + ".." + node->lexemes[0]->token + "  " + to_string(node->lexemes[0]->category), this->fileName);
		this->i++;
		expression(node, depth + "..");
		if (this->lexemes.size() <= this->i) { return; }
		if (this->lexemes[this->i].category != 59) {
			generateError("Syntax-analyzer! ERROR! semicolon(;) not found in end of statement at row:" + to_string(this->lexemes[this->i].row) + "col:" + to_string(this->lexemes[this->i].col), this->fileName);
			parent->next.push_back(node);
			return;
		}
		node->lexemes.push_back(&this->lexemes[this->i]);
		generateData(depth + ".." + node->lexemes[1]->token + "  " + to_string(node->lexemes[1]->category), this->fileName);
		this->i++;

		parent->next.push_back(node);
		return;
	}

	if (this->lexemes[this->i].category == 406) {
		node->lexemes.push_back(&this->lexemes[this->i]);
		generateData(depth + ".." + node->lexemes[0]->token + "  " + to_string(node->lexemes[0]->category), this->fileName);
		this->i++;
		procedure_identifier(node, depth + "..");
		if (this->lexemes[this->i].category != 407) {
			generateError("Syntax-analyzer! ERROR! DO not found in end of statement at row:" + to_string(this->lexemes[this->i].row) + "col:" + to_string(this->lexemes[this->i].col), this->fileName);
			parent->next.push_back(node);
			return;
		}
		
		node->lexemes.push_back(&this->lexemes[this->i]);
		generateData(depth + ".." + node->lexemes[1]->token + "  " + to_string(node->lexemes[1]->category), this->fileName);
		this->i++;

		statements_list(node, depth + "..");
		cout << this->lexemes[this->i].token;
		if (this->lexemes[this->i].category != 408) {
			generateError("Syntax-analyzer! ERROR! ENDWhILE not found in end of statement at row:" + to_string(this->lexemes[this->i].row) + "col:" + to_string(this->lexemes[this->i].col), this->fileName);
			parent->next.push_back(node);
			return;
		}
		node->lexemes.push_back(&this->lexemes[this->i]);
		generateData(depth + ".." + node->lexemes[2]->token + "  " + to_string(node->lexemes[2]->category), this->fileName);
		this->i++;
		if (this->lexemes[this->i].category != 59) {
			generateError("Syntax-analyzer! ERROR! ; not found in end of statement at row:" + to_string(this->lexemes[this->i].row) + "col:" + to_string(this->lexemes[this->i].col), this->fileName);
			parent->next.push_back(node);
			return;
		}
		node->lexemes.push_back(&this->lexemes[this->i]);
		generateData(depth + ".." + node->lexemes[3]->token + "  " + to_string(node->lexemes[3]->category), this->fileName);
		this->i++;
		parent->next.push_back(node);
		return;
	}

	if (this->lexemes[this->i].category == 409) {
		node->lexemes.push_back(&this->lexemes[this->i]);
		generateData(depth + ".." + node->lexemes[0]->token + "  " + to_string(node->lexemes[0]->category), this->fileName);
		this->i++;
		node->lexemes.push_back(&this->lexemes[this->i]);
		generateData(depth + ".." + node->lexemes[1]->token + "  " + to_string(node->lexemes[1]->category), this->fileName);
		this->i++;
		if (this->lexemes[this->i].category != 59) {
			generateError("Syntax-analyzer! ERROR! ; not found in end of statement at row:" + to_string(this->lexemes[this->i].row) + "col:" + to_string(this->lexemes[this->i].col), this->fileName);
			parent->next.push_back(node);
			return;
		}
		parent->next.push_back(node);
		return;
	}
	if (this->lexemes[this->i].category == 59) {
		node->lexemes.push_back(&this->lexemes[this->i]);
		generateData(depth + ".." + node->lexemes[0]->token + "  " + to_string(node->lexemes[0]->category), this->fileName);
		this->i++;
	
		parent->next.push_back(node);
		return;
	}

}

void Parser::expression(TreeNode* parent, string depth) {
	TreeNode* node = new TreeNode;
	node->keyword = "<expression>";
	generateData(depth + node->keyword, this->fileName);
	if (this->lexemes.size() <= this->i) { return; }
	if (this->lexemes[this->i].category == 45) {
		node->lexemes.push_back(&this->lexemes[this->i]);
		generateData(depth + ".." + node->lexemes[0]->token + "  " + to_string(node->lexemes[0]->category), this->fileName);
		this->i++;
	}
	summand(node, depth + "..");
	summands_list(node, depth + "..");

	parent->next.push_back(node);

}

void Parser::summand(TreeNode* parent, string depth) {
	TreeNode* node = new TreeNode;
	node->keyword = "<summand>";
	generateData(depth + node->keyword, this->fileName);
	multiplier(node, depth + "..");
	multipliers_list(node, depth + "..");
	parent->next.push_back(node);

}
void Parser::summands_list(TreeNode* parent, string depth) {
	TreeNode* node = new TreeNode;
	node->keyword = "<summands-list>";
	generateData(depth + node->keyword, this->fileName);
	if (this->lexemes.size() <= this->i) { return; }
	if (this->lexemes[this->i].category == 45 || this->lexemes[this->i].category == 43) {
		node->lexemes.push_back(&this->lexemes[this->i]);
		generateData(depth + ".." + node->lexemes[0]->token + "  " + to_string(node->lexemes[0]->category), this->fileName);
		this->i++;
	}
	else {
		/*	generateError("Syntax-analyzer! ERROR! add instruction not found at row:" + to_string(this->lexemes[this->i].row) + "col:" + to_string(this->lexemes[this->i].col), this->fileName);
			parent->next.push_back(node);
			return;*/
		empty(node, depth + "..");
		parent->next.push_back(node);
		return;
	}
	summand(node, depth + "..");


	summands_list(node, depth + "..");

	parent->next.push_back(node);
}

void Parser::multiplier(TreeNode* parent, string depth) {
	TreeNode* node = new TreeNode;
	node->keyword = "<multiplier>";
	generateData(depth + node->keyword, this->fileName);
	if (this->lexemes[this->i].category >= 1001) { variable_identifier(node, depth + ".."); }
	else if (this->lexemes[this->i].category >= 501) {
		unsigned_integer(node, depth + "..");
	}
	else if (this->lexemes[this->i].category == 40) {
		node->lexemes.push_back(&this->lexemes[this->i]);
		generateData(depth + ".." + node->lexemes[0]->token + "  " + to_string(node->lexemes[0]->category), this->fileName);
		this->i++;

		expression(node, depth + "..");

		node->lexemes.push_back(&this->lexemes[this->i]);
		generateData(depth + ".." + node->lexemes[1]->token + "  " + to_string(node->lexemes[1]->category), this->fileName);
		this->i++;
	}
	else {
		generateError("Syntax-analyzer! ERROR! multiplier error at row:" + to_string(this->lexemes[this->i].row) + "col:" + to_string(this->lexemes[this->i].col), this->fileName);
	}

	parent->next.push_back(node);
}

void Parser::multipliers_list(TreeNode* parent, string depth) {
	TreeNode* node = new TreeNode;
	node->keyword = "<multipliers-list>";
	generateData(depth + node->keyword, this->fileName);
	if (this->lexemes.size() <= this->i) { return; }

	if (this->lexemes[this->i].category == 42 || this->lexemes[this->i].category == 47) {
		node->lexemes.push_back(&this->lexemes[this->i]);
		generateData(depth + ".." + node->lexemes[0]->token + "  " + to_string(node->lexemes[0]->category), this->fileName);
		this->i++;
	}
	else {
		empty(node, depth + "..");
		parent->next.push_back(node);
		return;
	}

	multiplier(node, depth + "..");
	if (this->lexemes[this->i].category == 42 || this->lexemes[this->i].category == 47) {
		multipliers_list(node, depth + "..");
	}
	parent->next.push_back(node);
}

void Parser::unsigned_integer(TreeNode* parent, string depth) {
	TreeNode* node = new TreeNode;
	node->keyword = "<unsigned-integer>";
	generateData(depth + node->keyword, this->fileName);
	node->lexemes.push_back(&this->lexemes[this->i]);
	generateData(depth + ".." + node->lexemes[0]->token + "  " + to_string(node->lexemes[0]->category), this->fileName);
	this->i++;
	parent->next.push_back(node);

}
