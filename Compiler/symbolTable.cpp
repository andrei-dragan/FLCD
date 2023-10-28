#include "symbolTable.h"

SymbolTable::SymbolTable() {
	root = NULL;
	uniqueId = 1;
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

std::vector<std::pair<std::string, int>> SymbolTable::getEntries() {
	std::vector<std::pair<std::string, int>> answer;
	std::queue<Node*> q;

	if (root == NULL) return {};

	q.push(root);
	while (!q.empty()) {
		Node* currNode = q.front();
		q.pop();

		answer.push_back({ currNode->key, currNode->id });

		if (currNode->left != NULL) q.push(currNode->left);
		if (currNode->right != NULL) q.push(currNode->right);
	}

	// sort ascending by id
	std::sort(answer.begin(), answer.end(), [](std::pair<std::string, int>& A, std::pair<std::string, int>& B) {
		return (A.second < B.second);
	});

	return answer;
}