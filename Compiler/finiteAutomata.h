#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <iostream>


/*
	Lexic:
		digit := "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
		letter := "a" | "b" | "c" | "d" | ... | "z" | "A" | "B" | "C" | "D" | ... | "Z"
		special_character := "-" | "_" | "," | "." | " " | "\n" | "?"
		space := " "

		state_character := digit | letter
		generic_character := digit | letter | special_character

		invalid_state := character | character invalid_state
		valid_state := letter | invalid_state

	Syntax:
		set_of_states := valid_state | valid_state space set_of_states 
		initial_state := valid_state
		final_states := valid_state | valid_state space set_of_states 
		alphabet := generic_character | generic_character space alphabet

		transition := valid_state ";" valid_state ";" special_character 
		transitions := transition | transition space transitions

		program := set_of_states "\n" alphabet "\n" initial_state "\n" final_states "\n" transitions
*/

class FiniteAutomata {
private: 
	std::vector<std::string> states; 
	std::vector<std::string> alphabet;
	std::vector<std::string> finalStates; 
	std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> transitions;
	std::string initialState;

	std::vector<std::string> separateByDelimiter(std::string line, char delimiter);
	void buildTransitions(std::vector<std::string>& unparsedTransitions);
	
	void printMenu();
	void printStates();
	void printAlphabet();
	void printTransitions();
	void printInitialState();
	void printFinalStates();

	bool isFinalState(std::string state);

	void checkSequence();
	bool canObtainSequence(std::string sequenceSoFar, std::string finalSequence, std::string currentState);

public: 
	FiniteAutomata(std::string inFile);
	void parseFile(std::string inFile);
	void run();
	bool canObtainGivenSequence(std::string sequence);
};
