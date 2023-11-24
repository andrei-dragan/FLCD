#include "parserOutput.h"

ParserOutput::ParserOutput(std::string grammarInFile) : Parser(grammarInFile) {}

ParserOutput::TableRow::TableRow(int p_index, int p_parent, int p_rightSibling, std::string p_info) :
	index(p_index), parent(p_parent), rightSibling(p_rightSibling), info(p_info) {};
void ParserOutput::buildParsingTable(std::string inFile) {
	std::ifstream inputFile(inFile);
	std::string line; 
	while (getline(inputFile, line)) {
		// in the case of the PIF besides the token itself we also have an id - but we care only about the token
		std::string token = separateByDelimiter(line, ' ')[0];
		sequence.push_back(token);
	}

	startParsing(sequence);
	constructParsingTree();
}

void ParserOutput::printParsingTable(std::string outFile) {
	std::ofstream outputFile(outFile);

	if (parsingFeedback == false) {
		outputFile << "The program has a syntax error." << '\n';
		//std::cout << "The program has a syntax error." << '\n';
	}

	// output the result
	for (int i = 0; i < parsingTreeTable.size(); i++) {
		//std::cout << parsingTreeTable[i].index << " " << parsingTreeTable[i].info << " " << parsingTreeTable[i].parent << " " << parsingTreeTable[i].rightSibling << '\n';
		outputFile << parsingTreeTable[i].index << " " << parsingTreeTable[i].info << " " << parsingTreeTable[i].parent << " " << parsingTreeTable[i].rightSibling << '\n';
	}
}

void ParserOutput::constructParsingTree() {
	if (parsingFeedback == false) {
		return; 
	}

	// transform the working stack into a vector
	std::vector<WorkingStackElement> workingStackVector, nonterminalOperations;

	while (!finalWorkingStack.empty()) {
		WorkingStackElement workingStackHead = finalWorkingStack.top();
		finalWorkingStack.pop();
		workingStackVector.push_back(workingStackHead);
	}

	for (int i = workingStackVector.size() - 1; i >= 0; i--) {
		finalWorkingStack.push(workingStackVector[i]);
	
		if (workingStackVector[i].index == -1) {
			// it's a terminal so we skip it 
			continue; 
		}
		else if (workingStackVector[i].index != -1) {
			// we have a nonterminal 
			nonterminalOperations.push_back(workingStackVector[i]);
		}
	}

	// build the tree
	parsingTreeTable.push_back(TableRow(1, 0, 0, startingSymbol));
	int i = 0, tableIndex = 1, nonterminalOperationIndex = 0;
	while (nonterminalOperationIndex < nonterminalOperations.size()) {
		if (isNonterminal(parsingTreeTable[i].info)) {
			// we expand the tree
			std::string element = nonterminalOperations[nonterminalOperationIndex].element;
			int prodIndex = nonterminalOperations[nonterminalOperationIndex].index;
			nonterminalOperationIndex++;

			int rightSibing = 0;
			for (auto& transformationElement : productions[element][prodIndex - 1]) {
				tableIndex++;
				parsingTreeTable.push_back(TableRow(tableIndex, i + 1, rightSibing, transformationElement));
				rightSibing = tableIndex;
			}
		}
		i++;
	}
}