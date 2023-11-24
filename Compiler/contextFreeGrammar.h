#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>

class ContextFreeGrammar {
protected: 
	std::vector<std::string> nonterminals;
	std::vector<std::string> terminals;
	std::string startingSymbol; 
	std::unordered_map<std::string, std::vector<std::vector<std::string>>> productions;

	std::vector<std::string> separateByDelimiter(std::string line, char delimiter);
	void buildProductions(std::vector<std::string>& unparsedProductions);

	void printMenu();
	void printNonterminals();
	void printTerminals();
	void printProductions();
	void printStartingSymbol();
	void checkIfValid();

	bool isNonterminal(std::string potentialNonterminal);
	bool isTerminal(std::string potentialTerminal);

public: 
	ContextFreeGrammar(std::string inFile);
	void parseFile(std::string inFile);
	void run();
};