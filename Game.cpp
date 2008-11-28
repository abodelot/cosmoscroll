#include <iostream>
#include <typeinfo>
#include <SFML/System.hpp>

#include "Asteroid.hpp"
#include "Ennemy.hpp"
#include "EvilBoss.hpp"
#include "Game.hpp"
#include "LevelManager.hpp"
#include "MediaManager.hpp"
#include "Menu.hpp"
#include "Misc.hpp"
#include "Settings.hpp"

#define CONFIG_FILE "config/settings.txt"

#ifndef SVN_REV
#define SVN_REV "???"
#endif

Game& Game::GetInstance()
{
	static Game self;
	return self;
}

Game::Game() :
	controls_	(AbstractController::GetInstance()),
	bullets_	(BulletManager::GetInstance()),
	panel_		(ControlPanel::GetInstance()),
	levels_		(LevelManager::GetInstance()),
	particles_	(ParticleSystem::GetInstance()),
	PM_			(PlayerManager::GetInstance()),
	settings_	(Settings::GetInstance())
{
	Settings& settings = Settings::GetInstance();
	settings.Load(CONFIG_FILE);

	if (!settings.Fullscreen())
	{
		app_.Create(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT, WIN_BPP), WIN_TITLE);
	}
	else
	{
		app_.Create(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT, WIN_BPP), WIN_TITLE,
			sf::Style::Fullscreen);
	}
	app_.SetFramerateLimit(WIN_FPS);
	app_.ShowMouseCursor(false);
	app_.EnableKeyRepeat(false);
	player_1_ = player_2_ = -1;
	controls_.SetControls(AC::KEYBOARD);
}

Game::~Game()
{
	RemoveEntities();
	app_.EnableKeyRepeat(true); // bug SFML 1.3 sous linux : il faut réactiver à la main le keyrepeat
	app_.Close();
}

void Game::Run()
{
	puts("running...");
	// première scène = Intro
	Choice what = Intro();
#ifndef NO_AUDIO
	music_ = GET_MUSIC("space");
	music_->Play();
#endif
	do
	{
		switch (what)
		{
			case INTRO:
				what = Intro();
				break;
			case MAIN_MENU:
				what = MainMenu();
				break;
			case DOUBLE_STORY_MODE:
				what = PlayStoryTwoPlayers();
				break;
			case ARCADE_MODE:
				what = PlayArcade();
				break;
			case STORY_MODE:
				what = PlayStoryOnePlayer();
				break;
			case IN_GAME_MENU:
				what = InGameMenu();
				break;
			case END_PLAY:
				what = EndPlay();
				break;
			case SELECT_LEVEL:
				what = SelectLevel();
				break;
			case LEVEL_CAPTION:
				what = LevelCaption();
				break;
			case END_ARCADE:
				what = EndArcade();
			default:
				break;
		}
	} while (what != EXIT_APP);
#ifndef NO_AUDIO // si what == EXIT_APP
	music_->Stop();
	delete music_;
	music_ = NULL;
#endif
}

// les méthodes-écrans

// une (superbe) intro au lancement
Game::Choice Game::Intro()
{
#ifdef DEBUG
	puts("[ Game::Intro ]");
#endif
	AC::Action action;
	Choice what = MAIN_MENU;
	const int DURATION = 5, PADDING = 10;
	float time, elapsed = 0;
#ifdef DEBUG
#ifndef NO_AUDIO
	bool played = false;
	sf::Sound intro_sound(GET_SOUNDBUF("title"));
#endif
#endif
	sf::FloatRect rect;
	
	sf::Sprite background;
		background.SetImage(GET_IMG("background"));
	
	sf::String title;
		title.SetText("CosmoScroll");
		title.SetFont(GET_FONT());
		title.SetSize(1000);
		title.SetColor(sf::Color::White);
		title.SetPosition(WIN_WIDTH / 2, WIN_HEIGHT / 2);
		rect = title.GetRect();
		title.SetCenter(rect.GetWidth() / 2, rect.GetHeight() / 2);
	
	sf::String sfml;
		sfml.SetText("Powered by SFML");
		sfml.SetSize(16);
		sfml.SetColor(sf::Color::White);
		rect = sfml.GetRect();
		sfml.SetPosition(WIN_WIDTH - rect.GetWidth() - PADDING,
			WIN_HEIGHT - rect.GetHeight() - PADDING);	
	
	sf::Sprite ship(GET_IMG("spaceship"));
		ship.SetPosition(-20, 100);

	while (elapsed < DURATION)
	{
		while (controls_.GetAction(action)) 
		{
			if (action == AC::EXIT_APP)
			{
				elapsed = DURATION;
				what = EXIT_APP;
			}
			else if (action == AC::VALID)
			{
				elapsed = DURATION;
			}
		}
		time = app_.GetFrameTime();
		elapsed += time;
#ifdef DEBUG
#ifndef NO_AUDIO
		if (static_cast<int>(elapsed) == 1 && !played)
		{
			played = true;
			intro_sound.Play();
		}
#endif
#endif
		ship.Move(180 * time, 25 * time);	// FIXME: déplacement magique !
		title.Scale(0.99, 0.99);			// FIXME: dépendant des FPS
		title.SetColor(sf::Color(255, 255, 255,
			static_cast<sf::Uint8>(255 * elapsed / DURATION)));
		
		app_.Draw(background);	app_.Draw(sfml);
		app_.Draw(title);		app_.Draw(ship);
		app_.Display();
	}
	return what;
}


