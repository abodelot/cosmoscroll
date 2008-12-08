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
#include "PlayerShip.hpp"

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
	
	inline Entity* GetShip() const
	{
		return player1_; // FIXME: toujours player 1
	}
	
	/*
	 * Ajout d'une nouvelle unité dans le jeu
	 */
	void AddEntity(Entity* entity);
	
	void Hit(int player_id);

	
	
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
	
	/*
	 * Suppression de toutes les unités en jeu
	 */
	void RemoveEntities();
	
	
	sf::RenderWindow app_;

	float timer_;
	GameMode mode_;
	
	int current_level_;
	int last_level_reached_;
	float best_arcade_time_;
	
	PlayerShip *player1_, *player2_;
	// toutes les unités sont allouées dynamiquement
	std::list<Entity*> entities_;
	
	/* pointeurs de méthodes pour la boucle de jeu */
	
	// transmettre les actions de la boucle d'évènements aux joueurs
	void (Game::*p_ForwardAction_)(AC::Action action, AC::Device device);
	
	void ForwardAction1P(AC::Action action, AC::Device device);
	void ForwardAction2P(AC::Action action, AC::Device device);
	
	// condition pour stopper la boucle de jeu
	bool (Game::*p_StopPlay_)();

	bool ArcadeMoreBadGuys();
	bool StoryMoreBadBuys();
	
	// Singletons
	AbstractController& controls_;
	BulletManager& bullets_;
	ControlPanel& panel_;
	LevelManager& levels_;
	ParticleSystem& particles_;
#ifndef NO_AUDIO
	Music* music_;
#endif
};
	
#endif /* guard GAME_HPP */

