#include "Game.hpp"


int main(int argc, char* argv[])
{
	short int ls = 0;
	if (argc > 1)
		++ls;
	Game::GetInstance(ls).Run();
//	game.Init(argv[0]);
	return 0;
}