Game::Choice Game::MainMenu()
{
#ifdef DEBUG
	puts("[ Game::MainMenu ]");
#endif
	bool running = true;
	int choice = EXIT_APP;
	
	AC::Action action;

	sf::String title(str_sprintf("CosmoScroll - r%s", SVN_REV));
		title.SetFont(GET_FONT());
		title.SetSize(40);
		title.SetY(42);
		title.SetX((WIN_WIDTH - title.GetRect().GetWidth()) / 2);
	
	Menu menu;
		menu.SetOffset(sf::Vector2f(60, 120));
		menu.AddItem("Aventure solo", STORY_MODE);
		menu.AddItem("Aventure duo", DOUBLE_STORY_MODE);
		menu.AddItem("Arcade", ARCADE_MODE);
		menu.AddItem("Quitter", EXIT_APP);

	while (running)
	{
		while (controls_.GetAction(action))
		{
			if (action == AC::EXIT_APP)
			{
				running = false;
			}
			else if (menu.ItemChosen(action, choice))
			{
				running = false;
			}
		}
		app_.Draw(title);
		menu.Show(app_);
		app_.Display();
	}



	// si on lance une nouvelle partie, on nettoie les restes des éventuelles
	// parties précédentes et l'on respawn un joueur neuf
	if (choice == ARCADE_MODE || choice == STORY_MODE || choice == DOUBLE_STORY_MODE)
	{
		// clean up
		RemoveEntities();	// SUPPRESSED IN R 64 !?
		particles_.Clear();
		bullets_.Clear();
		
		// init
		timer_ = 0.0f;
		particles_.AddStars();
		/*
		if (choice == STORY_MODE || choice == DOUBLE_STORY_MODE)
		{
				cur_lvl_ = 0;
			mode_ = (choice == DOUBLE_STORY_MODE)? STORY2X : STORY;
			choice = CONTINUE;
		}
		else
		{
			mode_ = ARCADE;
		}
		*/
		if (choice == ARCADE_MODE)
		{
			mode_ = ARCADE;
		}
		else
		{
			current_level_ = 1;
			mode_ = (choice == DOUBLE_STORY_MODE)? STORY2X : STORY;
			choice = SELECT_LEVEL;
		}
		// Respawn();  <-RESTORED IN R64?!
	}
	return static_cast<Choice>(choice);
}

Game::Choice Game::Options()
{
	puts("-> Game::Options\nnot implemented yet (quit)");
	return EXIT_APP;
}


// pause avec un menu
Game::Choice Game::InGameMenu()
{
#ifdef DEBUG
	puts("[ Game::InGameMenu ]");
#endif
	AC::Action action;
	sf::String title("P A U S E");
	title.SetPosition(180, 160);
	title.SetSize(30.0);
	
	Menu menu;
	menu.SetOffset(sf::Vector2f(220, 200));
	Choice resume = mode_ == ARCADE ? ARCADE_MODE : mode_ == STORY ? STORY_MODE : DOUBLE_STORY_MODE;
	menu.AddItem("Reprendre la partie", resume);
	menu.AddItem("Revenir au menu principal", MAIN_MENU);
	//menu.AddItem("Options", OPTIONS);
	menu.AddItem("Quitter le jeu", EXIT_APP);
	
	bool paused = true;
	int what;
	while (paused)
	{
		while (controls_.GetAction(action))
		{
			if (action == AC::EXIT_APP)
			{
				what = EXIT_APP;
				paused = false;
			}
			else if (action == AC::PAUSE)
			{
				what = resume;
				paused = false;
			}
			else if (menu.ItemChosen(action, what))
			{
				paused = false;
			}
		}
		
		// seules les particules sont mises à jour
		particles_.Update(app_.GetFrameTime());
		
		// rendering
		bullets_.Show(app_);
		particles_.Show(app_);
		std::list<Entity*>::iterator it;
		for (it = entities_.begin(); it != entities_.end(); ++it)
		{
			(**it).Show(app_);
		}
		panel_.Show(app_);
		
		app_.Draw(title);
		menu.Show(app_);
		app_.Display();
	}

	return static_cast<Choice>(what);

}

