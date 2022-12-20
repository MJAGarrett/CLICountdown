#include <vector>
#include <unordered_map>
#include <string>
#include "MGHelperFunctions.h"
#include <set>
#include <fstream>
#include <iostream>
#include <functional>

#pragma once

class Node
{
	char val;
	Node* parent = nullptr;
	std::unordered_map<char, Node*> children = {};
	bool wordEnd = false;

public:
	Node(char val) : val(val) {};

	void addChildNode(Node* child)
	{
		this->children.insert(std::pair<char, Node*>(child->getLetter(), child));
	}

	Node* getChild(char key)
	{
		return (*this->children.find(key)).second;
	}

	std::vector<Node*> getChildren()
	{
		std::vector<Node*> children;
		for (auto &it : this->children)
		{
			children.push_back(it.second);
		}

		return children;
	}

	void removeChild(char key)
	{
		this->children.erase(key);
	}

	bool hasChildren()
	{
		return this->children.size() > 0;
	}

	bool hasChild(char key) const
	{
		return this->children.find(key) != this->children.end();
	}

	bool isWordEnd() const
	{
		return this->wordEnd;
	}

	void setWordEnd(bool truth)
	{
		this->wordEnd = truth;
	}

	void setParent(Node* parent)
	{
		this->parent = parent;
	}

	Node* getParent()
	{
		return this->parent;
	}
	
	char getLetter()
	{
		return this->val;
	}

	std::string getWord()
	{
		Node* node = this;
		std::string output;
		while (node != nullptr && node->getLetter() != 0)
		{
			output.insert(output.begin(), node->getLetter());
			node = node->getParent();
		}

		return output;
	}
};

/**
* @brief A class for storing and manipulating the words this application recognizes. Implements word storage using a basic trie.
*/
class Dictionary
{
	Node root = Node(0);
	unsigned int numOfWords = 0;
	std::string conundrumBase;

	void findWordsFromNode(Node* node, std::vector<std::string>& outputVec)
	{
		if (node->isWordEnd()) 
			outputVec.push_back(node->getWord());

		for (auto child : node->getChildren())
		{
			this->findWordsFromNode(child, outputVec);
		}
	}

	void pruneBranch(Node* leaf)
	{
		Node* node = leaf;

		while (!node->isWordEnd() && !node->hasChildren() && node->getParent() != nullptr)
		{
			Node* parent = node->getParent();
			parent->removeChild(node->getLetter());
			delete node;
			node = parent;
		}
	}

	void searchForWords(Node* node, std::vector<char> letters, std::set<std::string>& wordSet)
	{
		if (node->isWordEnd()) wordSet.insert(node->getWord());
		if (letters.size() == 0) return;

		for (auto letter : letters)
		{
			if (!node->hasChild(letter)) continue;

			Node* nextNode = node->getChild(letter);
			std::vector<char> nextLetters = MG::removeElementFromVector(letters, letter);

			this->searchForWords(nextNode, nextLetters, wordSet);
		}
	}

	void loadWordsFromDisk(std::string fileName)
	{
		std::string str;
		std::fstream file(fileName);
		if (file.fail())
		{
			std::cerr << "Error reading file" << std::endl;
		}
		else
		{
			while (!file.eof())
			{
				std::getline(file, str, ',');
				if (str.size() == 0) continue;
				this->addWord(str);
			}
			file.close();
		}
	}

	void saveWordsToDisk(std::string fileName)
	{
		std::ofstream file(fileName);
		if (file.fail())
		{
			std::cerr << "Error reading file" << std::endl;
		}
		else 
		{
			std::vector<std::string> dictContents = this->getAllWords();
			auto dictIterator = dictContents.begin();

			while (dictIterator != dictContents.end())
			{
				file << *dictIterator << ",";
				dictIterator++;
			}
			file.close();
			std::cout << "Saved dictionary successfully" << std::endl;
		}
	}

	void recursiveClear(Node* node)
	{
		if (!node->hasChildren())
		{
			delete node;
		}
		else
		{
			for (auto child : node->getChildren())
			{
				recursiveClear(child);
			}
			delete node;
		}
	}

