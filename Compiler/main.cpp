#include <iostream>
#include <assert.h>
#include "symbolTable.h"
#include "lexicalAnalyzer.h"
#include "finiteAutomata.h"
#include "contextFreeGrammar.h"
#include "parser.h"
#include "parserOutput.h"

void testSymbolTable() {
	SymbolTable t;

	std::cout << "We should get 0(false): " << t.search("identifier1") << '\n';
	std::cout << "We should get -1: " << t.getId("identifier1") << '\n';

	std::cout << "We should get 1(successful operation): " << t.add("identifier1") << '\n';

	std::cout << "We should get 1(true): " << t.search("identifier1") << '\n';
	std::cout << "We should get 0: " << t.getId("identifier1") << '\n';

	std::cout << "We should get 1(successful operation): " << t.add("identifier2") << '\n';
	std::cout << "We should get 1(successful operation): " << t.add("identifier3") << '\n';

	std::cout << "We should get 1: " << t.getId("identifier2") << '\n';
	std::cout << "We should get 2: " << t.getId("identifier3") << '\n';
}

void testScanner() {
	LexicalAnalyzer l1("in_files/tokens.in", "in_files/p1.txt", "out_files/p1/");
	std::cout << "For program 1: " << '\n';
	l1.parseProgram();
	std::cout << '\n';

	LexicalAnalyzer l2("in_files/tokens.in", "in_files/p2.txt", "out_files/p2/");
	std::cout << "For program 2: " << '\n';
	l2.parseProgram();
	std::cout << '\n';

	LexicalAnalyzer l3("in_files/tokens.in", "in_files/p3.txt", "out_files/p3/");
	std::cout << "For program 3: " << '\n';
	l3.parseProgram();
	std::cout << '\n';

	LexicalAnalyzer l4("in_files/tokens.in", "in_files/p4.txt", "out_files/p4/");
	std::cout << "For program 4: " << '\n';
	l4.parseProgram();
	std::cout << '\n';

	LexicalAnalyzer lFirstErr("in_files/tokens.in", "in_files/p1FirstErr.txt", "out_files/p1FirstErr/");
	std::cout << "For program with first error: " << '\n';
	lFirstErr.parseProgram();
	std::cout << '\n';

	LexicalAnalyzer lSecondErr("in_files/tokens.in", "in_files/p1SecondErr.txt", "out_files/p1SecondErr/");
	std::cout << "For program with second error: " << '\n';
	lSecondErr.parseProgram();
	std::cout << '\n';
}

void testFiniteAutomata() {
	FiniteAutomata fa("in_files/FA.in");
	fa.run();
}

void testCFG() {
	ContextFreeGrammar cfg("in_files/parser/g2.txt");
	cfg.run();
}

