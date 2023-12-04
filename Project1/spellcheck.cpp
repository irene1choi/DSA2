#include "hash.h"

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <algorithm>

// Initial dictionary size of a large prime # and max word length of 20
const int DICT_SIZE = 200000;
const int MAX_WORD_LENGTH = 20;

// Function to load words from a dictionary file into a hash table
hashTable loadDictionary(const std::string &dictionaryFile) {
    std::ifstream input(dictionaryFile);
    std::string word;
    hashTable dictionary(DICT_SIZE);
    // Read each word from the dictionary file, convert to lowercase, and insert into hash table
    while (getline(input, word)) {
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        dictionary.insert(word);
    }
    return dictionary;
}

// Function to check if a character is a valid letter, dash, or apostrophe
bool isValidCharacter(char c) {
    return (c >= 'a' && c <= 'z') || (c == '-') || (c == '\'');
}

// Function to check if a character is a number
bool isNumber(char c) {
    return c >= '0' && c <= '9';
}

// Function to spell check a single line of text
void spellCheckLine(const std::string &inputLine, int lineNum, hashTable &dictionary, std::ofstream &output) {
    std::string line = inputLine + " ";
    std::string currentWord;
    bool containsNumber = false;
    // Iterate through each character in the line
    for (char c : line) {
        if (isValidCharacter(c) || isNumber(c)) {
            currentWord += c;
            if (isNumber(c)) {
                containsNumber = true;
            }
        } else {
            // Handle long words and unknown words without numbers
            // Unknown words with numbers are valid, but ignored
            if (currentWord.length() > MAX_WORD_LENGTH) {
                output << "Long word at line " << lineNum << ", starts: " << currentWord.substr(0, MAX_WORD_LENGTH) << std::endl;
            } else if (!containsNumber && !dictionary.contains(currentWord) && !currentWord.empty()) {
                output << "Unknown word at line " << lineNum << ": " << currentWord << std::endl;
            }
            currentWord = "";
            containsNumber = false;
        }
    }
}


// Function to spell check an entire document
void spellCheckDocument(const std::string &inputFile, const std::string &outputFile, hashTable &dictionary) {
    std::ifstream input(inputFile);
    std::ofstream output(outputFile);
    std::string line;
    int lineNum = 1;
    // Process each line of the input file
    while (getline(input, line)) {
        // Convert all capital letters to lowercase
        std::transform(line.begin(), line.end(), line.begin(), ::tolower);
        spellCheckLine(line, lineNum, dictionary, output);
        lineNum++;
    }
}

int main() {
    clock_t startDictTime, endDictTime, startCheckTime, endCheckTime;
    double timeElapsed;
    std::string dictionaryFile, inputFile, outputFile;
    // Get input for dictionary, input, and output files
    std::cout << "Enter name of dictionary: ";
    std::cin >> dictionaryFile;
    // Measure time taken to load dictionary into hash table
    startDictTime = clock();
    hashTable dictionary = loadDictionary(dictionaryFile);
    endDictTime = clock();
    // Calculate time elapsed to load dictionary
    timeElapsed = static_cast<double>(endDictTime - startDictTime) / CLOCKS_PER_SEC;
    std::cout << "Total time (in seconds) to load dictionary: " << timeElapsed << std::endl;
    std::cout << "Enter name of input file: ";
    std::cin >> inputFile;
    std::cout << "Enter name of output file: ";
    std::cin >> outputFile;
    // Measure time taken to spell check the input file
    startCheckTime = clock();
    spellCheckDocument(inputFile, outputFile, dictionary);
    endCheckTime = clock();
    // Calculate time elapsed for spell checking
    timeElapsed = static_cast<double>(endCheckTime - startCheckTime) / CLOCKS_PER_SEC;
    std::cout << "Total time (in seconds) to check document: " << timeElapsed << std::endl;
    return 0;
}
