#include "Game.h"
#include <string>
#include <iostream>
using namespace std;


Game::Game(const Board& b, Player* south, Player* north) 
	: m_board(b), m_south(south), m_north(north) 
{
}

void Game::display() const {
	cout << endl;
	for (int i = 0; i < m_board.holes(); i++) {
		cout << " ";
	}
	cout << m_north->name() << endl << " ";
	for (int i = 1; i < m_board.holes() + 1; i++) {
		cout << m_board.beans(NORTH, i) << " ";
	}
	cout << endl << m_board.beans(NORTH, 0);
	for (int i = 0; i < m_board.holes(); i++) {
		cout << "  ";
	}
	cout << m_board.beans(SOUTH, 0) << endl << " ";
	for (int i = 1; i < m_board.holes() + 1; i++) {
		cout << m_board.beans(SOUTH, i) << " ";
	}
	cout << endl;
	for (int i = 0; i < m_board.holes(); i++) {
		cout << " ";
	}
	cout << m_south->name() << endl << endl;
}

void Game::status(bool& over, bool& hasWinner, Side& winner) const {
	// if the game is over determine a winner if there is one
	if (m_board.beansInPlay(NORTH) == 0 || m_board.beansInPlay(SOUTH) == 0) {
		over = true;
		if (m_board.beans(NORTH, 0) > m_board.beans(SOUTH, 0)) {
			hasWinner = true;
			winner = NORTH;
		}
		else if (m_board.beans(NORTH, 0) < m_board.beans(SOUTH, 0)) {
			hasWinner = true;
			winner = SOUTH;
		}
		else {
			hasWinner = false;
		}
	}
	else {
		over = false;
	}
}

bool Game::move(Side s) {
	Side endSide;
	int endHole;
	string temp;
	
	// Check various conditions to determine the game state and available moves
	bool capture;
	bool nempty = (m_board.beansInPlay(NORTH) == 0);
	bool sempty = (m_board.beansInPlay(SOUTH) == 0);
	int pmove = -1;

	// If NORTH has no beans in play, sweep all SOUTH's beans to its pot
	if (nempty) {
		for (int i = 1; i < m_board.holes() + 1; i++) {
			m_board.moveToPot(SOUTH, i, SOUTH);
		}
		display();
		return false;
	}

	// If SOUTH has no beans in play, move all NORTH's beans to its pot
	if (sempty) {
		for (int i = 1; i < m_board.holes() + 1; i++) {
			m_board.moveToPot(NORTH, i, NORTH);
		}
		display();
		return false;
	}

	if (s == NORTH) {

		// choose a move based on the player type
		if (nempty == false) pmove = m_north->chooseMove(m_board, s);

		cout << m_north->name() << " chose Hole " << pmove << endl;

		// make the move
		m_board.sow(s, pmove, endSide, endHole);
		capture = m_board.beans(s, endHole) == 1 && m_board.beans(opponent(s), endHole) > 0 && endSide == s;

		// if the last bean lands in the pot move again
		if (endHole == 0) {
			display();
			return move(s);
		}
		else {
			if (capture == false && nempty == false) display();
		}

		// if the conditions to capture are true than capture
		if (capture) {
			m_board.moveToPot(SOUTH, endHole, NORTH);
			m_board.moveToPot(NORTH, endHole, NORTH);
			display();
		}

		// if the game is over at this stage then call the function again
		if (m_board.beansInPlay(s) == 0 || m_board.beansInPlay(opponent(s)) == 0) move(s);
	
	}
	if (s == SOUTH) {

		if (sempty == false) pmove = m_south->chooseMove(m_board, s);

		cout << m_south->name() << " chose Hole " << pmove << endl;

		m_board.sow(s, pmove, endSide, endHole);
		capture = m_board.beans(s, endHole) == 1 && m_board.beans(opponent(s), endHole) > 0 && endSide == s;

		if (endHole == 0) {
			display();
			return move(s);
		}
		else {
			if (capture == false && sempty == false) display();
		}
		
		if (capture) {
			m_board.moveToPot(NORTH, endHole, SOUTH);
			m_board.moveToPot(SOUTH, endHole, SOUTH);
			display();
		}

		if (m_board.beansInPlay(s) == 0 || m_board.beansInPlay(opponent(s)) == 0) move(s);
	}
	
	return true;
}

void Game::play() {
	bool over;
	bool hasWinner;
	Side winner;
	string temp;

	bool nonInteractive = m_south->isInteractive() == false && m_north->isInteractive() == false;
	
	display();


	do {
		// if both players are cpu's than prompt the user to continue after each move
		if (nonInteractive) {
			cout << "Press enter to continue:";
			cin.ignore(10000, '\n');
		}

		move(SOUTH);

		status(over, hasWinner, winner);
		if (over == true) break;

		if (nonInteractive) {
			cout << "Press enter to continue:";
			cin.ignore(10000, '\n');
		}

		move(NORTH);

		status(over, hasWinner, winner);
	} while (over == false);

	// print winner if there is one
	if (hasWinner) {
		if (winner == NORTH) {
			cout << m_north->name() << " Wins!" << endl;
		}
		else {
			cout << m_south->name() << " Wins!" << endl;
		}
	}
	else {
		cout << "Draw!" << endl;
	}
	
}

int Game::beans(Side s, int hole) const {
	if (hole < 0 || hole > m_board.holes()) return -1;
	if (s == NORTH) return m_board.beans(NORTH, hole);
	else return m_board.beans(SOUTH, hole);
}