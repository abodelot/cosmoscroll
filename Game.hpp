#ifndef GAME_HPP
#define GAME_HPP

#include <limits>
#include <list>
#include <SFML/Graphics.hpp>

#include "AbstractController.hpp"
#include "BulletManager.hpp"
#include "ControlPanel.hpp"
#include "Entity.hpp"
#include "LevelManager.hpp"
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
		INTRO,
		MAIN_MENU,
		OPTIONS,
		ARCADE_MODE,
		STORY_MODE,
		DOUBLE_STORY_MODE,
		PONG_MODE,
		IN_GAME_MENU,
		END_PLAY,
		SELECT_LEVEL,
		LEVEL_CAPTION,
		END_ARCADE,
		EXIT_APP
	};

	enum GameMode
	{
		STORY, ARCADE, STORY2X, PONG
	};
	
	// scène d'intro
	Choice Intro();
	// menu principal
	Choice MainMenu();
	// les options de configurations
	Choice Options();
	// jouer (arcade ou story)
	//Choice Play();

	Choice PlayArcade();
	Choice PlayStory();
	
	Choice PlayPong();


	// menu en cours de jeu (pause)
	Choice InGameMenu();
	
	/*
	// partie terminée
	Choice GameOverArcade();
	Choice GameOverStory();
	// fin d'un niveau [Non-fin du jeu]
	Choice Intertitre();
	// lancement niveau suivant
	Choice Continue();
	*/
	
	// partie terminée (succès comme défaite)
	Choice EndPlay();
	// sélection d'un niveau
	Choice SelectLevel();
	// Introduction d'un niveau
	Choice LevelCaption();
	// résultat d'une partie en mode arcade
	Choice EndArcade();
	
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



	float timer_;

	GameMode mode_;
	
	inline GameMode& GetMode()
	{
		return mode_;
	}
	
	inline void SetMode(GameMode mode)
	{
		mode_ = mode;
	}
	int current_level_, player_1_, player_2_;
	
	// toutes les unités sont allouées dynamiquement
	std::list<Entity*> entities_;
	
	// Singletons
	AbstractController& controls_;
	BulletManager& bullets_;
	ControlPanel& panel_;
	LevelManager& levels_;
	ParticleSystem& particles_;
	PlayerManager& PM_;
	Settings& settings_;
#ifndef NO_AUDIO
	Music* music_;
#endif
};
	
#endif /* guard GAME_HPP */

