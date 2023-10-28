#include <iostream>
#include "symbolTable.h"
#include "lexicalAnalyzer.h"

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

int main() {

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

	return 0;
}

