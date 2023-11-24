#include "parser.h";

Parser::Parser(std::string inFile) : ContextFreeGrammar(inFile) {}

Parser::WorkingStackElement::WorkingStackElement(std::string p_element, int p_index) {
	element = p_element;
	index = p_index;
}

Parser::Configuration::Configuration() {
	state = "q";
	i = 0;
};
Parser::Configuration::Configuration(std::string startingSymbol) {
	state = "q";
	i = 1;
	inputStack.push(startingSymbol);
}

std::string Parser::printConfiguration(Configuration& configuration) {
	std::string answer = "";
	answer = "(" + configuration.state + "; " + std::to_string(configuration.i) + "; ";

	// transform the working stack into a vector
	std::vector<WorkingStackElement> workingStackVector;
	while (!configuration.workingStack.empty()) {
		WorkingStackElement workingStackHead = configuration.workingStack.top();
		configuration.workingStack.pop();
		workingStackVector.push_back(workingStackHead);
	}
	for (int i = workingStackVector.size() - 1; i >= 0; i--) configuration.workingStack.push(workingStackVector[i]);
	
	// transform the input stack into a vector
	std::vector<std::string> inputStackVector;
	while (!configuration.inputStack.empty()) {
		std::string inputStackHead = configuration.inputStack.top();
		configuration.inputStack.pop();
		inputStackVector.push_back(inputStackHead);
	}
	for (int i = inputStackVector.size() - 1; i >= 0; i--) configuration.inputStack.push(inputStackVector[i]);

	// print the working stack
	for (int i = workingStackVector.size() - 1; i > 0; i--) {
		if (workingStackVector[i].index != -1) answer += workingStackVector[i].element + std::to_string(workingStackVector[i].index) + " ";
		else answer += workingStackVector[i].element + " ";
	}
	if (workingStackVector.size() > 0) {
		if (workingStackVector[0].index != -1) answer += workingStackVector[0].element + std::to_string(workingStackVector[0].index) + "; ";
		else answer += workingStackVector[0].element + "; ";
	}
	else {
		answer += "epsilon";
		answer += "; ";
	}

	// print the input stack
	if (inputStackVector.size() > 1) for (int i = 0; i <= inputStackVector.size() - 2; i++) answer += inputStackVector[i] + " ";
	if (inputStackVector.size() > 0) answer += inputStackVector[inputStackVector.size() - 1] + ")";
	else answer += "epsilon)";


	parsingOutput.push_back(answer);
	//std::cout << answer << '\n';
	return answer;
}

void Parser::startParsing(std::vector<std::string> w) {
	parsingOutput.clear();

	Configuration configuration = Configuration(startingSymbol);

	printConfiguration(configuration);
	nextConfiguration(w, configuration);
}

void Parser::nextConfiguration(std::vector<std::string>& w, Configuration& configuration) {

	while (configuration.state != "e" && configuration.state != "f") {
		std::string headInputStack = "";
		std::string headWorkingStack = "";
		if (!configuration.inputStack.empty()) headInputStack = configuration.inputStack.top();
		if (!configuration.workingStack.empty()) headWorkingStack = configuration.workingStack.top().element;

		if (configuration.state == "q") {
			if (isNonterminal(headInputStack)) expand(w, configuration);
			else if (isTerminal(headInputStack) && (configuration.i > w.size()))  momentaryInsuccess(w, configuration);
			else if (isTerminal(headInputStack) && headInputStack == w[configuration.i - 1]) advance(w, configuration);
			else if (isTerminal(headInputStack) && headInputStack != w[configuration.i - 1]) momentaryInsuccess(w, configuration);
		}
		else if (configuration.state == "b") {
			if (isTerminal(headWorkingStack)) back(w, configuration);
			else if (isNonterminal(headWorkingStack)) anotherTry(w, configuration);
		}
	}	

	if (configuration.state == "e") error(w, configuration);
	else if (configuration.state == "f") success(w, configuration);
}

void Parser::expand(std::vector<std::string>& w, Configuration& configuration) {
	std::string headInputStack = configuration.inputStack.top();

	configuration.inputStack.pop();
	for (int i = productions[headInputStack][0].size() - 1; i >= 0; i--) {
		configuration.inputStack.push(productions[headInputStack][0][i]);
	}
	configuration.workingStack.push(WorkingStackElement(headInputStack, 1));
	parsingOutput.push_back("----expand----");
	printConfiguration(configuration);
}

void Parser::advance(std::vector<std::string>& w, Configuration& configuration) {
	std::string headInputStack = configuration.inputStack.top();
	configuration.i += 1;
	configuration.inputStack.pop();
	configuration.workingStack.push(WorkingStackElement(headInputStack, -1));

	if (configuration.i == w.size() + 1 && configuration.inputStack.empty()) {
		configuration.state = "f";

		parsingOutput.push_back("----advance----");
		printConfiguration(configuration);
	}
	else {
		parsingOutput.push_back("----advance----");
		printConfiguration(configuration);
	}
}

void Parser::momentaryInsuccess(std::vector<std::string>& w, Configuration& configuration) {
	configuration.state = 'b';
	parsingOutput.push_back("----momentary insuccess----");
	printConfiguration(configuration);
}

void Parser::back(std::vector<std::string>& w, Configuration& configuration) {
	std::string headWorkingStack = configuration.workingStack.top().element;
	configuration.workingStack.pop();
	configuration.inputStack.push(headWorkingStack);
	configuration.i -= 1;

	parsingOutput.push_back("----back----");
	printConfiguration(configuration);
}

void Parser::anotherTry(std::vector<std::string>& w, Configuration& configuration) {
	WorkingStackElement headWorkingStack = configuration.workingStack.top();

	if (headWorkingStack.index + 1 > productions[headWorkingStack.element].size()) {
		if (configuration.i == 1 && headWorkingStack.element == startingSymbol) {
			parsingOutput.push_back("----another try----");
			configuration.state = "e";
			printConfiguration(configuration);
		}
		else {
			configuration.workingStack.pop();
			for (int i = 0; i < productions[headWorkingStack.element][headWorkingStack.index - 1].size(); i++) configuration.inputStack.pop();
			configuration.inputStack.push(headWorkingStack.element);

			parsingOutput.push_back("----another try----");;
			printConfiguration(configuration);
		}
	}
	else {
		configuration.state = 'q';
		configuration.workingStack.pop();
		configuration.workingStack.push(WorkingStackElement(headWorkingStack.element, headWorkingStack.index + 1));


		for (int i = 0; i < productions[headWorkingStack.element][headWorkingStack.index - 1].size(); i++) configuration.inputStack.pop();
		for (int i = productions[headWorkingStack.element][headWorkingStack.index].size() - 1; i >= 0; i--) {
			configuration.inputStack.push(productions[headWorkingStack.element][headWorkingStack.index][i]);
		}

		parsingOutput.push_back("----another try----");
		printConfiguration(configuration);
	}
}

void Parser::success(std::vector<std::string>& w, Configuration& configuration) {
	parsingOutput.push_back("----success----");
	parsingFeedback = true;
	finalWorkingStack = configuration.workingStack;
}

void Parser::error(std::vector<std::string>& w, Configuration& configuration) {
	parsingOutput.push_back("----error----");
	parsingFeedback = false;
}

std::vector<std::string> Parser::getParsingOutput() {
	return parsingOutput;
}