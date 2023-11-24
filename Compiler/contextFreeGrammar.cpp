#include "contextFreeGrammar.h"

ContextFreeGrammar::ContextFreeGrammar(std::string inFile) {
	parseFile(inFile);
}

std::vector<std::string> ContextFreeGrammar::separateByDelimiter(std::string line, char delimiter) {
	std::vector<std::string> answer;
	int nextDelimiterPos = 0;

	while (nextDelimiterPos != std::string::npos) {
		nextDelimiterPos = line.find(delimiter);
		answer.push_back(line.substr(0, nextDelimiterPos));
		line = line.substr(nextDelimiterPos + 1, line.size());
	}

	return answer;
}

void ContextFreeGrammar::buildProductions(std::vector<std::string>& unparsedProductions) {
	for (auto& unparsedProduction : unparsedProductions) {
		std::vector<std::string> productionElements = separateByDelimiter(unparsedProduction, ':');
		std::string nonterminal = productionElements[0];
		
		std::vector<std::string> productionTransformations = separateByDelimiter(productionElements[1], '|');
		for (auto& productionTransformation : productionTransformations) {
			std::vector<std::string> productionTransformationElements = separateByDelimiter(productionTransformation, '.');
			productions[nonterminal].push_back(productionTransformationElements);
		}
	}
}

void ContextFreeGrammar::parseFile(std::string inFile) {
	std::ifstream inputFile(inFile);
	std::string line;

	// parse the 1st line: the nonterminals
	// we separate each nonterminal by space
	std::getline(inputFile, line);
	nonterminals = separateByDelimiter(line, ' ');

	// parse the 2nd line: the terminals
	// we separate each terminal by space
	std::getline(inputFile, line);
	terminals = separateByDelimiter(line, ' ');

	// parse the 3rd line: the starting symbol of the grammar
	// it's the whole line
	std::getline(inputFile, line);
	startingSymbol = line;

	// parse the 4th line: the set of productions
	// we separate each description of a production by space
	// each description of a production has the following format: 
	// X:A.B|C.D - this is equivalent to X -> AB | CD
	std::getline(inputFile, line);
	std::vector<std::string> unparsedProductions = separateByDelimiter(line, ' ');
	buildProductions(unparsedProductions);
}

void ContextFreeGrammar::printMenu() {
	std::cout << "----------------------------------------" << '\n';
	std::cout << "Please choose what you want to output:" << '\n';
	std::cout << "Type <0> to exit" << '\n';
	std::cout << "Type <1> to output the set of nonterminals" << '\n';
	std::cout << "Type <2> to output the set of terminals" << '\n';
	std::cout << "Type <3> to output the set of productions" << '\n';
	std::cout << "Type <4> to output the starting symbol" << '\n';
	std::cout << "Type <5> to test if the given grammar is actually a CFG" << '\n';
	std::cout << "----------------------------------------" << '\n';
}

void ContextFreeGrammar::printNonterminals() {
	for (auto& nonterminal : nonterminals) std::cout << nonterminal << '\n';
	std::cout << '\n';
}

void ContextFreeGrammar::printTerminals() {
	for (auto& terminal : terminals) std::cout << terminal << '\n';
	std::cout << '\n';
}

void ContextFreeGrammar::printProductions() {
	std::unordered_map<std::string, std::vector<std::vector<std::string>>>::iterator it;
	for (it = productions.begin(); it != productions.end(); it++) {
		std::cout << it->first << " -> ";
		
		for (int i = 0; i < it->second.size() - 1; i++) {
			for (auto& transformationElement : it->second[i])
				std::cout << transformationElement << " ";
			std::cout << "| ";
		}

		// the last transformation is treated differently because we don't want to ouput the '|' at the end
		for (auto& transformationElement : it->second[it->second.size() - 1]) {
			std::cout << transformationElement << " ";
		}

		std::cout << '\n';
	}
}

void ContextFreeGrammar::printStartingSymbol() {
	std::cout << startingSymbol << '\n';
}

void ContextFreeGrammar::checkIfValid() {

	if (!isNonterminal(startingSymbol)) {
		std::cout << "The given grammar is not a CFG because the starting symbol is a nonterminal: " << startingSymbol << '\n';
		return;
	}

	std::unordered_map<std::string, std::vector<std::vector<std::string>>>::iterator it;
	for (it = productions.begin(); it != productions.end(); it++) {
		if (!isNonterminal(it->first)) {
			std::cout << "The given grammar is not a CFG because there is a production where the left-hand side (" << it->first << ") is not a nonterminal" << '\n';
			return; 
		}

		for (auto& transformationElements : it->second) {
			for (auto& transformationElement : transformationElements) {
				if (!isNonterminal(transformationElement) && !isTerminal(transformationElement)) {
					std::cout << "The given grammar is not a CFG because there is a production where on the right-hand side is an unknown symbol (neither a terminal nor a nonterminal): " << transformationElement << '\n';
					return;
				}
			}
		}

		
	}
	std::cout << "The given grammar is a CFG" << '\n';
}

bool ContextFreeGrammar::isNonterminal(std::string potentialNonterminal) {
	for (auto& nonterminal : nonterminals) {
		if (potentialNonterminal == nonterminal) return true;
	}
	return false;
}

bool ContextFreeGrammar::isTerminal(std::string potentialTerminal) {
	for (auto& terminal : terminals) {
		if (potentialTerminal == terminal) return true;
	}
	return false;
}

void ContextFreeGrammar::run() {
	while (true) {
		printMenu();

		std::string input;
		std::getline(std::cin, input);

		try {
			int option = stoi(input);
			if (option == 0) return;
			else if (option == 1) printNonterminals();
			else if (option == 2) printTerminals();
			else if (option == 3) printProductions();
			else if (option == 4) printStartingSymbol();
			else if (option == 5) checkIfValid();
			else {
				std::cout << "Invalid option! Try again!" << '\n';
			}
		}
		catch (std::exception& err) {
			std::cout << "Invalid input. Try again!" << '\n';
		}
	}
}