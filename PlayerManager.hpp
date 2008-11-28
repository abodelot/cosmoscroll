#ifndef PLAYERMANAGER_HPP
#define PLAYERMANAGER_HPP

#include <cassert>
#include <map>

#include "PlayerShip.hpp"
#include "Window.hpp"

class PlayerManager
{
public:
		
	static PlayerManager& GetInstance();

	int New();	// Création dynamique d'un nouveau player. Renvoie son id.
	void Delete(int id = 0); // Destruction du player d'id défini.
	void Clear();

	void Select(int id);
	int Selected();

	PlayerShip* GetShip();
	PlayerShip* NewShip(sf::Vector2f& offset);
	void DelShip();

	void Place();
	float GetBestTime();
	int GetControlMode();

	void SetBestTime(float time);
	void SetControlMode(int mode);

	inline int GetLastID()
	{
		return last_;	
	};
private:

	// ne pas mélanger la gestion du joueur avec PlayerShip
	struct Player
	{
		float best_time;
		PlayerShip* ship;
		int ctrl_mode;
		
		inline void Place()
		{
			assert(ship != NULL);
			static const sf::Vector2f offset (0, WIN_HEIGHT / 2.0);
			ship->SetPosition(offset);
		}
	};
	
	PlayerManager();
	PlayerManager(const PlayerManager& other);
	~PlayerManager();

	inline bool InBounds()
	{
		return current_ >= 0 && current_ <= last_;
	}

	std::map<int, Player> players_;
	typedef std::map<int, Player>::iterator PlayerIterator;
	int current_, last_;

};

typedef PlayerManager PM;

#endif /* guard PLAYERMANAGER_HPP */

