#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <chrono>
#include <cassert>

#include "AVL.h"

struct morseAndCharacter {
	char character;
	std::string morseCode;

	bool operator<(const morseAndCharacter& other) const {
		return character < other.character;
	}

	bool operator>(const morseAndCharacter& other) const {
		return character > other.character;
	}
};

class MorseAVL : public AvlTree<morseAndCharacter> {
public:
    MorseAVL() : AvlTree<morseAndCharacter>() {}

    void insertMorse(char character, const std::string &morseCode) {
        this->insert(morseAndCharacter{static_cast<char>(toupper(character)), morseCode});
    }

    // Translate a text string to Morse code
	std::string translateToMorse(const std::string &text) {
        std::string morseTranslation;
        for (char ch : text) {
            std::string morse = findCode(toupper(ch));
            if (!morse.empty()) {
                morseTranslation += morse + " ";
            } else if (ch == ' ') { // Agregar espacio entre palabras
                morseTranslation += "/ ";
            } else {
				morseTranslation += "?";
			}
        }
		if (morseTranslation.back() == ' ') morseTranslation.pop_back();
        return morseTranslation;
    }

    // Translate Morse code back to text
    std::string translateToText(const std::string &morse) {
        std::string textTranslation;
        std::string code;
        for (size_t i = 0; i < morse.length(); ++i) {
            if (morse[i] != ' ' && morse[i] != '/') {
                code += morse[i];
            } else {
                if (morse[i] == '/') { // Nuevo espacio entre palabras
                    textTranslation += ' ';
                } else if (!code.empty()) { // Fin de un carácter en Morse
                    char character = findCharacter(code, this->root);
                    if (character) {
                        textTranslation += character;
                    } else {
						textTranslation += '?';
					}
                    code.clear();
                }
            }
        }
        if (!code.empty()) { // Traducción del último código si no está seguido de espacio
            char character = findCharacter(code, this->root);
            if (character) {
                textTranslation += character;
            } else {
				textTranslation += '?';
			}
        }
        return textTranslation;
    }

	void print() const {
        std::cout << "Morse AVL Tree:\n";
        printTree(this->root);
    }

private:
    
	char findCharacter(const std::string &code, AvlTree::AvlNode* t) {
        if (t == nullptr) return '\0';
        if (code == t->element.morseCode) return t->element.character;
        char leftResult = findCharacter(code, t->left);
        return leftResult ? leftResult : findCharacter(code, t->right);
    }


    std::string findCode(const char ch) {
		AvlTree::AvlNode* currentNode = this->root;  // Assuming 'root' is a member of AvlTree

        while (currentNode) {
            if (currentNode->element.character == ch) {
                return currentNode->element.morseCode;  // Return the character if found
            }
            // If the Morse code is lexicographically less, go left; otherwise, go right
            if (ch < currentNode->element.character) {
                currentNode = currentNode->left;  // Assuming 'left' is a member of Node
            } else {
                currentNode = currentNode->right; // Assuming 'right' is a member of Node
            }
        }

        return "";  // Return null character if not found
	}

	void printTree(AvlTree::AvlNode* node, int depth = 0) const {
        if (node == nullptr) {
            return;
        }

        // Print right subtree first
        printTree(node->right, depth + 1);

        // Print the current node with indentation based on depth
        std::cout << std::setw(depth * 4) << " " << node->element.character << " (" << node->element.morseCode << ")\n";

        // Print left subtree
        printTree(node->left, depth + 1);
    }
};

