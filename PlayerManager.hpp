#ifndef PLAYERMANAGER_HPP
#define PLAYERMANAGER_HPP

#include <map>

#include "PlayerShip.hpp"
#include "Window.hpp"

class PlayerManager
{
public:
	
	// ne pas mélanger la gestion du joueur avec PlayerShip
	struct Player
	{
		float best_time;
		PlayerShip* ship;
		
		inline void Place()
		{
			static const sf::Vector2f offset (0, WIN_HEIGHT / 2.0);
			if (ship)
				ship->SetPosition(offset);
		}
	};
	
	static PlayerManager& GetInstance();

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

