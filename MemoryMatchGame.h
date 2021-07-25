#include <string>
#include <vector>

#pragma once
class MemoryMatchGame {
	public:
		void start();
	private:
		int vectorSize;
		int speed;
		int longestWord;
		std::string theme;
		std::string faceCardName;
		std::vector<std::string> allAnswerCards;
		std::vector<std::vector<std::string>> displayAnswerCards;
		std::vector<std::vector<std::string>> displayBoardCards;
		std::vector<std::vector<std::string>> displayBlankCards;
		void menu();
		void generateVectors();
		void displayBoard();
		void readFromThemeFile();
};