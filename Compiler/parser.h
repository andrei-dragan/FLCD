#pragma once
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>
#include "contextFreeGrammar.h";

class Parser : protected ContextFreeGrammar {
protected:
	struct WorkingStackElement {
		std::string element;
		int index;

		WorkingStackElement(std::string p_element, int p_index);
	};

	struct Configuration {
		std::string state;
		int i;
		std::stack<std::string> inputStack;
		std::stack<WorkingStackElement> workingStack;

		Configuration(std::string startingSymbol);
		Configuration();
	}; 

	std::vector<std::string> parsingOutput; 
	bool parsingFeedback = false;
	std::stack<WorkingStackElement> finalWorkingStack;


	void nextConfiguration(std::vector<std::string>& w, Configuration& configuration);
	void advance(std::vector<std::string>& w, Configuration& configuration);
	void expand(std::vector<std::string>& w, Configuration& configuration);
	void momentaryInsuccess(std::vector<std::string>& w, Configuration& configuration);
	void back(std::vector<std::string>& w, Configuration& configuration);
	void anotherTry(std::vector<std::string>& w, Configuration& configuration);
	void success(std::vector<std::string>& w, Configuration& configuration);
	void error(std::vector<std::string>& w, Configuration& configuration);

	std::string printConfiguration(Configuration& configuration);
public: 
	Parser(std::string inFile);
	void startParsing(std::vector<std::string> w);
	std::vector<std::string> getParsingOutput();
};