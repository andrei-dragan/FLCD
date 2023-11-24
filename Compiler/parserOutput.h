#pragma once

#include "parser.h"
#include <string>
#include <vector>
#include <fstream>

class ParserOutput : protected Parser {
protected:
	std::vector<std::string> sequence; 

	struct TableRow {
		int index, parent, rightSibling;
		std::string info;

		TableRow(int p_index, int p_parent, int p_rightSibling, std::string p_info);
	};
	std::vector<TableRow> parsingTreeTable;
	void constructParsingTree();

public: 
	ParserOutput(std::string grammarInFile);
	void buildParsingTable(std::string inFile);
	void printParsingTable(std::string outFile);
};