void testParser() {
	Parser parser("in_files/parser/g1.txt");

	// success case
	parser.startParsing({ "a", "a", "c", "b", "c" });
	std::vector<std::string> parsingOutputSuccess = parser.getParsingOutput();
	assert(parsingOutputSuccess[0] == "(q; 1; epsilon; S)");
	assert(parsingOutputSuccess[1] == "----expand----");
	assert(parsingOutputSuccess[2] == "(q; 1; S1; a S b S)");
	assert(parsingOutputSuccess[3] == "----advance----");
	assert(parsingOutputSuccess[4] == "(q; 2; S1 a; S b S)");
	assert(parsingOutputSuccess[5] == "----expand----");
	assert(parsingOutputSuccess[6] == "(q; 2; S1 a S1; a S b S b S)");
	assert(parsingOutputSuccess[7] == "----advance----");
	assert(parsingOutputSuccess[8] == "(q; 3; S1 a S1 a; S b S b S)");
	assert(parsingOutputSuccess[9] == "----expand----");
	assert(parsingOutputSuccess[10] == "(q; 3; S1 a S1 a S1; a S b S b S b S)");
	assert(parsingOutputSuccess[11] == "----momentary insuccess----");
	assert(parsingOutputSuccess[12] == "(b; 3; S1 a S1 a S1; a S b S b S b S)");
	assert(parsingOutputSuccess[13] == "----another try----");
	assert(parsingOutputSuccess[14] == "(q; 3; S1 a S1 a S2; a S b S b S)");
	assert(parsingOutputSuccess[15] == "----momentary insuccess----");
	assert(parsingOutputSuccess[16] == "(b; 3; S1 a S1 a S2; a S b S b S)");
	assert(parsingOutputSuccess[17] == "----another try----");
	assert(parsingOutputSuccess[18] == "(q; 3; S1 a S1 a S3; c b S b S)");
	assert(parsingOutputSuccess[19] == "----advance----");
	assert(parsingOutputSuccess[20] == "(q; 4; S1 a S1 a S3 c; b S b S)");
	assert(parsingOutputSuccess[21] == "----advance----");
	assert(parsingOutputSuccess[22] == "(q; 5; S1 a S1 a S3 c b; S b S)");
	assert(parsingOutputSuccess[23] == "----expand----");
	assert(parsingOutputSuccess[24] == "(q; 5; S1 a S1 a S3 c b S1; a S b S b S)");
	assert(parsingOutputSuccess[25] == "----momentary insuccess----");
	assert(parsingOutputSuccess[26] == "(b; 5; S1 a S1 a S3 c b S1; a S b S b S)");
	assert(parsingOutputSuccess[27] == "----another try----");
	assert(parsingOutputSuccess[28] == "(q; 5; S1 a S1 a S3 c b S2; a S b S)");
	assert(parsingOutputSuccess[29] == "----momentary insuccess----");
	assert(parsingOutputSuccess[30] == "(b; 5; S1 a S1 a S3 c b S2; a S b S)");
	assert(parsingOutputSuccess[31] == "----another try----");
	assert(parsingOutputSuccess[32] == "(q; 5; S1 a S1 a S3 c b S3; c b S)");
	assert(parsingOutputSuccess[33] == "----advance----");
	assert(parsingOutputSuccess[34] == "(q; 6; S1 a S1 a S3 c b S3 c; b S)");
	assert(parsingOutputSuccess[35] == "----momentary insuccess----");
	assert(parsingOutputSuccess[36] == "(b; 6; S1 a S1 a S3 c b S3 c; b S)");
	assert(parsingOutputSuccess[37] == "----back----");
	assert(parsingOutputSuccess[38] == "(b; 5; S1 a S1 a S3 c b S3; c b S)");
	assert(parsingOutputSuccess[39] == "----another try----");
	assert(parsingOutputSuccess[40] == "(b; 5; S1 a S1 a S3 c b; S b S)");
	assert(parsingOutputSuccess[41] == "----back----");
	assert(parsingOutputSuccess[42] == "(b; 4; S1 a S1 a S3 c; b S b S)");
	assert(parsingOutputSuccess[43] == "----back----");
	assert(parsingOutputSuccess[44] == "(b; 3; S1 a S1 a S3; c b S b S)");
	assert(parsingOutputSuccess[45] == "----another try----");
	assert(parsingOutputSuccess[46] == "(b; 3; S1 a S1 a; S b S b S)");
	assert(parsingOutputSuccess[47] == "----back----");
	assert(parsingOutputSuccess[48] == "(b; 2; S1 a S1; a S b S b S)");
	assert(parsingOutputSuccess[49] == "----another try----");
	assert(parsingOutputSuccess[50] == "(q; 2; S1 a S2; a S b S)");
	assert(parsingOutputSuccess[51] == "----advance----");
	assert(parsingOutputSuccess[52] == "(q; 3; S1 a S2 a; S b S)");
	assert(parsingOutputSuccess[53] == "----expand----");
	assert(parsingOutputSuccess[54] == "(q; 3; S1 a S2 a S1; a S b S b S)");
	assert(parsingOutputSuccess[55] == "----momentary insuccess----");
	assert(parsingOutputSuccess[56] == "(b; 3; S1 a S2 a S1; a S b S b S)");
	assert(parsingOutputSuccess[57] == "----another try----");
	assert(parsingOutputSuccess[58] == "(q; 3; S1 a S2 a S2; a S b S)");
	assert(parsingOutputSuccess[59] == "----momentary insuccess----");
	assert(parsingOutputSuccess[60] == "(b; 3; S1 a S2 a S2; a S b S)");
	assert(parsingOutputSuccess[61] == "----another try----");
	assert(parsingOutputSuccess[62] == "(q; 3; S1 a S2 a S3; c b S)");
	assert(parsingOutputSuccess[63] == "----advance----");
	assert(parsingOutputSuccess[64] == "(q; 4; S1 a S2 a S3 c; b S)");
	assert(parsingOutputSuccess[65] == "----advance----");
	assert(parsingOutputSuccess[66] == "(q; 5; S1 a S2 a S3 c b; S)");
	assert(parsingOutputSuccess[67] == "----expand----");
	assert(parsingOutputSuccess[68] == "(q; 5; S1 a S2 a S3 c b S1; a S b S)");
	assert(parsingOutputSuccess[69] == "----momentary insuccess----");
	assert(parsingOutputSuccess[70] == "(b; 5; S1 a S2 a S3 c b S1; a S b S)");
	assert(parsingOutputSuccess[71] == "----another try----");
	assert(parsingOutputSuccess[72] == "(q; 5; S1 a S2 a S3 c b S2; a S)");
	assert(parsingOutputSuccess[73] == "----momentary insuccess----");
	assert(parsingOutputSuccess[74] == "(b; 5; S1 a S2 a S3 c b S2; a S)");
	assert(parsingOutputSuccess[75] == "----another try----");
	assert(parsingOutputSuccess[76] == "(q; 5; S1 a S2 a S3 c b S3; c)");
	assert(parsingOutputSuccess[77] == "----advance----");
	assert(parsingOutputSuccess[78] == "(f; 6; S1 a S2 a S3 c b S3 c; epsilon)");
	assert(parsingOutputSuccess[79] == "----success----");

	// error case
	parser.startParsing({ "b" });
	std::vector<std::string> parsingOutputError = parser.getParsingOutput();
	assert(parsingOutputError[0] == "(q; 1; epsilon; S)");
	assert(parsingOutputError[1] == "----expand----");
	assert(parsingOutputError[2] == "(q; 1; S1; a S b S)");
	assert(parsingOutputError[3] == "----momentary insuccess----");
	assert(parsingOutputError[4] == "(b; 1; S1; a S b S)");
	assert(parsingOutputError[5] == "----another try----");
	assert(parsingOutputError[6] == "(q; 1; S2; a S)");
	assert(parsingOutputError[7] == "----momentary insuccess----");
	assert(parsingOutputError[8] == "(b; 1; S2; a S)");
	assert(parsingOutputError[9] == "----another try----");
	assert(parsingOutputError[10] == "(q; 1; S3; c)");
	assert(parsingOutputError[11] == "----momentary insuccess----");
	assert(parsingOutputError[12] == "(b; 1; S3; c)");
	assert(parsingOutputError[13] == "----another try----");
	assert(parsingOutputError[14] == "(e; 1; S3; c)");
	assert(parsingOutputError[15] == "----error----");
}

