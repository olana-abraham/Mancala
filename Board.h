#ifndef BOARD_H
#define BOARD_H

#include "Side.h"
#include <vector>
using namespace std;

class Board {
private:
	int m_holes;
	int translate(int hole) const{
		if (hole == 0) return 0;
		return m_holes - hole + 1;
	}

	vector<int> Nside;
	vector<int> Sside;

public:
	Board(int nHoles, int nInitialBeansPerHole);

	int holes() const;
	int beans(Side s, int hole) const;
	int beansInPlay(Side s) const;
	int totalBeans() const;
	bool sow(Side s, int hole, Side& endSide, int& endHole);
	bool moveToPot(Side s, int hole, Side potOwner);
	bool setBeans(Side s, int hole, int beans);
	//void dump() const;
};





#endif // !BOARD_H