	void searchAtDepth(Node* node, std::vector<std::string>& outputVector, int depth = INT_MAX)
	{
		if (depth == 0 && node->isWordEnd())
				outputVector.push_back(node->getWord());

		for (auto child : node->getChildren())
		{
			searchAtDepth(child, outputVector, depth - 1);
		}
	}

	std::vector<std::string> filterFoundWords(const std::set<std::string>& original, const std::function<bool(std::string)>& predicate)
	{
		std::vector<std::string> copy;

		std::copy_if(original.begin(), original.end(), std::back_inserter(copy), predicate);

		return copy;
	}
	std::vector<std::string> filterFoundWords(const std::vector<std::string>& original, const std::function<bool(std::string)>& predicate)
	{
		std::vector<std::string> copy;

		std::copy_if(original.begin(), original.end(), std::back_inserter(copy), predicate);

		return copy;
	}

	std::string getTwoWordConundrum(const std::string& baseWord, int sizeOfBiggerWord)
	{
		std::string copyToAlter = baseWord;
		std::vector<char> firstLetterSet;
		std::vector<char> secondLetterSet;

		while (copyToAlter.size() > 0)
		{
			int randIndex = rand() % copyToAlter.size();
			firstLetterSet.size() < sizeOfBiggerWord ? firstLetterSet.push_back(copyToAlter[randIndex]) : secondLetterSet.push_back(copyToAlter[randIndex]);
			copyToAlter.erase(randIndex, 1);
		}

		auto firstBatch = this->findLongestPossibleWordsInSet(firstLetterSet);
		auto secondBatch = this->findLongestPossibleWordsInSet(secondLetterSet);
		if (firstBatch.size() == 0 || secondBatch.size() == 0 || firstBatch[0].size() + secondBatch[0].size() < 9) return "";

		int firstPos = rand() % firstBatch.size();
		int secondPos = rand() % secondBatch.size();

		return firstBatch[firstPos] + secondBatch[secondPos];
	}

	std::string getThreeWordConundrum(const std::string& baseWord)
	{
		std::string copyToAlter = baseWord;
		std::vector<char> firstLetterSet;
		std::vector<char> secondLetterSet;
		std::vector<char> thirdLetterSet;

		while (copyToAlter.size() > 0)
		{
			int randIndex = rand() % copyToAlter.size();
			if (firstLetterSet.size() < 3) firstLetterSet.push_back(copyToAlter[randIndex]);
			else if (secondLetterSet.size() < 3) secondLetterSet.push_back(copyToAlter[randIndex]);
			else thirdLetterSet.push_back(copyToAlter[randIndex]);
			copyToAlter.erase(randIndex, 1);
		}

		auto firstBatch = this->findLongestPossibleWordsInSet(firstLetterSet);
		if (firstBatch.size() == 0 || firstBatch[0].size() < 3) return "";
		auto secondBatch = this->findLongestPossibleWordsInSet(secondLetterSet);
		if (secondBatch.size() == 0 || secondBatch[0].size() < 3) return "";
		auto thirdBatch = this->findLongestPossibleWordsInSet(thirdLetterSet);
		if (thirdBatch.size() == 0 || thirdBatch[0].size() < 3) return "";

		int firstPos = rand() % firstBatch.size();
		int secondPos = rand() % secondBatch.size();
		int thirdPos = rand() % thirdBatch.size();

		return firstBatch[firstPos] + secondBatch[secondPos] + thirdBatch[thirdPos];
	}

public:

	Dictionary() 
	{
		srand(time(NULL));
	};

	Dictionary(std::string fileName) 
	{
		srand(time(NULL));
		// Potentially better to use a separate thread to allow showing a "loading" indicator for slower computers/larger word bases.
		loadWordsFromDisk(fileName);
	}

	std::vector<std::string> findXLengthWords(unsigned int length)
	{
		std::vector<std::string> output;
		searchAtDepth(&this->root, output, length);
		return output;
	}

