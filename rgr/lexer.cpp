#include "lexer.h"

int SymbolCategories[255];
int tabulation = 4;
int unsintegers = 501;
int identifiers = 1001;
void fillAscii() {
	for (int i = 0; i < 255; i++) {

		if (i >= 8 && i <= 13 || i == 32) {
			SymbolCategories[i] = 0;
			continue;
		}

		if (i >= 48 && i <= 57) {
			SymbolCategories[i] = 1;
			continue;
		}

		if (i >= 65 && i <= 90) {
			SymbolCategories[i] = 2;
			continue;
		}

		if ((i >= 40 && i <= 47) || (i >= 59 && i <= 62)) {
			if (i == 40) { SymbolCategories[i] = 5; }
			else { SymbolCategories[i] = 3; }
			continue;
		}

		if (i == 58) {
			SymbolCategories[i] = 4;
			continue;
		}

		if ((i >= 0 && i <= 5) || i >= 91 || (i >= 62 && i <= 64) || i == 60 || (i >= 33 && i <= 39) || i == 44) {
			SymbolCategories[i] = 6;
			continue;
		}

	}
	/*
	cout << "id\t group\t symb" << endl;
	for (int i = 0; i < 255; i++) {
		cout << i << "| \t" << SymbolCategories[i] << "| \t" << (char)i << endl;
	}*/
}

void showLexeme(int row, int column, string lexeme, int ascii, string inputFile) {

	string tmp = "./" + inputFile + "/generated.txt";
	fstream output;

	output.open(tmp, fstream::in | fstream::out | fstream::app);
	output << setw(10) << row << setw(5) << " " << column << setw(5) << " " << ascii << setw(5) << " " << lexeme << endl;
	//cout << setw(10) << lexeme << setw(5) << " " << row << setw(5) << " " << column << setw(5) << " " << ascii << endl;
	output.close();



}
void showError(string error, string inputFile) {
	string tmp = "./" + inputFile + "/generated.txt";
	fstream output;

	output.open(tmp, fstream::in | fstream::out | fstream::app);
	output << error << endl;
}

Lexer::Lexer(string inputFile) {
	string tmp = "./" + inputFile + "/input.sig";
	strcpy(filePath, tmp.c_str());
	cout << filePath << endl;
	fileName = inputFile;
	input = ifstream(filePath);


}

void clearFile(string inputFile) {
	string tmp = "./" + inputFile + "/generated.txt";
	fstream output;
	output.open(tmp, fstream::in | fstream::out | fstream::trunc);
	output.close();
}


