#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "stdlib.h"
#include <string>
#include <iomanip>
#include "parser.h"
using namespace std;


int main(int argc,char **argv)
{
	for (int i = 1; i < argc; i++) {
		 Parser parser;
		 parser = Parser(argv[i]);
		 parser.parse();
	}
}