/*
Game::Choice Game::GameOverStory()
{
#ifdef DEBUG
	puts("[ Game::GameOverStory ]");
#endif
	sf::String title;
		title.SetText("Fin de niveau");
		title.SetFont(GET_FONT());
		title.SetColor(sf::Color::White);
		title.SetPosition(42, 42);
	
	Menu menu;
		menu.SetOffset(sf::Vector2f(42, 100));
		menu.AddItem("Menu principal", MAIN_MENU);	
		menu.AddItem("Quitter", EXIT_APP);
	
	bool running = true;
	int choice = EXIT_APP;
	
	AC::Action action;
	while (running)
	{
		while (controls_.GetAction(action))
		{
			if (action == AC::EXIT_APP)
			{
				running = false;
			}
			else if (menu.ItemChosen(action, choice))
			{
				running = false;
			}
		}
		app_.Draw(title);
		menu.Show(app_);
		app_.Display();
	}
	return static_cast<Choice>(choice);
}

Game::Choice Game::GameOverArcade()
{
#ifdef DEBUG
	puts("[ Game::GameOverArcade ]");
#endif

	int min = (int) PM_.GetBestTime() / 60;
	int sec = (int) PM_.GetBestTime() % 60;

	sf::String title;
		title.SetText(str_sprintf("Vous avez tenu seulement %d min et %d sec", min, sec));
		title.SetFont(GET_FONT());
		title.SetColor(sf::Color::White);
		title.SetPosition(42, 42);
	
	Menu menu;
		menu.SetOffset(sf::Vector2f(42, 100));
		menu.AddItem("Menu principal", MAIN_MENU);	
		menu.AddItem("Quitter", EXIT_APP);
	
	bool running = true;
	int choice = EXIT_APP;
	
	AC::Action action;
	while (running)
	{
		while (controls_.GetAction(action))
		{
			if (action == AC::EXIT_APP)
			{
				running = false;
			}
			else if (menu.ItemChosen(action, choice))
			{
				running = false;
			}
		}
		app_.Draw(title);
		menu.Show(app_);
		app_.Display();
	}
	return static_cast<Choice>(choice);
}
*/

// la partie est finie
Game::Choice Game::EndPlay()
{
	const float DURATION = 5;
	float timer = 0;
	Choice what;
	sf::String info;
	info.SetSize(70);
	info.SetColor(sf::Color::White);
	info.SetFont(GET_FONT());
	// perdu ou gagné ?
	if (entities_.size() > 1) // si perdu
	{
		info.SetText("GameOver");
		what = mode_ == ARCADE ? END_ARCADE : MAIN_MENU;
	}
	else if (current_level_ < levels_.GetLast())
	{
		info.SetText(str_sprintf("Niveau %d termine", current_level_));
		++current_level_;
		what = SELECT_LEVEL;
	}
	else
	{
		std::string epic_win = str_sprintf("Felicitations :-D\n\n"
			"Vous avez fini les %d niveaux du jeu.\n"
			"Vous etes vraiment doue(e) :D", current_level_);
		info.SetText(epic_win);
		info.SetSize(30);
		what = MAIN_MENU;
	}
	sf::FloatRect rect = info.GetRect();
	info.SetPosition((WIN_WIDTH - rect.GetWidth()) / 2,
		(WIN_HEIGHT - rect.GetHeight()) / 2);
	bool running = true;
	AC::Action action;
	while (running)
	{
		while (controls_.GetAction(action))
		{
			if (action == AC::EXIT_APP)
			{
				running = false;
				what = EXIT_APP;
			}
			else if (action == AC::VALID)
			{
				running = false;
			}
		}
		float frametime = app_.GetFrameTime();
		timer += frametime;
		if (timer >= DURATION)
		{
			running = false;
		}
		// seules les particules sont mises à jour
		particles_.Update(frametime);
		info.SetColor(sf::Color(255, 255, 255,
			(sf::Uint8)255 - 255 * timer / DURATION));
		
		// rendering
		bullets_.Show(app_);
		particles_.Show(app_);
		std::list<Entity*>::iterator it;
		for (it = entities_.begin(); it != entities_.end(); ++it)
		{
			(**it).Show(app_);
		}
		panel_.Show(app_);
		app_.Draw(info);
		app_.Display();
	}
	return what;
}


