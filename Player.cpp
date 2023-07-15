#include "Player.h"
#include <iostream>

// Player
Player::Player(string name) : m_name(name) {}

string Player::name() const{
	return m_name;
}

bool Player::isInteractive() const {
	return false;
}

Player::~Player() {}

// Human Player
HumanPlayer::HumanPlayer(string name) : Player(name) {}

bool HumanPlayer::isInteractive() const{
	return true;
}

int HumanPlayer::chooseMove(const Board& b, Side s) const {
	int userHole;
	bool validHole = true;
	// Ask user for hole number while it's a valid hole
	do {
		cout << "Enter a hole number, " << name() << ": ";
		cin >> userHole;
		cin.ignore(10000, '\n');
		if (userHole > b.holes() || userHole <= 0 || b.beans(s,userHole) == 0) {
			validHole = false;
		}
		else {
			validHole = true;
		}
	} 
	while (validHole == false);
	return userHole;
}

// Bad Player
BadPlayer::BadPlayer(string name) : Player(name) {}

int BadPlayer::chooseMove(const Board& b, Side s) const {
	int randHole = -1;
	// choses random hole from 1 to number of holes while its a valid hole
	do {
		randHole = rand() % b.holes() + 1;
	} while (b.beans(s, randHole) == 0);
	return randHole;
}

// Smart Player
SmartPlayer::SmartPlayer(string name) : Player(name) {}

int SmartPlayer::eval(const Board& b, Side s) const{
	// Difference in pots between player and opponent
	int potDiff = b.beans(s, 0) - b.beans(opponent(s), 0);
	// If the player has more beans in their pot and more beans in play than the opponent,
	// consider it a winning state for the player
	if (potDiff > 0 && b.beansInPlay(s) > b.beansInPlay(opponent(s))) return INT_MAX;
	// If the opponent has more beans in their pot and more beans in play than the player,
	// consider it a losing state for the player
	else if (potDiff < 0 && b.beansInPlay(s) < b.beansInPlay(opponent(s))) return INT_MIN;
	// otherwise consider it neutral
	else return 0;
}

void SmartPlayer::chooseMoveHelper(const Board& b, Side s, int &bestHole, int &value, int depth, Side startingSide) const{

	// If either player has no beans in play, the game is over
	// Set the bestHole to -1 and calculate the value based on the evaluation function
	if (b.beansInPlay(s) == 0 || b.beansInPlay(opponent(s)) == 0) {
		bestHole = -1;
		value = eval(b, s);
		return;
	}
	// If the max depth is reached
	// Set the bestHole to -1 and calculate the value based on the evaluation function
	if (depth == 5) {
		bestHole = -1;
		value = eval(b, s);
		return;
	}

	Board tempB(b); // Create a temp board to simulate moves
	Side endSide;
	int endHole;

	// if it's the player's turn find the move with the max value
	if (s == startingSide) {


		value = INT_MIN;

		for (int h = 1; h < tempB.holes() + 1; h++) {

			int h2;
			int v2 = INT_MIN;

			// Check if hole is valid
			if (tempB.beans(s, h) > 0) {

				tempB.sow(s, h, endSide, endHole);
				bool capture = tempB.beans(s, endHole) == 1 && tempB.beans(opponent(s), endHole) > 0 && endSide == s;

				// If the last seed is dropped in the player's own pot, continue with the same player's turn
				if (endSide == s && endHole == 0) {
					chooseMoveHelper(tempB, s, h2, v2, depth, startingSide);
				}

				if (capture) {
					tempB.moveToPot(opponent(s), endHole, s);
					tempB.moveToPot(s, endHole, s);
				}

				//switch to the opponent's turn
				chooseMoveHelper(tempB, opponent(s), h2, v2, depth + 1, startingSide);

				// Update the best move if a higher value is found
				if (v2 >= value) {
					bestHole = h;
					value = v2;
				}
			}
			tempB = b; // Unmake the move
		}
	}

	// If it's the opponent's turn, find the move with the minimum value
	if (s == opponent(startingSide)) {
		value = INT_MAX;

		for (int h = 1; h < tempB.holes() + 1; h++) {
			int h2;
			int v2 = INT_MAX;

			if (tempB.beans(s, h) > 0) {

				tempB.sow(s, h, endSide, endHole);
				bool capture = tempB.beans(s, endHole) == 1 && tempB.beans(opponent(s), endHole) > 0 && endSide == s;

				if (capture) {
					tempB.moveToPot(opponent(s), endHole, s);
					tempB.moveToPot(s, endHole, s);
				}


				if (endSide == s && endHole == 0) {
					chooseMoveHelper(tempB, s, h2, v2, depth, startingSide);
				}

				chooseMoveHelper(tempB, opponent(s), h2, v2, depth + 1, startingSide);

				//  Update the best move if a lower value is found
				if (v2 <= value) {
					bestHole = h;
					value = v2;
				}
			}
			tempB = b;
		}

	}

	return;
}

int SmartPlayer::chooseMove(const Board& b, Side s) const {
	int bestHole;
	int value;

	chooseMoveHelper(b, s, bestHole, value, 0, s);

	return bestHole;
}
