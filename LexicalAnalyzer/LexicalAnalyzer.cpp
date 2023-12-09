#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Data.h"
#include <regex>
#include <iostream>
#include <fstream>
#include <sstream>

std::map<std::string, TypeOfTokens> tokenMap = {
	{"Program", StartProgram},
	{"Start", VarStart},
	{"Var", Variable},
	{"Int_4", Type},
	{"Finish", EndProgram},
	{"Scan", Input},
	{"Print", Output},
	{"If", If},
	{"then", Then},
	{"Else", Else},
	{"While", While},
	{"Do", Do},
	{"End", End},
	{"Repeat", Repeat},
	{"Until", Until},
	{"For", For},
	{"Downto", Downto},
	{"To", To},
	{"Goto", Goto},
	{"<-", Assign},
	{"++", Add},
	{"--", Sub},
	{"**", Mul},
	{"Div", Div},
	{"Mod", Mod},
	{"Eg", Equality},
	{"Ne", NotEquality},
	{"Le", Greate},
	{"Ge", Less},
	{"Not", Not},
	{"And", And},
	{"Or", Or},
	{"(", LBraket},
	{")", RBraket},
	{";", Semicolon},
	{",", Comma},
	{"??", Comment},
	{"unknown", Unknown}
};

// функція отримує лексеми з вхідного файлу F і записує їх у таблицю лексем TokenTable 
// результат функції - кількість лексем
unsigned int GetTokens(std::ifstream &inputFile, Token TokenTable[])
{
	States state = Start;
	Token TempToken;
	// кількість лексем
	unsigned int NumberOfTokens = 0;
	char ch, buf[16];
	int line = 1;
	// читання символів з файлу і пошук лексем
	//ch = getc(F);
	std::string lineText;

	while (std::getline(inputFile, lineText))
	{
		std::istringstream lineStream(lineText);
		std::string token;
		if (!lineText.empty() && lineText[0] == '\t') {
			for (int i = 0; i < lineText.length(); i++) {
				if (lineText[i] == '\t') {
					TempToken.name = "Tab";
					TempToken.type = Tab;
					TempToken.value = 0;
					TempToken.line = line;
					TokenTable[NumberOfTokens++] = TempToken;
				}else {
					break;
				}
			}
		}
		while (lineStream >> token) {
			bool isWorking = true;
			while (isWorking) {
				switch (state)
				{
					// початок виділення чергової лексеми 
					// якщо поточний символ маленька літера, то переходимо до стану Letter
					// якщо поточний символ цифра, то переходимо до стану Digit
					// якщо поточний символ пробіл, символ табуляції або переходу на новий рядок, то переходимо до стану Separators
					// якщо поточний символ відмінний від попередніх, то переходимо до стану Another
				case Start:
				{	
					std::regex keywordRegex("\\b(?:[A-Z][a-z])\\b");
					std::regex identifierRegex("\\b(?:[a-z][A-Z]{0,3})\\b");
					std::regex digitRegex("[+-]?[0-9]+\\b");
					std::smatch match;

					if (std::regex_search(token, match, identifierRegex)) {
						token = match.str(0);
						state = Letter;
					}
					else if (std::regex_search(token, match, keywordRegex)) {
						token = match.str(0);
						state = Keyword;
					}
					else if (std::regex_search(token, match, digitRegex)) {
						token = match.str(0);
						state = Digit;
					}
					
					else {
						state = Another;
					}

					continue;
				}
				// кінець виділення чергової лексеми 
				// запис лексеми у таблицю лексем
				case Finish:
				{	

					if (NumberOfTokens < MAX_TOKENS)
					{
						TokenTable[NumberOfTokens++] = TempToken;
						state = Start;
					}
					else
					{
						printf("\n\t\t\ttoo many tokens !!!\n");
						return NumberOfTokens - 1;
					}
					isWorking = false;
					continue;
				}
				// поточний символ - маленька літера, поточна лексема - ключове слово або ідентифікатор
				case Keyword:
				{
					TypeOfTokens temp_type = Unknown;
					auto it = tokenMap.find(token);
					if (it != tokenMap.end()) {
						// Знайдено значення
						temp_type = it->second;
					}
					TempToken.name = token;
					TempToken.type = temp_type;
					TempToken.value = 0;
					TempToken.line = line;
					state = Finish;
					continue;
				}
				case Letter:
				{
					TempToken.name = token;
					TempToken.type = Identifier;
					TempToken.value = 0;
					TempToken.line = line;
					state = Finish;
					continue;
				}

				// поточний символ - цифра, поточна лексема - число
				case Digit:
				{
					
					TempToken.name = token;
					TempToken.type = Number;
					TempToken.value = stoi(token);
					TempToken.line = line;
					state = Finish;
					continue;
				}
				// поточний символ - символ, відмінний від попередніх
				case Another:
				{	

					TypeOfTokens temp_type = Unknown;
					auto it = tokenMap.find(token);
					if (it != tokenMap.end()) {
						// Знайдено значення
						temp_type = it->second;
					}
					TempToken.name = token;
					TempToken.type = temp_type;
					TempToken.value = 0;
					TempToken.line = line;
					state = Finish;
					continue;
				}
				}
			}
		}
		line++;
	}
	return NumberOfTokens;
}