// bilan fin de partie en aracde
Game::Choice Game::EndArcade()
{
#ifdef DEBUG
	puts("[ Game::EndArcade ]");
#endif
	sf::String title;
	assert(mode_ == ARCADE);

	int min = (int) PM_.GetBestTime() / 60;
	int sec = (int) PM_.GetBestTime() % 60;
	title.SetText(str_sprintf("Vous avez tenu seulement %d min et %d sec",
		min, sec));
	
	title.SetFont(GET_FONT());
	title.SetColor(sf::Color::White);
	title.SetPosition(42, 42);
	
	Menu menu;
	menu.SetOffset(sf::Vector2f(42, 100));
	menu.AddItem("Menu principal", MAIN_MENU);	
	menu.AddItem("Quitter", EXIT_APP);
	
	bool running = true;
	int choice = EXIT_APP;
	
	AC::Action action;
	while (running)
	{
		while (controls_.GetAction(action))
		{
			if (action == AC::EXIT_APP)
			{
				running = false;
			}
			else if (menu.ItemChosen(action, choice))
			{
				running = false;
			}
		}
		app_.Draw(title);
		menu.Show(app_);
		app_.Display();
	}
	return static_cast<Choice>(choice);
}

/*
Game::Choice Game::Intertitre()
{
#ifdef DEBUG
	puts("[ Game::Intertitre ]");
#endif
	sf::String title;
	sf::String subtitle1;
	sf::String subtitle2;
	PM_.Select(player_1_);
	int min = (int) PM_.GetBestTime() / 60;
	int sec = (int) PM_.GetBestTime() % 60;
	subtitle1.SetText(str_sprintf("Termine en %d min et %d sec",
			min, sec));
	subtitle2.SetText(str_sprintf("Pass: %s", MakePassword().c_str()));
	
	title.SetText(str_sprintf("Fin du niveau %d", cur_lvl_));
	
	title.SetFont(GET_FONT());
	title.SetColor(sf::Color::White);
	title.SetPosition(42, 42);
	subtitle1.SetFont(GET_FONT());
	subtitle1.SetColor(sf::Color::White);
	subtitle1.SetPosition(32, 72);
	subtitle2.SetFont(GET_FONT());
	subtitle2.SetColor(sf::Color::White);
	subtitle2.SetPosition(32, 92);
	
	Menu menu;
	menu.SetOffset(sf::Vector2f(42, 200));
	menu.AddItem("Continuer", CONTINUE);	
	// menu.AddItem("Réessayer", RETRY);

	bool running = true;
	int choice;
	
	AC::Action action;
	while (running)
	{
		while (controls_.GetAction(action))
		{
			if (action == AC::EXIT_APP)
			{
				running = false;
			}
			else if (menu.ItemChosen(action, choice))
			{
				running = false;
			}
			else if (action == AC::USE_HACK)
			{
				//HACK
				Passwd_HACK();
			}
		}
		app_.Draw(title);
		app_.Draw(subtitle1);
		app_.Draw(subtitle2);
		menu.Show(app_);
		app_.Display();
	}
	return static_cast<Choice>(choice);
}
*/

// choix des niveaux
Game::Choice Game::SelectLevel()
{
#ifdef DEBUG
	puts("[ Game::SelectLevel ]");
#endif
	Menu menu;
	menu.SetOffset(sf::Vector2f(42, 42));
	AC::Action action;
	bool running = true;
	Choice choice = EXIT_APP;
	int last = levels_.GetLast();
	for (int i = 0; i < last; ++i)
	{
		menu.AddItem(str_sprintf("Niveau %d", i + 1), i);
	}
	menu.AddItem("Retour au menu principal", last);
	menu.SelectItem(current_level_ - 1);

	// hacky re-init
	timer_ = 0.0f;
	bullets_.Clear();
	particles_.Clear();
	particles_.AddStars();
	
	int level;
	while (running)
	{
		while (controls_.GetAction(action))
		{
			if (action == AC::EXIT_APP)
			{
				running = false;
			}
			else if (menu.ItemChosen(action, level))
			{
				running = false;
				if (level == last)
				{
					choice = MAIN_MENU;
				}
				else
				{
					current_level_ = level + 1;
					
					if (mode_ == STORY)
					{
						std::cerr << "Mode Story Un Player\n";
						Respawn();	
					}
					if (mode_ == STORY2X)
					{
						std::cerr << "Mode Story DEUX Player\n";
						RespawnTwo();
					}
					
					levels_.Set(current_level_);
					choice = LEVEL_CAPTION;
				}
			}
		}
		
		menu.Show(app_);
		app_.Display();
	}
	return choice;
}


