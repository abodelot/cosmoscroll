#include "Game.hpp"


int main(int argc, char* argv[])
{
	short int ls = 0;
	if (argc > 1)
		++ls;
	Game& game = Game::GetInstance();
	game.Init("./config", ls);
	game.Run();
	return 0;
}

