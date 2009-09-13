#ifndef GAME_HPP
#define GAME_HPP

#include <limits>
#include <list>
#include <SFML/Graphics.hpp>

#include "AbstractController.hpp"
#include "ControlPanel.hpp"
#include "../entities/Entity.hpp"
#include "LevelManager.hpp"
#include "../utils/Music.hpp"
#include "ParticleSystem.hpp"
#include "../entities/PlayerShip.hpp"
#include "../entities/EntityManager.hpp"

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
		STORY, STORY2X, ARCADE, PONG
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

	sf::RenderWindow app_;

	float timer_;
	GameMode mode_;

	int current_level_;
	int last_level_reached_;
	float best_arcade_time_;
	std::string music_name_;

	PlayerShip *player1_, *player2_;


	/* pointeurs de méthodes pour la boucle de jeu */

	// transmettre les actions de la boucle d'évènements aux joueurs
	void (Game::*p_ForwardAction_)(AC::Action action, AC::Device device);

	void ForwardAction1P(AC::Action action, AC::Device device);
	void ForwardAction2P(AC::Action action, AC::Device device);

	// condition pour stopper la boucle de jeu
	bool (Game::*p_StopPlay_)();

	bool ArcadeMoreBadGuys();
	bool StoryMoreBadBuys();

	bool player_dead_;

	// Singletons
	AbstractController& controls_;
	ControlPanel& panel_;
	LevelManager& levels_;
	ParticleSystem& particles_;
	EntityManager& entitymanager_;
#ifndef NO_AUDIO
	Music* music_;
#endif
};

#endif // GAME_HPP
