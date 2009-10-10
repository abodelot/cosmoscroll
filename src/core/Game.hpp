#ifndef GAME_HPP
#define GAME_HPP

#include <list>
#include <SFML/Graphics.hpp>

#include "Input.hpp"
#include "ControlPanel.hpp"
#include "ParticleSystem.hpp"
#include "LevelManager.hpp"
#include "../utils/DumbMusic.hpp"
#include "../entities/PlayerShip.hpp"

class EntityManager;

/**
 * Gestion du déroulement du jeu
 */
class Game
{
public:
	static Game& GetInstance();

	/**
	 * Lancer une partie de CosmoScroll
	 */
	void Run();

	inline sf::RenderWindow& GetApp()
	{
		return app_;
	}

	void NotifyPlayerDead();

	inline const sf::Input& GetInput() const
	{
		return app_.GetInput();
	}

	Entity* GetPlayerShip() const;

private:
	Game();
	~Game();

	bool LoadConfig(const char* filename);

	void WriteConfig(const char* filename) const;

	enum Scene
	{
		INTRO,
		MAIN_MENU,
		OPTIONS,
		PLAY,
		IN_GAME_MENU,
		ABOUT,
		END_PLAY,
		SELECT_LEVEL,
		LEVEL_CAPTION,
		ARCADE_RESULT,
		EXIT_APP
	};

	enum GameMode
	{
		STORY, ARCADE
	};


	/* Gestion des scènes */

	// scène d'intro
	Scene Intro();
	// menu principal
	Scene MainMenu();
	// les options de configurations
	Scene Options();
	// jouer
	Scene Play();
	// menu en cours de jeu (pause)
	Scene InGameMenu();
	// fin de partie
	Scene EndPlay();
	// sélection d'un niveau
	Scene SelectLevel();
	// Introduction d'un niveau
	Scene LevelCaption();
	// résultat d'une partie en mode arcade
	Scene ArcadeResult();
	// About
	Scene About();


	/* Gestion de la boucle de jeu */

	// initialiser le jeu avant de lancer une partie
	// (nettoyage des parties précédentes, allocation des joueurs)
	void Init();

	/**
	 * Allouer et jouer une musique
	 * @param[in] music_name: identifiant de la musique
	 */
	void LoadMusic(const char* music_name);

	/**
	 * Désallouer et stopper la musique en cours
	 */
	void StopMusic();

	void SetBackgroundColor(const sf::Color& topcolor, const sf::Color& bottomcolor);

	void TakeScreenshot(const char* directory);

	sf::RenderWindow app_;

	float timer_;
	GameMode mode_;

	int current_level_;
	int last_level_reached_;
	float best_arcade_time_;
	std::string music_name_;

	PlayerShip* player_;

	// condition pour stopper la boucle de jeu
	bool (Game::*p_StopPlay_)();

	bool ArcadeMoreBadGuys();
	bool StoryMoreBadBuys();

	bool player_dead_;
	bool fullscreen_;

	// temp hack
	bool GetAction(Input::Action& action);

	// Singletons
	Input& input_;
	ControlPanel& panel_;
	LevelManager& levels_;
	ParticleSystem& particles_;
	EntityManager& entitymanager_;
#ifndef NO_DUMB_MUSIC
	DumbMusic* music_;
#endif
	sf::Shape background_;
};

#endif // GAME_HPP
