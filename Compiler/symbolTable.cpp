#include <string>
#include "symbolTable.h"

SymbolTable::SymbolTable() {
	root = NULL;
	uniqueId = 0;
}

SymbolTable::Node* SymbolTable::createNewNode(std::string key) {
	return new Node(key, uniqueId++);
}

bool SymbolTable::add(std::string key) {
	if (!root) {
		root = createNewNode(key);
		return true;
	}

	Node* node = root;
	while (node != NULL) {
		if (key == node->key) {
			// do nothing, because the key is already added
			return true;
		}
		else if (key < node->key) {
			// we want to go to the left
			if (node->left != NULL) node = node->left;
			else {
				node->left = createNewNode(key);
				return true;
			}
		}
		else {
			// we want to go to the right
			if (node->right != NULL) node = node->right;
			else {
				node->right = createNewNode(key);
				return true;
			}
		}
	}

	// if for some reason we can't insert / find a node with a certain key we return false, meaning the operation has failed
	return false;
}

bool SymbolTable::search(std::string key) {
	Node* node = root;
	while (node != NULL) {
		if (key == node->key) return true;
		else if (key < node->key) node = node->left;
		else node = node->right;
	}

	return false;
}

int SymbolTable::getId(std::string key) {
	Node* node = root;
	while (node != NULL) {
		if (key == node->key) return node->id;
		else if (key < node->key) node = node->left;
		else node = node->right;
	}

	return -1;
}
