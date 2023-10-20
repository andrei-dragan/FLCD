#include <iostream>
#include "symbolTable.h"

int main() {

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

	return 0;
}