	unsigned int numberOfWords()
	{
		return this->numOfWords;
	}

	void saveWords(std::string fileName)
	{
		this->saveWordsToDisk(fileName);
	}

	std::vector<std::string> getAllWords()
	{
		std::vector<std::string> output;
		this->findWordsFromNode(&this->root, output);

		return output;
	}

	std::vector<std::string> getWordsWithPrefix(const std::string prefix)
	{
		std::vector<std::string> output;
		Node* node = &this->root;
		for (int i = 0; i < prefix.size(); i++)
		{
			Node* nextNode = node->getChild(prefix[i]);
			if (nextNode == nullptr) return output;
			node = nextNode;
		}

		this->findWordsFromNode(node, output);
		return output;
	}

	void addWord(const std::string& word)
	{
		Node* node = &this->root;

		for (int i = 0; i < word.size(); i++)
		{
			if (!node->hasChild(word[i]))
			{
				Node* newNode = new Node(word[i]);
				newNode->setParent(node);
				node->addChildNode(newNode);
			}

			node = node->getChild(word[i]);

			if (i == word.size() - 1)
			{
				node->setWordEnd(true);
				this->numOfWords += 1;
			}
		}
	}
	void addWord(const std::vector<std::string>& wordList)
	{
		for (const std::string& word : wordList)
		{
			this->addWord(word);
		}
	}

	bool lookUpWord(std::string word)
	{
		Node* node = &this->root;

		for (auto letter : word)
		{
			if (!node->hasChild(letter)) return false;
			node = node->getChild(letter);
		}

		return node->isWordEnd();
	}

	bool removeWord(std::string word)
	{
		if (word == "") return false;
		Node* node = &this->root;

		for (auto letter : word)
		{
			if (!node->hasChild(letter)) return false;
			node = node->getChild(letter);
		}

		if (node->isWordEnd() && node->getWord() == word)
		{
			node->setWordEnd(false);
			this->numOfWords -= 1;
			this->pruneBranch(node);

			return true;
		}
		return false;
	}

	void clearDictionary()
	{
		for (auto child : this->root.getChildren())
		{
			this->root.removeChild(child->getLetter());
			this->recursiveClear(child);
		}
		this->numOfWords = 0;
	}

	std::set<std::string> findPossibleWordsInSet(std::vector<char> lettersSet)
	{
		std::set<std::string> possibleWords;
		
		this->searchForWords(&this->root, lettersSet, possibleWords);
		return possibleWords;
	}

	std::vector<std::string> findLongestPossibleWordsInSet(std::vector<char> lettersSet)
	{
		const std::set<std::string> possibleWords = this->findPossibleWordsInSet(lettersSet);
		std::vector<std::string> output;
		int length = 0;

		for (auto word : possibleWords)
		{
			if (word.size() > length)
			{
				output = { word };
				length = word.size();
			}
			else if (word.size() == length)
			{
				output.push_back(word);
			}
		}

		return output;
	}

	std::string getConundrumBaseWord()
	{
		return this->conundrumBase;
	}

	std::string makeConundrum()
	{
		if (this->numberOfWords() == 0) throw "No words in dictionary";
		const std::vector<std::string> nineLetterWords = this->findXLengthWords(9);
		std::string baseWord;
		std::string conundrum;

		while (conundrum.size() == 0)
		{
			int randomIndex = rand() % nineLetterWords.size();
			baseWord = nineLetterWords[randomIndex];

			int conundrumStyle = rand() % 3;
			switch (conundrumStyle)
			{
			case 0:
				conundrum = this->getTwoWordConundrum(baseWord, 5);
				break;
			case 1:
				conundrum = this->getTwoWordConundrum(baseWord, 6);
				break;
			case 2:
				conundrum = this->getThreeWordConundrum(baseWord);
				break;
			default:
				throw "Error when deciding style of conundrum.";
			}
		}
		this->conundrumBase = baseWord;
		std::cout << "baseword was " << baseWord << std::endl;

		std::cout << "Conundrum was: " << conundrum << std::endl;
		return conundrum;
	}
};