void PrintTokens(Token TokenTable[], unsigned int TokensNum)
{
	char type_tokens[16];
	printf("\n\n---------------------------------------------------------------------------\n");
	printf("|             TOKEN TABLE                                                 |\n");
	printf("---------------------------------------------------------------------------\n");
	printf("| line number |      token      |    value   | token code | type of token |\n");
	printf("---------------------------------------------------------------------------");
	for (unsigned int i = 0; i < TokensNum; i++)
	{
		switch (TokenTable[i].type)
		{
		case StartProgram: 
			strcpy_s(type_tokens, "StartProgram"); 
			break;
		case VarStart:
			strcpy_s(type_tokens, "Start");
			break;
		case Variable:
			strcpy_s(type_tokens, "Variable"); 
			break;
		case Type:
			strcpy_s(type_tokens, "Type");
			break;
		case Identifier: 
			strcpy_s(type_tokens, "Identifier"); 
			break;
		case EndProgram: 
			strcpy_s(type_tokens, "EndProgram"); 
			break;
		case Input: 
			strcpy_s(type_tokens, "Input"); 
			break;
		case Output: 
			strcpy_s(type_tokens, "Output"); 
			break;
		case If: 
			strcpy_s(type_tokens, "If"); 
			break;
		case Then: 
			strcpy_s(type_tokens, "Then"); 
			break;
		case Else: 
			strcpy_s(type_tokens, "Else"); 
			break;
		case While:
			strcpy_s(type_tokens, "While");
			break;
		case Do:
			strcpy_s(type_tokens, "Do");
			break;
		case End:
			strcpy_s(type_tokens, "End");
			break;
		case Repeat:
			strcpy_s(type_tokens, "Repeat");
			break;
		case Until:
			strcpy_s(type_tokens, "Until");
			break;
		case Downto:
			strcpy_s(type_tokens, "Downto");
			break;
		case For:
			strcpy_s(type_tokens, "For");
			break;
		case To:
			strcpy_s(type_tokens, "To");
			break;
		case Goto:
			strcpy_s(type_tokens, "Goto");
			break;
		case Assign: 
			strcpy_s(type_tokens, "Assign"); 
			break;
		case Add: 
			strcpy_s(type_tokens, "Add"); 
			break;
		case Sub: 
			strcpy_s(type_tokens, "Sub"); 
			break;
		case Mul: 
			strcpy_s(type_tokens, "Mul"); 
			break;
		case Div: 
			strcpy_s(type_tokens, "Div"); 
			break;
		case Mod:
			strcpy_s(type_tokens, "Mod");
			break;
		case Equality: 
			strcpy_s(type_tokens, "Equality"); 
			break;
		case NotEquality: 
			strcpy_s(type_tokens, "NotEquality"); 
			break;
		case Greate: 
			strcpy_s(type_tokens, "Greate"); 
			break;
		case Less: 
			strcpy_s(type_tokens, "Less"); 
			break;
		case Not: 
			strcpy_s(type_tokens, "Not"); 
			break;
		case And: 
			strcpy_s(type_tokens, "And"); 
			break;
		case Or: 
			strcpy_s(type_tokens, "Or"); 
			break;
		case LBraket: 
			strcpy_s(type_tokens, "LBraket"); 
			break;
		case RBraket: 
			strcpy_s(type_tokens, "RBraket"); 
			break;
		case Number: 
			strcpy_s(type_tokens, "Number"); 
			break;
		case Semicolon:
			strcpy_s(type_tokens, "Semicolon"); 
			break;
		case Comma: 
			strcpy_s(type_tokens, "Comma"); 
			break;
		case Comment:
			strcpy_s(type_tokens, "Comment");
			break;
		case Unknown: 
			strcpy_s(type_tokens, "Unknown"); 
			break;
		case Tab:
			strcpy_s(type_tokens, "Tab");
			break;
		}
		
		printf("\n|%12d |%16s |%11d |%11d | %-13s |\n", 
			TokenTable[i].line,
			TokenTable[i].name.c_str(),
			TokenTable[i].value, 
			TokenTable[i].type, 
			type_tokens);
		printf("---------------------------------------------------------------------------");
	}
}

