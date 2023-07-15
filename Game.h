#ifndef GAME_H
#define GAME_H

#include "Player.h"

class Game {
private:
	Board m_board;
	Player* m_south;
	Player* m_north;
	

public:
	Game(const Board& b, Player* south, Player* north);

	void display() const;

	void status(bool& over, bool& hasWinner, Side& winner) const;

	bool move(Side s);

	void play();

	int beans(Side s, int hole) const;
};




#endif
