#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "stdlib.h"
#include <string>
#include <iomanip>
#include "lexer.h"
#include "parser.h"
using namespace std;


int main(int argc, char** argv)
{
	
	for (int i = 1; i < argc; i++) {
		Lexer lexer;
		
		lexer = Lexer(argv[i]);
		Parser parser = Parser(argv[i], lexer.parse());
		TreeNode* node = parser.parse();
		
	}
}
