#include "MemoryMatchGame.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <random>
#include <chrono>
#include <thread>
using namespace std;

void MemoryMatchGame::menu() {
	// Prompt the user to enter the level of play
	 cout << "Enter the level of play:" <<  endl;
	 cout << "1) 4 x 4 (easy), 2) 6 x 6 (moderate), 3) 8 x 8 (hard)" <<  endl;
	
	int levelOfPlay;
	while (true) {
		 cin >> levelOfPlay;

		if (levelOfPlay == 1) { // Easy mode
			this->vectorSize = 4;
			break;
		}

		else if (levelOfPlay == 2) { // Moderate mode
			this->vectorSize = 6;
			break;
		}

		else if (levelOfPlay == 3) { // Hard mode
			this->vectorSize = 8;
			break;
		}

		else { // User failed to enter valid input
			 cout << "Invalid input. Please try again with either:\n" <<
				"1) 4 x 4, 2) 6 x 6, or 3) 8 x 8" <<  endl;
		}
	}

	// Prompt the user to enter the speed of play
	cout << "Enter the speed of play:" <<  endl;
	cout << "1) 6 sec (easy), 2) 4 sec (moderate), 3) 2 sec (hard)" <<  endl;

	int speedChoice;

	while (true) {
		 cin >> speedChoice;

		// We don't need multiple if / else if statements since we can assign directly
		// and therefore only need to make sure that the input is valid
		if (speedChoice == 1) {
			this->speed = 6;
			break;
		}

		else if (speedChoice == 2) {
			this->speed = 4;
			break;
		}

		else if (speedChoice == 3) {
			this->speed = 2;
			break;
		}

		else { // User failed to enter valid input
			 cout << "Invalid input. Please try again with either:\n" <<
				"1) 6 sec, 2) 4 sec, 3) 2 sec" <<  endl;
		}
	}

	// Prompt the user for the theme of the game
	 cout << "Enter the theme (1, 2, or 3):" <<  endl;
	 cout << "1) Periodic Table of Elements, 2) Animals" <<  endl; // Add more themes later!

	int themeChoice;
	while (true) {
		 cin >> themeChoice;

		if (themeChoice == 1) {
			this->theme = "PeriodicTable.csv";
			break;
		}

		else if (themeChoice == 2) {
			this->theme = "animals.txt";
			break;
		}

		else {
			 cout << "Invalid input. Please try again with either:\n" <<
				"'1', ..." <<  endl;
		}
	}
}

void MemoryMatchGame::generateVectors() {
	// Load data from theme file (.csv) into allAnswerCards vector
	readFromThemeFile();

	// Get a time-based seed
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();

	// Shuffle allAnswerCards using the default_random_engine and the above time-based seed
	shuffle(allAnswerCards.begin(), allAnswerCards.end(), default_random_engine(seed));

	vector<string> temp;
	int iterationLength = (this->vectorSize * this->vectorSize) / 2;
	
	// Push two of each term in allAnswerCards for as long as
	// determined by the iterationLength
	for (int i = 0; i < iterationLength; i++) {
		temp.push_back(allAnswerCards[i]);
		temp.push_back(allAnswerCards[i]);
	}

	// Resize 2D vector in order to use it
	this->displayAnswerCards.resize(this->vectorSize);
	this->displayBoardCards.resize(this->vectorSize);
	this->displayBlankCards.resize(this->vectorSize);

	for (int i = 0; i < this->vectorSize; i++) {
		this->displayAnswerCards[i].resize(this->vectorSize);
		this->displayBoardCards[i].resize(this->vectorSize);
		this->displayBlankCards[i].resize(this->vectorSize);
	}

	// Shuffle temp
	shuffle(temp.begin(), temp.end(), default_random_engine(seed));

	int z = 0;
	// Load terms from temporary vector into displayAnswerCards 2D vector
	for (int i = 0; i < this->vectorSize; i++) {
		for (int j = 0; j < this->vectorSize; j++) {
			this->displayAnswerCards[i][j] = temp[j + z];
			this->displayBoardCards[i][j] = this->faceCardName;
			this->displayBlankCards[i][j] = "";
		}
		z += 4;
		cout << endl;
	}
}

void MemoryMatchGame::readFromThemeFile() {
	ifstream fin;
	string line;
	fin.open(theme);

	string field;

	int counter = 0;
	this->longestWord = -1;
	while (getline(fin, field, '\n')) {
		if (this->longestWord == -1) {
			this->longestWord = field.length();
		}

		allAnswerCards.push_back(field);

		if (field.length() > longestWord) {
			this->longestWord = field.length();
		}
		counter++;
	}
	this->faceCardName = this->allAnswerCards[0];

	this->allAnswerCards.erase(this->allAnswerCards.begin());
}

