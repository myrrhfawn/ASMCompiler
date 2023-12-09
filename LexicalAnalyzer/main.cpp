#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Data.h"
#include <fstream>

int main(int argc, char* argv[])
{
	// таблиц€ лесем
	//Token TokenTable[MAX_TOKENS];

	Token* TokenTable = new Token[MAX_TOKENS];

	// к≥льк≥сть лексем
	unsigned int TokensNum;
	// таблиц€ ≥дентиф≥катор≥в
	Id IdTable[MAX_IDENTIFIER];
	// к≥льк≥сть ≥дентиф≥катор≥в
	unsigned int IdNum;

	char InputFile[32] = "test3.h06";
	FILE* InFile;

	printf("---------------------------------------------------------------------------\n");
	printf("|         TRANSLATOR ( M -> ASSEMBLER )                                   |\n");
	printf("---------------------------------------------------------------------------\n\n");


	/*if (argc != 2)
	{
		printf("Input file name: ");
		gets_s(InputFile);
	}
	else
	{
		strcpy_s(InputFile, argv[1]);
	}*/

	

	std::ifstream inputFile(InputFile);
	if (!inputFile.is_open()) {
		printf("Cannot open file %s", InputFile);
		return 0;
	}

	TokensNum = GetTokens(inputFile, TokenTable);

	char TokenFile[32];
	

	int i = 0;
	while (InputFile[i] != '.')
	{
		TokenFile[i] = InputFile[i];
		i++;
	}
	TokenFile[i] = '\0';
	strcat_s(TokenFile, ".token");

	PrintTokensToFile(TokenFile, TokenTable, TokensNum);
	
	printf("\nLexical analysis completed. List of tokens in the file %s\n", TokenFile);
	PrintTokens(TokenTable, TokensNum);


	

	SyntaxAnalyzer syntax(TokenTable, TokensNum);
	syntax.analyzeSyntax();

	delete[]TokenTable;
	return 0;
}