Game::Choice Game::LevelCaption()
{
#ifdef DEBUG
	puts("[ Game::LevelCaption ]");
#endif
	AC::Action action;
	bool running = true;
	Choice what;
	
	std::string content = str_sprintf("Niveau %d\n\n%s", current_level_,
		levels_.GetDescription(current_level_));
	find_replace(content, "\\n", "\n");
	sf::String description(content);
	description.SetColor(sf::Color::White);
	description.SetFont(GET_FONT());
	description.SetSize(30); // FIXME: magique
	sf::FloatRect rect = description.GetRect();
	description.SetPosition((WIN_WIDTH - rect.GetWidth()) / 2,
		(WIN_HEIGHT - rect.GetHeight()) / 2);
	
	// hacky re-init
	timer_ = 0.0f;
	bullets_.Clear();
	particles_.Clear();
	particles_.AddStars();
	
	while (running)
	{
		while (controls_.GetAction(action))
		{
			if (action == AC::EXIT_APP)
			{
				running = false;
				what = EXIT_APP;
			}
			else if (action == AC::VALID)
			{
				running = false;
				what = STORY_MODE;
			}
		}
		
		
		app_.Draw(description);
		app_.Display();
	}
	return what;
}


/*
Game::Choice Game::Continue()
{
#ifdef DEBUG
	puts("[ Game::Continue ]");
#endif
	sf::String title;
	sf::String subtitle;

	Menu menu;
	menu.SetOffset(sf::Vector2f(42, 558));
	
	// hacky re-init
	timer_ = 0.0f;
	bullets_.Clear();
	particles_.Clear();
	particles_.AddStars();
	
	if (cur_lvl_ < level_.GetLastID())
	{
		if (mode_ == STORY)
		{
			std::cerr << "Mode Story Un Player\n";
			Respawn();	
		}
		if (mode_ == STORY2X)
		{
			std::cerr << "Mode Story DEUX Player\n";
			RespawnTwo();
		}
		level_.Set(++cur_lvl_, level_desc_);
		find_replace(level_desc_, "\\n", "\n");
		title.SetText(str_sprintf("Niveau %d", cur_lvl_));
		subtitle.SetText(level_desc_);
			
		menu.AddItem("Jouer", mode_ == STORY? STORY_MODE : DOUBLE_STORY_MODE);  
		 PM_.Place();
	}
	else	// On a fini le jeu :)
	{
		title.SetText("Felicitations :-D");
		subtitle.SetText(str_sprintf("Vous avez fini les %d niveaux du jeu.\n\nVous etes vraiment doue(e) :D", cur_lvl_));
		menu.AddItem("Retour", MAIN_MENU);
	}
   
	title.SetFont(GET_FONT());
	title.SetColor(sf::Color::White);
	title.SetPosition(42, 42);
	subtitle.SetFont(GET_FONT());
	subtitle.SetColor(sf::Color::White);
	subtitle.SetPosition(42, 120);
	
	bool running = true;
	int choice = EXIT_APP;
	AC::Action action;
	while (running)
	{		
		while (controls_.GetAction(action))
		{
			if (action == AC::EXIT_APP)
			{
				running = false;
			}
			else if (menu.ItemChosen(action, choice))
			{
				running = false;
			}
			else if (action == AC::USE_HACK)
			{
				//<HACK>
				Passwd_HACK();
				level_.Set(cur_lvl_, level_desc_);
				find_replace(level_desc_, "\\n", "\n");
				title.SetText(str_sprintf("Niveau %d", cur_lvl_));
				subtitle.SetText(level_desc_);
				//</HACK>
			}
		}
		app_.Draw(subtitle);
		app_.Draw(title);
		menu.Show(app_);
		app_.Display();
	}
	return static_cast<Choice>(choice);
}*/

// retourne true s'il y a encore des ennemis à envoyer plus tard
// sinon false (niveau fini, bravo :D)
bool Game::MoreBadGuysArcade()
{
	PM_.Select(player_1_);
	std::cerr << "Ds MoreBadGuysArcade, playership = " << PM_.GetShip() << "\n";
	// un ennemi en plus toutes les 10s + 5
	unsigned int max_bad_guys = static_cast<unsigned int>(timer_ / 10 + 5);
	if (entities_.size() < max_bad_guys)
	{
		// ajout d'une ennemi au hasard
		// <hack>
		sf::Vector2f offset;
		offset.x = WIN_WIDTH;
		offset.y = sf::Randomizer::Random(CONTROL_PANEL_HEIGHT, WIN_HEIGHT);
		int random = sf::Randomizer::Random(0, 10);
		if (random > 6)
		{
			AddEntity(Ennemy::Make(Ennemy::INTERCEPTOR, offset, PM_.GetShip()));
		}
		else if (random > 4)
		{
			AddEntity(Ennemy::Make(Ennemy::BLORB, offset, PM_.GetShip()));
		}
		else if (random > 2)
		{
			AddEntity(Ennemy::Make(Ennemy::DRONE, offset, PM_.GetShip()));
		}
		else
		{
			AddEntity(new Asteroid(offset, Asteroid::BIG));
		};
		// </hack>
	}
	// endless badguys! arcade mode will kill you til you die from it.
	return true;
}