vector<Lexeme> Lexer::parse() {
	vector<Lexeme> lexemeList;
	clearFile(fileName);
	fillAscii();
	int row = 1;
	int col = 1;
	char buff;

	while (1) {
		input.get(buff);
		if (input.eof()) { break; }
		switch (SymbolCategories[(int)buff])
		{
		case 0://пробелы и символы приравненные к пробелам
		{
			switch ((int)buff)
			{
			case 10: {
				row++;
				col = 1;
				break;
			}
			case 9: {
				col += tabulation;
				break;
			}
			case 32: {
				col++;
				break;
			}
			}
			break;
		}
		case 1: {//числовые костанты
			int position = col;
			string token;
			do {

				if (SymbolCategories[(int)buff] == 2) {
					string error = "Lexer: ERROR! Detected letter after number in row:" + to_string(row) + " col:" + to_string(col);
					showError(error, fileName);
				}
				token += buff;
				col++;

			} while (input.get(buff) && SymbolCategories[(int)buff] == 1 || SymbolCategories[(int)buff] == 2);

			input.seekg(-1, ios_base::cur);
			auto type = lexemes[token];


			if (!type) {

				lexemes[token] = unsintegers;
				Lexeme newLexeme;
				newLexeme.row = row;
				newLexeme.col = position;
				newLexeme.category = unsintegers++;
				newLexeme.token = token;
				lexemeList.push_back(newLexeme);
				//showLexeme(row, position, token, unsintegers++, fileName);
			}
			else {
				//showLexeme(row, position, token, type, fileName);
				Lexeme newLexeme;
				newLexeme.row = row;
				newLexeme.col = position;
				newLexeme.category = type;
				newLexeme.token = token;
				lexemeList.push_back(newLexeme);
			}

			break;
		}

		case 2: {
			int position = col;
			string token;
			do {
				token += buff;
				col++;

			} while (input.get(buff) && SymbolCategories[(int)buff] == 1 || SymbolCategories[(int)buff] == 2);
			input.seekg(-1, ios_base::cur);
			auto type = lexemes[token];


			if (!type) {//не найдено существующей лексемы в списке, добавляем новую в список
				lexemes[token] = identifiers;
				//showLexeme(row, position, token, identifiers++, fileName);
				Lexeme newLexeme;
				newLexeme.row = row;
				newLexeme.col = position;
				newLexeme.category = identifiers++;
				newLexeme.token = token;
				lexemeList.push_back(newLexeme);
			}
			else {

				//showLexeme(row, position, token, type, fileName);
				Lexeme newLexeme;
				newLexeme.row = row;
				newLexeme.col = position;
				newLexeme.category = type;
				newLexeme.token = token;
				lexemeList.push_back(newLexeme);
			}
			break;

		}
		case 3: {
			//showLexeme(row, col, string{ buff }, (int)buff, fileName);
			Lexeme newLexeme;
			newLexeme.row = row;
			newLexeme.col = col;
			newLexeme.category = (int)buff;
			newLexeme.token = string{ buff };
			lexemeList.push_back(newLexeme);
			col++;
			break;
		}
		case 4: {
			string token = string{ buff };
			int position = col;
			col++;
			if (input.get(buff) && buff == '=') {
				token += string{ buff };
				col++;
				//showLexeme(row, position, token, 301, fileName);
				Lexeme newLexeme;
				newLexeme.row = row;
				newLexeme.col = position;
				newLexeme.category = 301;
				newLexeme.token = token;
				lexemeList.push_back(newLexeme);
			}
			else {
				input.seekg(-1, ios_base::cur);

				//showLexeme(row, position, token, (int)token[0], fileName);
				Lexeme newLexeme;
				newLexeme.row = row;
				newLexeme.col = position;
				newLexeme.category = (int)token[0];
				newLexeme.token = token;
				lexemeList.push_back(newLexeme);
			}

			break;
		}
		case 5: {
			col++;
			if (input.get(buff) && buff == '*') {
				int startComRow = row;
				int startComCol = col;
				col++;
				while (1) {
					while (input.get(buff) && buff != '*') //читаем комментарий пока не найдем звездочку
					{
						if (input.eof()) { showError("Lexer: ERROR! Comment on row:" + to_string(startComRow) + " col:" + to_string(startComCol) + " - unexpected end of file in unclosed comment! ", fileName); }

						if ((int)buff == 10) {
							row++;
							col = 1;
						}
						else if ((int)buff == 9) { col += tabulation; }
						else {
							col++;
						}

					}
					if (input.eof()) { showError("Lexer: ERROR! Comment on row:" + to_string(startComRow) + " col:" + to_string(startComCol) + " - unexpected end of file in unclosed comment! ", fileName); }

					col++;//нашли звездочку, инкремент по столбику
					if (input.get(buff) && buff != ')') {

						//если не закрывающая скобка, комментарий не закрываем, проверяем на следующую звездочку, если и там не звездочка то продолжаем читать комментарий до звездочки
						if (buff != '*') {
							input.seekg(-1, ios_base::cur);
							continue;
						}
						else {
							//если всё таки звездочка, продолжаем считывать звездочки
							col++;
							while (input.get(buff) && buff == '*')
							{
								col++;
								if (input.eof()) { showError("Lexer: ERROR! Comment on row:" + to_string(startComRow) + " col:" + to_string(startComCol) + " - unexpected end of file in unclosed comment! ", fileName); }
							}
							if (input.eof()) { showError("Lexer: ERROR! Comment on row:" + to_string(startComRow) + " col:" + to_string(startComCol) + " - unexpected end of file in unclosed comment! ", fileName); }

							if (buff == ')') { col++; break; }//после ряда звездочек нашли скобку, end comment
							else { input.seekg(-1, ios_base::cur);  continue; }//не скобка - сдвигаем каретку назад и читаем символы


						}
					}
					else {
						col++;
						//если закрывающая скобка после звездочки, закончили комментарий
						break;
					}

				}
			}
			else {
				input.seekg(-1, ios_base::cur);
				//showLexeme(row, col, string{ '(' }, (int)'(', fileName);
				Lexeme newLexeme;
				newLexeme.row = row;
				newLexeme.col = col;
				newLexeme.category = (int)'(';
				newLexeme.token = string{ '(' };
				lexemeList.push_back(newLexeme);

			}
			break;
		}
		case 6: {
			showError("Lexer: ERROR! Detected illegal symbol " + string{ buff } + " at col:" + to_string(col) + " row: " + to_string(row), fileName);
		}

		}


	}
	/*
	for (int i = 0; i < lexemeList.size(); i++) {
		cout << setw(10) << lexemeList[i].row << setw(5) << " " << lexemeList[i].col << setw(5) << " " << lexemeList[i].category << setw(5) << " " << lexemeList[i].token << endl;
	}*/

	return lexemeList;



}