void PrintTokensToFile(char * FileName, Token TokenTable[], unsigned int TokensNum)
{
	FILE* F;
	if ((fopen_s(&F, FileName, "wt")) != 0)
	{
		printf("Error: Can not create file: %s\n", FileName);
		return ;
	}
	char type_tokens[16];
	fprintf(F,"---------------------------------------------------------------------------\n");
	fprintf(F, "|             TOKEN TABLE                                                 |\n");
	fprintf(F, "---------------------------------------------------------------------------\n");
	fprintf(F, "| line number |      token      |    value   | token code | type of token |\n");
	fprintf(F, "---------------------------------------------------------------------------");
	for (unsigned int i = 0; i < TokensNum; i++)
	{
		switch (TokenTable[i].type)
		{
		case StartProgram:
			strcpy_s(type_tokens, "StartProgram");
			break;
		case VarStart:
			strcpy_s(type_tokens, "Start");
			break;
		case Variable:
			strcpy_s(type_tokens, "Variable");
			break;
		case Identifier:
			strcpy_s(type_tokens, "Identifier");
			break;
		case EndProgram:
			strcpy_s(type_tokens, "EndProgram");
			break;
		case Input:
			strcpy_s(type_tokens, "Input");
			break;
		case Output:
			strcpy_s(type_tokens, "Output");
			break;
		case If:
			strcpy_s(type_tokens, "If");
			break;
		case Then:
			strcpy_s(type_tokens, "Then");
			break;
		case Else:
			strcpy_s(type_tokens, "Else");
			break;
		case While:
			strcpy_s(type_tokens, "While");
			break;
		case Do:
			strcpy_s(type_tokens, "Do");
			break;
		case End:
			strcpy_s(type_tokens, "End");
			break;
		case Repeat:
			strcpy_s(type_tokens, "Repeat");
			break;
		case Until:
			strcpy_s(type_tokens, "Until");
			break;
		case Downto:
			strcpy_s(type_tokens, "Downto");
			break;
		case For:
			strcpy_s(type_tokens, "For");
			break;
		case To:
			strcpy_s(type_tokens, "To");
			break;
		case Goto:
			strcpy_s(type_tokens, "Goto");
			break;
		case Assign:
			strcpy_s(type_tokens, "Assign");
			break;
		case Add:
			strcpy_s(type_tokens, "Add");
			break;
		case Sub:
			strcpy_s(type_tokens, "Sub");
			break;
		case Mul:
			strcpy_s(type_tokens, "Mul");
			break;
		case Div:
			strcpy_s(type_tokens, "Div");
			break;
		case Mod:
			strcpy_s(type_tokens, "Mod");
			break;
		case Equality:
			strcpy_s(type_tokens, "Equality");
			break;
		case NotEquality:
			strcpy_s(type_tokens, "NotEquality");
			break;
		case Greate:
			strcpy_s(type_tokens, "Greate");
			break;
		case Less:
			strcpy_s(type_tokens, "Less");
			break;
		case Not:
			strcpy_s(type_tokens, "Not");
			break;
		case And:
			strcpy_s(type_tokens, "And");
			break;
		case Or:
			strcpy_s(type_tokens, "Or");
			break;
		case LBraket:
			strcpy_s(type_tokens, "LBraket");
			break;
		case RBraket:
			strcpy_s(type_tokens, "RBraket");
			break;
		case Number:
			strcpy_s(type_tokens, "Number");
			break;
		case Semicolon:
			strcpy_s(type_tokens, "Semicolon");
			break;
		case Comma:
			strcpy_s(type_tokens, "Comma");
			break;
		case Comment:
			strcpy_s(type_tokens, "Comment");
			break;
		case Unknown:
			strcpy_s(type_tokens, "Unknown");
			break;
		}

		fprintf(F, "\n|%12d |%16s |%11d |%11d | %-13s |\n",
			TokenTable[i].line,
			TokenTable[i].name,
			TokenTable[i].value,
			TokenTable[i].type,
			type_tokens);
		fprintf(F, "---------------------------------------------------------------------------");
	}
	fclose(F);
}