// retourne true s'il y a encore des ennemis à envoyer plus tard
// sinon false (niveau fini, bravo :D)
bool Game::MoreBadGuysStory()
{
	LevelManager& level = LevelManager::GetInstance();
	Entity* p = level.GiveNext(timer_);
	while (p != NULL)
	{
		AddEntity(p);
		p = level.GiveNext(timer_);
	}
	
	// si le niveau n'est pas fini, alors il y a encore des ennemis
	return level.RemainingEntities() > 0;
}

void Game::RemoveEntities()
{
	std::list<Entity*>::iterator it;
	for (it = entities_.begin(); it != entities_.end(); ++it)
	{
        if (typeid(**it) != typeid(PlayerShip))
        {
            delete *it;
        }
	}
	entities_.clear();
}


void Game::RespawnTwo()
{
#ifdef DEBUG
	puts("\tGame::RespawnTwo()");
#endif
	sf::Vector2f offset;
	offset.y = WIN_HEIGHT / 2.0;
	PlayerShip* ship = NULL;

		std::cerr << "Story deux player\n";
		offset.x = WIN_HEIGHT;
		if (player_2_ == -1)
		{
			player_2_ = PM_.New();
			std::cerr << "On select le player " << player_2_ << std::endl;
		}
		PM_.Select(player_2_);
		ship = PM_.GetShip();
		if (ship)
			PM_.DelShip();
			
		ship = PM_.NewShip(offset);
		std::cerr << "ship vaut " << ship << "\n";
		PM_.SetControlMode(AC::KEYBOARD);
		AddEntity(ship);
		offset.x = 0;
		if (player_1_ == -1)
		{
			player_1_ = PM_.New();
			std::cerr << "On select le player " << player_1_ << std::endl;
		}
		PM_.Select(player_1_);
		ship = PM_.GetShip();
		if (ship)
			PM_.DelShip();
			
		ship = PM_.NewShip(offset);
		PM_.SetControlMode(AC::JOY_0 | AC::JOY_1);
		std::cerr << "ship vaut " << ship << "\n";
		AddEntity(ship);

}

void Game::Respawn()
{
#ifdef DEBUG
	puts("\tGame::Respawn()");
#endif
	sf::Vector2f offset;
	offset.y = WIN_HEIGHT / 2.0;
	PlayerShip* ship = NULL;
	
	offset.x = 0;
	if (player_1_ == -1)
	{
		player_1_ = PM_.New();
		std::cerr << "On select le player " << player_1_ << std::endl;
	}
	PM_.Select(player_1_);
	ship = PM_.GetShip();
	if (ship)
		PM_.DelShip();
		
	ship = PM_.NewShip(offset);
	std::cerr << "ship vaut " << ship << "\n";
	AddEntity(ship);

}

void Game::AddEntity(Entity* entity)
{
	entities_.push_front(entity);
}

std::string Game::MakePassword()
{
	Password pass;	
	unsigned char lives, level, shield, icecubes;
	
	lives = static_cast<unsigned char>(PM_.GetShip()->GetHP());
	level = static_cast<unsigned char>(current_level_);
	icecubes = static_cast<unsigned char>(PM_.GetShip()->GetCoolers());
	shield = static_cast<unsigned char>(PM_.GetShip()->GetShield());
	pass.setLives(lives);	pass.setLevel(level);
	pass.setCoolers(icecubes); pass.setShield(shield);
	
	std::string res = pass.getEncoded(); 
#ifdef DEBUG
	std::cerr << "\t PASS: " << res << "\n";
#endif
	return res;
}

// AAAAAAAAAAADAwICAAAAAAAK <- L2
// AAAAAAAAAAACBAIDAAAAAAAL <- L3

