#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include "Board.h"
using namespace std;


class Player {
private:
	string m_name;

public:
	Player(std::string name);

	std::string name() const;

	virtual bool isInteractive() const;

	virtual int chooseMove(const Board& b, Side s) const = 0;

	virtual ~Player();
};


class HumanPlayer : public Player {
public:
	HumanPlayer(string name);

	virtual bool isInteractive() const;

	virtual int chooseMove(const Board& b, Side s) const;

};

class BadPlayer : public Player {
public:
	BadPlayer(string name);

	virtual int chooseMove(const Board& b, Side s) const;

};

class SmartPlayer : public Player {
private:

	int eval(const Board& b, Side s) const;

	void chooseMoveHelper(const Board& b, Side s, int &bestHole, int &value, int depth, Side startingSide) const;

public:
	SmartPlayer(string name);

	virtual int chooseMove(const Board& b, Side s) const;
};

#endif // !PLAYER_H

