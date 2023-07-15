#include "Player.h"
#include "Board.h"
#include "Side.h"
#include "Game.h"
#include <iostream>
#include <cassert>
using namespace std;


int main()
{
	HumanPlayer hp("Marge");
	SmartPlayer sp("Homer");
	Board b(3, 2);
	Game g(b, &hp, &sp);
	g.play();
}