bool Game::UsePassword(std::string & source)
{
	bool ok = false;
	unsigned char pass_1_, pass_2_, pass_3_, pass_4_;

	Password pass(dynamic_cast<const std::string &>(source));
	
	pass_1_ = pass.getLives();
	
	pass_2_ = pass.getLevel();
	
	pass_3_ = pass.getShield();

	pass_4_ = pass.getCoolers();

	PM_.GetShip()->SetHP(static_cast<int>(pass_1_));
	PM_.GetShip()->SetShield(static_cast<int>(pass_3_));
	PM_.GetShip()->SetCoolers(static_cast<int>(pass_4_));
	
	panel_.SetShipHP(PM_.GetShip()->GetHP());
	panel_.SetShield(PM_.GetShip()->GetShield());
	panel_.SetCoolers(PM_.GetShip()->GetCoolers());
	
	if (levels_.Set(pass_2_) == LevelManager::SUCCESS) 
	{
		current_level_ = static_cast<int>(pass_2_);
	}
	else
	{
		ok = false;
	}
	return ok;
}

// Oh un joli hack bien rédigé :D
bool Game::Passwd_HACK() {
	bool ok = false; std::string src;	
	std::cout << "Entrez pass, ou X pour passer\n";
	std::cin >> src;
	if (!(src[0] - 88) || !(src[0] - 120)){
		std::cout << "Pas de pass entre\n";
		ok ^= 1; } else {
		if (UsePassword(src)) {
			std::cout << "Okay\n"; ok ^=1;
		} else std::cout << "Erreur ! \n";
	} return ok;
}


Game::Choice Game::PlayArcade()
{
#ifdef DEBUG
	puts("[ Game::PlayArcade ]");
#endif

	
	AC::Action action;
	bool running = true;
	Choice what = EXIT_APP;
	
	PM_.Select(player_1_);
	while (running)
	{
		while (controls_.GetAction(action))
		{
			if (action == AC::EXIT_APP)
			{
				running = false;
			}
			else if (action == AC::PAUSE)
			{
				running = false;
				what = IN_GAME_MENU;
			}
			else
			{
				PM_.GetShip()->HandleAction(action);
			}
		}
	
		if (running)
		{
			running = MoreBadGuysArcade();
			if (!running)
			{
				// Ne devrait jamais arriver en arcade
				assert(false);
			}
		}

		std::list<Entity*>::iterator it;
		// action
		for (it = entities_.begin(); it != entities_.end(); ++it)
		{
			(**it).Action();
		}
		
		// moving
		float time = app_.GetFrameTime();
		timer_ += time;
		panel_.SetTimer(timer_);
		
		for (it = entities_.begin(); it != entities_.end();)
		{
			if ((**it).IsDead())
			{
				if (PM_.GetShip() == *it )
				{
					running = false;
					what = END_PLAY;
#ifdef DEBUG
					puts("\nplayer killed");
#endif
					break;
				}
				bullets_.CleanSenders(*it);
				delete *it;
				it = entities_.erase(it);
			}
			else
			{
				(**it).Move(time);
				// collision Joueur <-> autres unités
				if (PM_.GetShip() != *it &&
					PM_.GetShip()->GetRect().Intersects((**it).GetRect()))
				{
					// collision sur les deux éléments
					PM_.GetShip()->Collide(**it);
					(**it).Collide(*PM_.GetShip());
				}
				++it;
			}
		}
		
		bullets_.Update(time);
		bullets_.Collide(entities_); // fusion avec Update ?
		
		particles_.Update(time);
		
		// rendering
		particles_.Show(app_);
		for (it = entities_.begin(); it != entities_.end(); ++it)
		{
			(**it).Show(app_);
		}
		bullets_.Show(app_);
		panel_.Show(app_);
		app_.Display();
	}
	
	PM_.SetBestTime(timer_);
	return what;
}


