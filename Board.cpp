#include "Board.h"
#include <iostream>
using namespace std;

Board::Board(int nHoles, int nInitialBeansPerHole) : m_holes(nHoles)
{
	// Ensure that the number of holes is at least 1
	if (m_holes <= 0) m_holes = 1;

	// Ensure that the initial number of beans per hole is non-negative
	if (nInitialBeansPerHole < 0) nInitialBeansPerHole = 0;

	// Initialize the pot for both sides
	Nside.push_back(POT);
	Sside.push_back(POT);

	// Populate the holes for both sides with the initial number of beans per hole
	for (int i = 0; i < m_holes; i++) {
		Nside.push_back(nInitialBeansPerHole);
		Sside.push_back(nInitialBeansPerHole);
	}
}

int Board::holes() const {
	return m_holes;
}

int Board::beans(Side s, int hole) const {
	if (hole < 0 || hole > m_holes) return -1;
	if (s == NORTH) return Nside[hole];
	else return Sside[translate(hole)];
}

int Board::beansInPlay(Side s) const {
	if (s == NORTH) {
		int ncounter = 0;
		// Accumulate the number of beans on the north side 
		for (int i = 1; i < Nside.size(); i++) {
			ncounter += Nside[i];
		}
		return ncounter;
	}
	else {
		int scounter = 0;
		// Accumulate the number of beans on the south side 
		for (int i = 1; i < Sside.size(); i++) {
			scounter += Sside[translate(i)];
		}
		return scounter;
	}
}

int Board::totalBeans() const {
	int counter = 0;
	for (int i = 0; i < Nside.size(); i++) {
		counter += Nside[i] + Sside[i];
	}
	return counter;
}

bool Board::sow(Side s, int hole, Side& endSide, int& endHole) {
	// Check for invalid holes for sowing
	if (hole <= 0 || hole > m_holes) return false;
	int sHole = translate(hole);
	if (s == NORTH && Nside[hole] <= 0) return false;
	if (s == SOUTH && Sside[sHole] <= 0) return false;

	Side startingSide = s;
	int hand;
	int holeIterator;

	// Store the initial side, number of beans in hand, and the hole iterator
	if (startingSide == NORTH) {
		hand = Nside[hole];
		holeIterator = hole - 1;
		Nside[hole] = 0;
	}
	else {
		hand = Sside[sHole];
		holeIterator = hole + 1;
		Sside[sHole] = 0;
	}

	while (hand != 0) {
		endSide = s;
		endHole = holeIterator;
		if (s == NORTH) {
			// Sowing on the North side excluding pot
			if (holeIterator > 0) {
				Nside[holeIterator]++; // Drop a bean in hole
				hand--;
				holeIterator--;
			}
			// Sowing on north side pot
			else if (holeIterator == 0) {
				if (startingSide == NORTH) {
					Nside[holeIterator]++;
					hand--;
				}
				s = SOUTH;
				holeIterator++;
			}
		}
		else if (s == SOUTH) {
			// Sowing on the South side excluding pot
			if (holeIterator < m_holes + 1) {
				Sside[translate(holeIterator)]++; // Drop a bean in hole
				hand--;
				holeIterator++;
			}
			// Sowing in south side pot
			else if (holeIterator == m_holes + 1) {
				if (startingSide == SOUTH) {
					Sside[0]++; // Drop a bean in pot
					hand--;
				}
				holeIterator--;
				s = NORTH;
			}
		}
	}

	// Translate the final hole index if it is the pot
	if (endHole == m_holes + 1) endHole = translate(m_holes + 1);
	return true;
}


bool Board::moveToPot(Side s, int hole, Side potOwner) { 
	// Check for valid hole
	if (hole <= 0 || hole > m_holes) return false;
	if (s == NORTH) {
		// Moving beans from the North side
		if (potOwner == NORTH) {
			Nside[0] += Nside[hole]; // adding beans to the pot
			Nside[hole] = 0; // emptying the hole
		}
		else {
			Sside[0] += Nside[hole]; // adding beans to the pot
			Nside[hole] = 0; // emptying the hole
		}
	}
	else {
		// Moving beans from the South side
		if (potOwner == NORTH) {
			Nside[0] += Sside[translate(hole)]; // adding beans to pot
			Sside[translate(hole)] = 0; // emptying the hole
		}
		else {
			Sside[0] += Sside[translate(hole)]; // adding beans to pot
			Sside[translate(hole)] = 0; // emptying the hole
		}
	}
	return true;
}
bool Board::setBeans(Side s, int hole, int beans) { 
	if (hole <= 0 || hole > m_holes || beans < 0) return false;
	if (s == NORTH) Nside[hole] = beans;
	else Sside[translate(hole)] = beans;
	return true;
}

//void Board::dump() const {
//	for (int i = 0; i < Nside.size(); i++) {
//		cout << Nside[i] << " ";
//	}
//	cout << endl << "----------------" << endl;
//	for (int i = Sside.size() - 1; i >= 0; i--) {
//		cout << Sside[i] << " ";
//	}
//	cout << endl;
//}