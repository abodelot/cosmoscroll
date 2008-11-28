#include "PlayerManager.hpp"

PlayerManager& PlayerManager::GetInstance()
{
    static PlayerManager self;
    return self;
}

int PlayerManager::New()
{
	// HACK: Virer le offset obligatoire à la création?
	static sf::Vector2f offset(0.f, 0.f);
	Player player;
	players_[++last_] = player;
	players_[last_].best_time = 0;
	players_[last_].ship = new PlayerShip(offset);

	return last_;
}

void PlayerManager::Delete(int id)
{
	delete players_[id].ship;
	players_.erase(id);
}

PM::Player& PlayerManager::Get(int id)
{
	return players_[id];
}

PlayerShip* PlayerManager::GetShip(int id)
{
	PlayerShip* ship = players_[id].ship;
	assert(ship != NULL);
	//std::cout << "return player ship: " << ship << std::endl;
	return players_[id].ship;
}


PlayerManager::PlayerManager()
{
	last_ = -1;
}