void runTests() {
    MorseAVL morseTree;

    // Test: Insert characters and their Morse codes
    std::cout << "Inserting Morse codes..." << std::endl;
    auto startInsert = std::chrono::high_resolution_clock::now();
    
    morseTree.insertMorse('A', ".-");
    morseTree.insertMorse('B', "-...");
    morseTree.insertMorse('C', "-.-.");
    morseTree.insertMorse('D', "-..");
    morseTree.insertMorse('E', ".");
    morseTree.insertMorse('F', "..-.");
    morseTree.insertMorse('G', "--.");
    morseTree.insertMorse('H', "....");
    morseTree.insertMorse('I', "..");
    morseTree.insertMorse('J', ".---");
    morseTree.insertMorse('K', "-.-");
    morseTree.insertMorse('L', ".-..");
    morseTree.insertMorse('M', "--");
    morseTree.insertMorse('N', "-.");
    morseTree.insertMorse('O', "---");
    morseTree.insertMorse('P', ".--.");
    morseTree.insertMorse('Q', "--.-");
    morseTree.insertMorse('R', ".-.");
    morseTree.insertMorse('S', "...");
    morseTree.insertMorse('T', "-");
    morseTree.insertMorse('U', "..-");
    morseTree.insertMorse('V', "...-");
    morseTree.insertMorse('W', ".--");
    morseTree.insertMorse('X', "-..-");
    morseTree.insertMorse('Y', "-.--");
    morseTree.insertMorse('Z', "--..");
    morseTree.insertMorse(' ', "/");
    morseTree.insertMorse('0', "-----");
    morseTree.insertMorse('1', ".----");
    morseTree.insertMorse('2', "..---");
    morseTree.insertMorse('3', "...--");
    morseTree.insertMorse('4', "....-");
    morseTree.insertMorse('5', ".....");
    morseTree.insertMorse('6', "-....");
    morseTree.insertMorse('7', "--...");
    morseTree.insertMorse('8', "---..");
    morseTree.insertMorse('9', "----.");
    
    auto endInsert = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> insertDuration = endInsert - startInsert;
    std::cout << "Insertions completed in " << insertDuration.count() << " ms." << std::endl;
	std::cout << std::endl;

    // Test: Translate text to Morse code
    std::string text = "HELLO WORLD";
    std::cout << "Translating text to Morse...: " << text << std::endl;
    auto startTranslateToMorse = std::chrono::high_resolution_clock::now();
    
    std::string morse = morseTree.translateToMorse(text);
    
    auto endTranslateToMorse = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> translateToMorseDuration = endTranslateToMorse - startTranslateToMorse;
    std::cout << "Morse translation: \"" << morse << "\"" << std::endl;
    std::cout << "Translation completed in " << translateToMorseDuration.count() << " ms." << std::endl;
	std::cout << std::endl;

    // Test: Translate Morse code back to text
    std::cout << "Translating Morse to text...: " << morse << std::endl;
    auto startTranslateToText = std::chrono::high_resolution_clock::now();
    
    std::string translatedText = morseTree.translateToText(morse);
    
    auto endTranslateToText = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> translateToTextDuration = endTranslateToText - startTranslateToText;
    std::cout << "Text translation: \"" << translatedText << "\"" << std::endl;
    std::cout << "Translation completed in " << translateToTextDuration.count() << " ms." << std::endl;
	std::cout << std::endl;

    // Validate that translation back matches original text
    if (translatedText == text) {
        std::cout << "Test passed: Translated text matches original text." << std::endl;
    } else {
        std::cout << "Test failed: Translated text does not match original text." << std::endl;
    }
	std::cout << std::endl;

	// More tests

	// Test translation to Morse code
	std::cout << "Testing from text to morse code..." << std::endl;

	startTranslateToMorse = std::chrono::high_resolution_clock::now();

    std::string textToMorseTest1 = "ABC";
    std::string expectedMorse1 = ".- -... -.-.";
    std::string resultMorse1 = morseTree.translateToMorse(textToMorseTest1);
    assert(resultMorse1 == expectedMorse1 && "Test 1 Failed: Expected '.- -... -.-.'");

    std::string textToMorseTest2 = "A1 B2 C3";
    std::string expectedMorse2 = ".- .---- / -... ..--- / -.-. ...--";
    std::string resultMorse2 = morseTree.translateToMorse(textToMorseTest2);
    assert(resultMorse2 == expectedMorse2 && "Test 2 Failed: Expected '.- / -... / -.-.'");

	endTranslateToMorse = std::chrono::high_resolution_clock::now();
	translateToMorseDuration = endTranslateToMorse - startTranslateToMorse;
	std::cout << "Tests completed in " << translateToMorseDuration.count() << " ms." << std::endl;
	std::cout << std::endl;

    // Test translation from Morse code back to text
	std::cout << "Testing from morse code to text..." << std::endl;

	startTranslateToText = std::chrono::high_resolution_clock::now();

    std::string morseToTextTest1 = ".- -... -.-.";
    std::string expectedText1 = "ABC";
    std::string resultText1 = morseTree.translateToText(morseToTextTest1);
    assert(resultText1 == expectedText1 && "Test 3 Failed: Expected 'ABC'");

	morseToTextTest1 = ".... . .-.. .-.. ---";
    expectedText1 = "HELLO";
    resultText1 = morseTree.translateToText(morseToTextTest1);
    assert(resultText1 == expectedText1 && "Test 4 Failed: Expected 'HELLO'");

    std::string morseToTextTest2 = ".- / -... / -.-.";
    std::string expectedText2 = "A B C";
    std::string resultText2 = morseTree.translateToText(morseToTextTest2);
    assert(resultText2 == expectedText2 && "Test 5 Failed: Expected 'A B C'");

	endTranslateToText = std::chrono::high_resolution_clock::now();
    translateToTextDuration = endTranslateToText - startTranslateToText;
    std::cout << "Tests completed in " << translateToTextDuration.count() << " ms." << std::endl;
	std::cout << std::endl;

	// Tests for unknown characters
	std::cout << "Testing for unknown characters..." << std::endl;

	startTranslateToText = std::chrono::high_resolution_clock::now();

	text = "!%$()";
	morse = morseTree.translateToMorse(text);
	assert(morse == "?????" && "Test 6 Failed: Expected '????' for unknown character");

	std::string morseToTextTest3 = "----";
    std::string expectedText3 = "?";
    std::string resultText3 = morseTree.translateToText(morseToTextTest3);
    assert(resultText3 == expectedText3 && "Test 7 Failed: Expected '?' for unknown Morse code");
	std::cout << std::endl;

	// Tests for unknown characters (mixed)
	std::cout << "Testing for unknown characters (mixed)..." << std::endl;
	text = "!%$()EA";
	morse = morseTree.translateToMorse(text);
	assert(morse == "?????. .-" && "Test 8 Failed: Expected '?????.' for '!%$()E' character");

	endTranslateToText = std::chrono::high_resolution_clock::now();
    translateToTextDuration = endTranslateToText - startTranslateToText;
    std::cout << "Tests completed in " << translateToTextDuration.count() << " ms." << std::endl;
	std::cout << std::endl;

	std::cout << "All tests passed!" << std::endl;

	morseTree.print();
}

int main() {
    runTests();
    return 0;
}
