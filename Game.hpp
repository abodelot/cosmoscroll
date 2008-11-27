#ifndef GAME_HPP
#define GAME_HPP

#include <limits>
#include <list>
#include <SFML/Graphics.hpp>

#include "AbstractController.hpp"
#include "BulletManager.hpp"
#include "ControlPanel.hpp"
#include "Entity.hpp"
#include "Level.hpp"
#include "Music.hpp"
#include "ParticleSystem.hpp"
#include "Password.hpp"
#include "PlayerManager.hpp"
#include "Settings.hpp"


/*
 * Gestion du déroulement du jeu
 */
class Game
{
public:
	static Game& GetInstance();

	/*
	 * Lancer une partie de CosmoScroll
	 */
	void Run();
	
	inline sf::RenderWindow& GetApp()
	{
		return app_;
	}
	
	inline const sf::Input& GetInput() const
	{
		return app_.GetInput();
	}

	/*
	 * Ajout d'une nouvelle unité dans le jeu
	 */
	void AddEntity(Entity* entity);

	
private:
	Game();
	~Game();
	
	enum Choice
	{
		INTRO, OPTIONS, IN_GAME_MENU, MAIN_MENU, ARCADE_MODE, STORY_MODE, DOUBLE_STORY_MODE, 
		GAME_OVER, EXIT_APP, INTERTITRE, CONTINUE
	};

	enum GameMode
	{
		STORY, ARCADE, STORY2X
	};
	
	// scène d'intro
	Choice Intro();
	// menu principal
	Choice MainMenu();
	// les options de configurations
	Choice Options();
	// jouer (arcade ou story)
	Choice Play();
	// menu en cours de jeu (pause)
	Choice InGameMenu();
	// partie terminée
	Choice GameOver();
	// fin d'un niveau [Non-fin du jeu]
	Choice Intertitre();
	// lancement niveau suivant
	Choice Continue();

	bool MoreBadGuys();
	
	/*
	 * Création du vaisseau du joueur
	 */
	void Respawn();
	
	/*
	 * Suppression de toutes les unités en jeu
	 */
	void RemoveEntities();
	
	std::string MakePassword();
	
	bool UsePassword(std::string& source);
	
	bool Passwd_HACK();
	
	sf::RenderWindow app_;

	std::vector<PM::Player> players_;

	float timer_;

	std::string level_desc_; // Description du niveau courant
	GameMode mode_;
	unsigned short cur_lvl_;
	int player_1_, player_2_;
	
	// toutes les unités sont allouées dynamiquement
	std::list<Entity*> entities_;
	
	// Singletons
	AbstractController& controls_;
	BulletManager& bullets_;
	ControlPanel& panel_;
	Level& level_;
	ParticleSystem& particles_;
	PlayerManager& PM_;
	Settings& settings_;
	
	Music* music_;
};
	
#endif /* guard GAME_HPP */

