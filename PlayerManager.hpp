#ifndef PLAYERMANAGER_HPP
#define PLAYERMANAGER_HPP

#include "PlayerShip.hpp"
#include "Window.hpp"

#include <map>
#include <cassert>
//#include <iostream>


class PlayerManager
{
public:
	
	// ne pas mélanger la gestion du joueur avec PlayerShip
	struct Player
	{
		Player()
		{
			ship = NULL;
		}
		
		float best_time;
		PlayerShip* ship;
		
		inline void Place()
		{
			assert(ship != NULL);
			static const sf::Vector2f offset (0, WIN_HEIGHT / 2.0);
			ship->SetPosition(offset);
		}
	};
	
	static PlayerManager& GetInstance();

	void ReallocShip(int id)
	{
		players_[id].ship = new PlayerShip(sf::Vector2f());
		//std::cout << "new player " << id << " allocated at: " << players_[id].ship << std::endl;
	}

	int New();	// Création dynamique d'un nouveau player. Renvoie son id.
	void Delete(int id = 0); // Destruction du player d'id défini.
	Player& Get(int id = 0);
	PlayerShip* GetShip(int id = 0);
private:
	
	PlayerManager();
	PlayerManager(const PlayerManager& other);

	std::map<int, Player> players_;
	int last_;

};

typedef PlayerManager PM;

#endif /* guard PLAYERMANAGER_HPP */