void testParserOutput() {
	ParserOutput parserOutputg1("in_files/parser/g1.txt");
	parserOutputg1.buildParsingTable("in_files/parser/seq.txt");
	parserOutputg1.printParsingTable("out_files/parser/out1.txt");

	ParserOutput parserOutputg21("in_files/parser/g2.txt");
	parserOutputg21.buildParsingTable("in_files/parser/pif1.out");
	parserOutputg21.printParsingTable("out_files/parser/out21.txt");

	ParserOutput parserOutputg22("in_files/parser/g2.txt");
	parserOutputg22.buildParsingTable("in_files/parser/pif2.out");
	parserOutputg22.printParsingTable("out_files/parser/out22.txt");

	ParserOutput parserOutputg23("in_files/parser/g2.txt");
	parserOutputg23.buildParsingTable("in_files/parser/pif3.out");
	parserOutputg23.printParsingTable("out_files/parser/out23.txt");

	ParserOutput parserOutputg24("in_files/parser/g2.txt");
	parserOutputg24.buildParsingTable("in_files/parser/pif4.out");
	parserOutputg24.printParsingTable("out_files/parser/out24.txt");
}

int main() {
	//testSymbolTable();
	//testFiniteAutomata();
	//testScanner();
	
	//testCFG();
	//testParser();
	//testParserOutput();
	return 0;
}

