#pragma once

#include "symbolTable.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <regex>

class LexicalAnalyzer {
private:
	SymbolTable st;
	std::vector<std::pair<std::string, std::string>> pif; 
	std::vector<std::string> delimiters, operators, keywords;
	std::string tokensInFile, programInFile, programOutPath; 

	/*
		We use this function to detect tokens based on a list of separators.
		This function is used:
			- when detecting the actual tokens from the tokens.in file. In this case the list of separators passed is controlled by me
			- when detecting the tokens in the program. In this case the list of separators is the reunion between the delimiters and the operators taken from tokens.in
	*/ 
	std::vector<std::string> parseInLine(std::string line, std::vector<std::string> separatorList, bool addSeparator);

	/*
		Function used to identify the delimiters, operators and keywords from tokens.in
	*/
	void parsePredefinedTokens();

	/*
		Function used to check if a token is a valid identifier
	*/
	bool isIdentifier(std::string token);

	/*
		Function used to check if a token is a valid constant
	*/
	enum ConstantType {
		CONSTANT_INVALID = 0,
		CONSTANT_INTEGER,
		CONSTANT_CHAR,
		CONSTANT_BOOL,
		CONSTANT_FLOAT
	};
	ConstantType isConstant(std::string token);

public:
	LexicalAnalyzer(std::string p_tokensInFile, std::string p_programInFile, std::string p_programOutPath);
	void parseProgram();
};