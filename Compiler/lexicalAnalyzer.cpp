#include "lexicalAnalyzer.h"

std::vector<std::string> LexicalAnalyzer::parseInLine(std::string line, std::vector<std::string> separatorList, bool addSeparator) {
	std::vector<std::string> answer;

	std::string nextSeparator;
	int nextSeparatorPos = INT_MAX;
	for (auto& separator : separatorList) {
		// for each separator passed we try to check if it can be found inside the current line and get the left-most one
		int separatorPos = line.find(separator);
		if (separatorPos != -1) {
			if (separatorPos < nextSeparatorPos) {
				nextSeparatorPos = separatorPos;
				nextSeparator = separator;
			}
		}
	}

	while (nextSeparatorPos != INT_MAX) {
		if (nextSeparatorPos != 0) {
			/*
				If nextSeparatorPos is 0, that means we don't really have a valid token so we won't add it

				One case where this might happen is if we have something like "2  3":
					1. it will find the first space as separator, thus identifying 2 as a token and resulting in the following remaining string " 3";
					2. with " 3", nextSeparatorPos will be 0 (pointing to the only remaining space), but in this case we don't really have a token to add, so we just skip the separator
			*/
			answer.push_back(line.substr(0, nextSeparatorPos));
		}

		// we want to keep the separator as well if the flag says so - we want this when we construct the PI
		if (addSeparator) answer.push_back(nextSeparator);

		// slice the line such that everything from [0; nextSeparatorPos] is removed
		line.erase(line.begin(), line.begin() + nextSeparatorPos + nextSeparator.size());

		// get the next separator if possible
		nextSeparatorPos = INT_MAX;
		for (auto& separator : separatorList) {
			int separatorPos = line.find(separator);
			if (separatorPos != -1) {
				if (separatorPos < nextSeparatorPos) {
					nextSeparatorPos = separatorPos;
					nextSeparator = separator;
				}
			}
		}
	}

	// if for some reason our line is not empty, it means it ends with a valid token
	if (line.size()) answer.push_back(line);

	return answer;
}

void LexicalAnalyzer::parsePredefinedTokens() {
	std::ifstream inputFile(tokensInFile);
	std::string line;
	int lineId = 0;

	// my predefined separators
	std::vector<std::string> tokensDelimiters = { " ", "\n", std::to_string(EOF) };

	while (std::getline(inputFile, line)) {
		lineId++;
		if (lineId == 1) { // we have delimiters
			delimiters = parseInLine(line, tokensDelimiters, false);
			// we add some special delimiters as well
			delimiters.push_back(" ");
			delimiters.push_back("\n");
			delimiters.push_back("\t");
			delimiters.push_back(std::to_string(EOF));
		}
		else if (lineId == 2) { // we have operators
			operators = parseInLine(line, tokensDelimiters, false);
			std::sort(operators.begin(), operators.end(), [](std::string& A, std::string& B) {
				return (A.size() > B.size());
				});
		}
		else if (lineId == 3) { // we have keywords
			keywords = parseInLine(line, tokensDelimiters, false);
		}
	}

	inputFile.close();
}

bool LexicalAnalyzer::isIdentifier(std::string token) {
	std::regex identifierRegex("^\\$[a-zA-Z][a-zA-Z]*$");
	return std::regex_match(token, identifierRegex);
}

LexicalAnalyzer::ConstantType LexicalAnalyzer::isConstant(std::string token) {
	std::regex integerRegex("^(-?[1-9][0-9]*|0)$");
	std::regex floatRegex("^(-?[1-9][0-9]*|0)\\.([1-9][0-9]*|0)$");
	std::regex charRegex("^'([0-9a-zA-Z_\\,\\.\\s\\n\\?\\-])'$");
	std::regex boolRegex("^(true|false)$");

	// check for each constant type
	if (std::regex_match(token, integerRegex)) return CONSTANT_INTEGER;
	else if (std::regex_match(token, floatRegex)) return CONSTANT_FLOAT;
	else if (std::regex_match(token, charRegex)) return CONSTANT_CHAR;
	else if (std::regex_match(token, boolRegex)) return CONSTANT_BOOL;
	else return CONSTANT_INVALID;
}

LexicalAnalyzer::LexicalAnalyzer(std::string p_tokensInFile, std::string p_programInFile, std::string p_programOutPath)
	: tokensInFile(p_tokensInFile), programInFile(p_programInFile), programOutPath(p_programOutPath) {};

void LexicalAnalyzer::parseProgram() {
	std::ifstream inputFile(programInFile);
	std::ofstream pifOutFile(programOutPath + "pif.out");
	std::ofstream stOutFile(programOutPath + "st.out");

	// get the delimiters, operators and keywords 
	parsePredefinedTokens();

	// construct the list of separators (operators + delimiters)
	std::vector<std::string> separatorList;
	separatorList.insert(separatorList.end(), delimiters.begin(), delimiters.end());
	separatorList.insert(separatorList.end(), operators.begin(), operators.end());

	std::string line;
	int lineId = 0;
	bool correct = true; 

	while (correct && std::getline(inputFile, line)) {
		lineId++;
		std::vector<std::string> tokens = parseInLine(line, separatorList, true);

		for (auto& token : tokens) {
			if (std::find(operators.begin(), operators.end(), token) != operators.end() ||
				std::find(delimiters.begin(), delimiters.end(), token) != delimiters.end() ||
				std::find(keywords.begin(), keywords.end(), token) != keywords.end()
			) {
				pif.push_back({ token, "0" });
			}
			else if ((isConstant(token) != CONSTANT_INVALID) || isIdentifier(token)) {
				if (!st.search(token)) st.add(token);
			
				int index = st.getId(token);
				pif.push_back({ token, std::to_string(index) });
			}
			else {
				std::cout << "Lexical error at line " << lineId << " for the following token: " << token << '\n';
				correct = false; 
				break;
			}
		}
	}
	
	for (auto& pifEntry : pif) pifOutFile << pifEntry.first << " " << pifEntry.second << '\n';

	stOutFile << "Used BST: " << '\n';
	for (auto& stEntry : st.getEntries()) stOutFile << stEntry.second << " " << stEntry.first << '\n';

	if (correct) std::cout << "Lexically correct" << '\n';

	inputFile.close();
}