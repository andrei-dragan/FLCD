#include "FiniteAutomata.h"

std::vector<std::string> FiniteAutomata::separateByDelimiter(std::string line, char delimiter) {
	std::vector<std::string> answer;
	int nextDelimiterPos = 0;

	while (nextDelimiterPos != std::string::npos) {
		nextDelimiterPos = line.find(delimiter);
		answer.push_back(line.substr(0, nextDelimiterPos));
		line = line.substr(nextDelimiterPos + 1, line.size());
	}

	return answer;
}

void FiniteAutomata::buildTransitions(std::vector<std::string>& unparsedTransitions) {
	for (auto& unparsedTransition : unparsedTransitions) {
		std::vector<std::string> transitionElements = separateByDelimiter(unparsedTransition, ';');
		transitions[transitionElements[0]].push_back({ transitionElements[1], transitionElements[2] });
	}
}

void FiniteAutomata::parseFile(std::string inFile) {
	std::ifstream inputFile(inFile);
	std::string line; 

	// parse the 1st line: the finite set of states
	// we separate each state by space
	std::getline(inputFile, line);
	states = separateByDelimiter(line, ' ');

	// parse the 2nd line: the alphabet
	// we separate each element of the alphabet by space
	std::getline(inputFile, line);
	alphabet = separateByDelimiter(line, ' ');

	// parse the 3rd line: the initial state
	// it's the whole line
	std::getline(inputFile, line);
	initialState = line;

	// parse the 4th line: the final states
	// we separate each final state by space
	std::getline(inputFile, line);
	finalStates = separateByDelimiter(line, ' ');

	// parse the 5th line: the transitions
	// we separate each description of a transition by space
	// each description of a transition is separated by ";"
	// for exaemple q;s;a represents a description of a transition meaning we can go from q to s through a
	std::getline(inputFile, line);
	std::vector<std::string> unparsedTransitions = separateByDelimiter(line, ' ');
	buildTransitions(unparsedTransitions);

	
}

void FiniteAutomata::printMenu() {
	std::cout << "----------------------------------------" << '\n';
	std::cout << "Please choose what you want to output:" << '\n';
	std::cout << "Type <0> to exit" << '\n';
	std::cout << "Type <1> to output the set of states" << '\n';
	std::cout << "Type <2> to output the alphabet" << '\n';
	std::cout << "Type <3> to output the transitions" << '\n';
	std::cout << "Type <4> to output the initial state" << '\n';
	std::cout << "Type <5> to output the set of final states" << '\n';
	std::cout << "Type <6> to test if a sequence is accepted by the FA" << '\n';
	std::cout << "----------------------------------------" << '\n';
}

void FiniteAutomata::printStates() {
	for (auto& state : states) std::cout << state << " ";
	std::cout << '\n';
}

void FiniteAutomata::printAlphabet() {
	for (auto& alphabetElement : alphabet) std::cout << alphabetElement << " ";
	std::cout << '\n';
}

void FiniteAutomata::printTransitions() {
	std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>>::iterator it;
	for (it = transitions.begin(); it != transitions.end(); it++) {
		for (auto& transitionTo : it->second) {
			std::cout << "(" << it->first << ", " << transitionTo.second << ") = " << transitionTo.first << '\n';
		}
	}
}

void FiniteAutomata::printInitialState() {
	std::cout << initialState << '\n';
}

void FiniteAutomata::printFinalStates() {
	for (auto& finalState : finalStates) std::cout << finalState << " ";
	std::cout << '\n';
}

bool FiniteAutomata::isFinalState(std::string state) {
	for (auto& finalState : finalStates) if (finalState == state) return true;
	return false;
}

bool FiniteAutomata::canObtainSequence(std::string sequenceSoFar, std::string finalSequence, std::string currentState) {
	if (sequenceSoFar.size() == finalSequence.size()) {
		if (sequenceSoFar == finalSequence && isFinalState(currentState)) return true;
		return false;
	}
	
	bool answer = false; 
	for (auto& transition : transitions[currentState]) {
		answer = (answer || canObtainSequence(sequenceSoFar + transition.second, finalSequence, transition.first));
	}

	return answer;
}

void FiniteAutomata::checkSequence() {
	std::cout << "Enter your sequence to be checked:" << '\n';
	std::string sequence;
	std::getline(std::cin, sequence);

	if (canObtainSequence("", sequence, initialState)) std::cout << "Your sequence is accepted" << '\n';
	else std::cout << "Your sequence is NOT accepted" << '\n';
}

bool FiniteAutomata::canObtainGivenSequence(std::string sequence) {
	return canObtainSequence("", sequence, initialState);
}

void FiniteAutomata::run() {
	while (true) {
		printMenu();
	
		std::string input;
		std::getline(std::cin,input);

		try {
			int option = stoi(input); 
			if (option == 0) return;
			else if (option == 1) printStates();
			else if (option == 2) printAlphabet();
			else if (option == 3) printTransitions();
			else if (option == 4) printInitialState();
			else if (option == 5) printFinalStates();
			else if (option == 6) checkSequence();
			else {
				std::cout << "Invalid option! Try again!" << '\n';
			}
		}
		catch (std::exception& err) {
			std::cout << "Invalid input. Try again!" << '\n';
		}
	}
}