Game::Choice Game::PlayStoryOnePlayer()
{
#ifdef DEBUG
	puts("[ Game::PlayStoryOnePlayer ]");
#endif
	
	PM_.Select(player_1_);
	AC::Action action;
	bool running = true;
	Choice what = EXIT_APP;
	
	while (running)
	{
		while (controls_.GetAction(action))
		{
			if (action == AC::EXIT_APP)
			{
				running = false;
			}
			else if (action == AC::PAUSE)
			{
				running = false;
				what = IN_GAME_MENU;
			}
			else
			{
				PM_.GetShip()->HandleAction(action);
			}
		}
	
		if (running)
		{
			running = MoreBadGuysStory() || entities_.size() > 1;
			if (!running)
			{
				// il n'y a plus d'ennemis en liste d'attente, et plus
				// d'ennemis dans le vecteur = Victoire
				// GameOver si plus de niveau
				what = END_PLAY;
			}
		}

		std::list<Entity*>::iterator it;
		// action
		for (it = entities_.begin(); it != entities_.end(); ++it)
		{
			(**it).Action();
		}
		
		// moving
		float time = app_.GetFrameTime();
		timer_ += time;
		panel_.SetTimer(timer_);
		
		for (it = entities_.begin(); it != entities_.end();)
		{
			if ((**it).IsDead())
			{
				if (PM_.GetShip() == *it)
				{
					running = false;
					what = END_PLAY;
#ifdef DEBUG
					puts("\nplayer killed");
#endif
					break;
				}
				bullets_.CleanSenders(*it);
				delete *it;
				it = entities_.erase(it);
			}
			else
			{
				(**it).Move(time);
				// collision Joueur <-> autres unités
				if (PM_.GetShip() != *it &&
					PM_.GetShip()->GetRect().Intersects((**it).GetRect()))
				{
					// collision sur les deux éléments
					PM_.GetShip()->Collide(**it);
					(**it).Collide(*PM_.GetShip());
				}
				++it;
			}
		}
		
		bullets_.Update(time);
		bullets_.Collide(entities_); // fusion avec Update ?
		
		particles_.Update(time);
		
		// rendering
		particles_.Show(app_);
		for (it = entities_.begin(); it != entities_.end(); ++it)
		{
			(**it).Show(app_);
		}
		bullets_.Show(app_);
		panel_.Show(app_);
		app_.Display();
	}
	
	PM_.SetBestTime(timer_);
	return what;
}


Game::Choice Game::PlayStoryTwoPlayers()
{
#ifdef DEBUG
	puts("[ Game::PlayStoryTwoPlayers ]");
#endif

	RespawnTwo();
	
	AC::Action action;
	bool running = true;
	Choice what = EXIT_APP;
	
	while (running)
	{
		while (controls_.GetAction(action))
		{
			if (action == AC::EXIT_APP)
			{
				running = false;
			}
			else if (action == AC::PAUSE)
			{
				running = false;
				what = IN_GAME_MENU;
			}
			else
			{
				PM_.Select(player_1_);
				controls_.SetControls(PM_.GetControlMode());
				// controls_.Filter(PM_.GetControlMode());
				PM_.GetShip()->HandleAction(action);
				PM_.Select(player_2_);
				controls_.SetControls(PM_.GetControlMode());
				//controls_.Filter(PM_.GetControlMode());
				PM_.GetShip()->HandleAction(action);
			}
		}
	
		if (running)
		{
			running = MoreBadGuysStory() || entities_.size() > 1;
			if (!running)
			{
				// il n'y a plus d'ennemis en liste d'attente, et plus
				// d'ennemis dans le vecteur = Victoire
				// GameOver si plus de niveau
				what = END_PLAY;
			}
		}

		std::list<Entity*>::iterator it;
		// action
		for (it = entities_.begin(); it != entities_.end(); ++it)
		{
			(**it).Action();
		}
		
		// moving
		float time = app_.GetFrameTime();
		timer_ += time;
		panel_.SetTimer(timer_);
		bool dead = false;
		for (it = entities_.begin(); it != entities_.end();)
		{
			if ((**it).IsDead())
				{
				PM_.Select(player_1_);
			
				if (PM_.GetShip() == *it)
				{
						dead = true;
				}
				else
				{
					PM_.Select(player_2_);
					if (PM_.GetShip() == *it)
						dead = true;
				}


				if (dead)
				{
					running = false;
					what = END_PLAY;
	#ifdef DEBUG
					puts("\nplayer killed");
	#endif
					break;
				}
				bullets_.CleanSenders(*it);
				delete *it;
				it = entities_.erase(it);
			}
			else
			{
				(**it).Move(time);
				// collision Joueur <-> autres unités
				PM_.Select(player_1_);
				if (*it != PM_.GetShip() && 
					PM_.GetShip()->GetRect().Intersects((**it).GetRect()))
				{

						PM_.GetShip()->Collide(**it);
						(**it).Collide(*PM_.GetShip());
				}
				else
				{ 
					PM_.Select(player_2_);
					if (*it != PM_.GetShip() && 
						PM_.GetShip()->GetRect().Intersects((**it).GetRect()))
					{
						PM_.GetShip()->Collide(**it);
						(**it).Collide(*PM_.GetShip());
					}
				}
				++it;
			}
		}
		
		bullets_.Update(time);
		bullets_.Collide(entities_); // fusion avec Update ?
		
		particles_.Update(time);
		
		// rendering
		particles_.Show(app_);
		for (it = entities_.begin(); it != entities_.end(); ++it)
		{
			(**it).Show(app_);
		}
		bullets_.Show(app_);
		panel_.Show(app_);
		app_.Display();
	}
	
	PM_.SetBestTime(timer_);
	return what;
}

