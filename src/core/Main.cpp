#include "Game.hpp"


int main(int argc, char* argv[])
{
	Game& game = Game::GetInstance();
	game.Init(argv[0]);
	game.Run();
	return 0;
}