void MemoryMatchGame::displayBoard() {
	int numSpaces;

	for (int i = 0; i < vectorSize; i++) {
		cout << (i + 1) << "> ";
		for (int j = 0; j < vectorSize; j++) {
			string curCard = this->displayBoardCards[i][j];
			int curLength = curCard.length();
			numSpaces = (this->longestWord - curLength) / 2;
			// If the remainder of the operation: (longest word - current word length) / 2 
			// has no remainder, add spaces within the brackets
			if ((this->longestWord - curLength) % 2 == 0) {
				cout << "[" << string(numSpaces, ' ') << curCard 
					<< string(numSpaces, ' ') << "]" << " ";
			}
			// Otherwise, add an extra space outside of the backets
			else {
				int spacesLeft = (longestWord + 3) - (curLength + (longestWord - curLength) + 2);
				cout << "[" << string(numSpaces, ' ') << curCard
					<< string((numSpaces + spacesLeft), ' ') << "]" << " ";
			}
		}
		cout << "\n" << endl;
	}
	cout << string(3, ' ');
	// Write row numbers
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < vectorSize; j++) {
			if (i == 0) {
				cout << "^" << string(longestWord + 2, ' ');
			}

			else {
				cout << (j + 1) << string(longestWord + 2, ' ');
			}
		}
		cout << endl;
		cout << string(3, ' ');
	}
	cout << endl;
}

void MemoryMatchGame::start() {
	menu();
	generateVectors();

	int x1, y1, x2, y2;
	while (true) {
		system("CLS");
		displayBoard();
		
		if (this->displayBoardCards == this->displayAnswerCards) {
			cout << "Game finished! Nice job!" << endl;
			break;
		}
		
		cout << "Enter the x-position of the first card: ";
		cin >> y1;
		if (y1 > this->vectorSize + 1) { 
			cout << "Invalid position! Enter coordinates again" << endl;
			this_thread::sleep_for(chrono::milliseconds(500));
		}

		cout << "Enter the y-position of the first card: ";
		cin >> x1;
		if (x1 > this->vectorSize + 1) {
			cout << "Invalid position! Enter coordinates again" << endl;
			this_thread::sleep_for(chrono::milliseconds(500));
		}
		
		cout << "Enter the x-position of the second card: ";
		cin >> y2;
		if (y2 > this->vectorSize + 1) {
			cout << "Invalid position! Enter coordinates again" << endl;
			this_thread::sleep_for(chrono::milliseconds(500));
		}

		cout << "Enter the y-position of the second card: ";
		cin >> x2;
		if (x2 > this->vectorSize + 1) {
			cout << "Invalid position! Enter coordinates again" << endl;
			this_thread::sleep_for(chrono::milliseconds(500));
		}

		if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0) {
			break;
		}

		system("CLS");

		string card1 = this->displayAnswerCards[x1 - 1][y1 - 1];
		string card2 = this->displayAnswerCards[x2 - 1][y2 - 1];

		vector<vector<string>> temp = this->displayBlankCards;
		vector<vector<string>> temp2 = this->displayBoardCards;

		if (card1 == card2) {
			for (int i = 0; i < 4; i++) {
				this->displayBlankCards[x1 - 1][y1 - 1] = card1;
				this->displayBlankCards[x2 - 1][y2 - 1] = card2;
				this->displayBoardCards = this->displayBlankCards;
				system("CLS");
				displayBoard();
				this_thread::sleep_for(chrono::milliseconds(500));
				this->displayBoardCards = temp;
				system("CLS");
				displayBoard();
				this_thread::sleep_for(chrono::milliseconds(500));
			}

			system("CLS");
			this->displayBlankCards = temp;
			this->displayBoardCards = temp2;
			this->displayBoardCards[x1 - 1][y1 - 1] = card1;
			this->displayBoardCards[x2 - 1][y2 - 1] = card2;
		}

		else {
			for (int i = 0; i < this->speed; i++) {
				this->displayBlankCards[x1 - 1][y1 - 1] = card1;
				this->displayBlankCards[x2 - 1][y2 - 1] = card2;
				this->displayBoardCards = this->displayBlankCards;
				system("CLS");
				displayBoard();
				this_thread::sleep_for(chrono::milliseconds(500));
				this->displayBoardCards = temp;
				system("CLS");
				displayBoard();
				this_thread::sleep_for(chrono::milliseconds(500));
			}

			system("CLS");
			this->displayBlankCards = temp;
			this->displayBoardCards = temp2;
		}
	}
}