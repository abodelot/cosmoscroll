#include <iostream>
#include <typeinfo>
#include <SFML/System.hpp>

#include "Game.hpp"

#include "Asteroid.hpp"
#include "Ball.hpp"
#include "Ennemy.hpp"
#include "EvilBoss.hpp"

#include "LevelManager.hpp"
#include "MediaManager.hpp"
#include "Menu.hpp"
#include "Misc.hpp"
//#include "ConfigParser.hpp"

#define CONFIG_FILE "config/config.txt"

#define UNDEF -1	// On initialise player_1_ et player_2_ à ça à la création.

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
	PM_			(PlayerManager::GetInstance())
{
	/*config_.LoadFile(CONFIG_FILE);
	config_.SeekSection("Settings");
	std::string value;
	if (config_.ReadItem("last_level_reached_"))
	{
		;
	}*/
	last_level_reached_ = 1;
	if (true)
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
#ifndef NO_MUSIC
	music_ = GET_MUSIC("space");
	music_->Play();
#endif
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
			case STORY_MODE:
			case DOUBLE_STORY_MODE:
				what = PlayStory();
				break;
			case ARCADE_MODE:
				what = PlayArcade();
				break;
			case PONG_MODE:
				what = PlayPong();
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
#ifndef NO_AUDIO
#ifndef NO_MUSIC
	music_->Stop();
	delete music_;
	music_ = NULL;
#endif
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
	controls_.SetControls(AC::ALL);

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
	
	sf::Sprite ship(GET_IMG("spaceship-red"));
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
	controls_.SetControls(AC::ALL);
	
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
		menu.AddItem("Pong", PONG_MODE);
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
	if (choice == ARCADE_MODE || choice == PONG_MODE || choice == STORY_MODE ||
		choice == DOUBLE_STORY_MODE)
	{
		// clean up
		//RemoveEntities();	// SUPPRESSED IN R 64 !?
		particles_.Clear();
		bullets_.Clear();
		
		// init
		timer_ = 0.0f;
		particles_.AddStars();

		if (choice == ARCADE_MODE)
		{
			mode_ = ARCADE;
		}
		else if (choice == PONG_MODE)
		{
			mode_ = PONG;
		}
		else
		{
			current_level_ = 1;
			mode_ = (choice == DOUBLE_STORY_MODE)? STORY2X : STORY;
			choice = SELECT_LEVEL;
		}
		// Respawn();

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
	controls_.SetControls(AC::ALL);

	sf::String title("P A U S E");
	title.SetPosition(180, 160);
	title.SetSize(30.0);
	
	Menu menu;
	menu.SetOffset(sf::Vector2f(220, 200));
	Choice resume = mode_ == ARCADE ? ARCADE_MODE : mode_ == STORY ? STORY_MODE : mode_==STORY2X ? DOUBLE_STORY_MODE : PONG_MODE;
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
	info.SetText("Game  Over");
	if (GetMode() == ARCADE || GetMode() == PONG)
	{	
		what = END_ARCADE;
	}
	else
	{
		what = SELECT_LEVEL;
		if ((GetMode()== STORY && entities_.size() > 1) ||
			(GetMode()== STORY2X && entities_.size() > 2)) // si perdu
		{
			info.SetText("Game  Over");
		}
		else if (current_level_ < levels_.GetLast())
		{
			info.SetText(str_sprintf("Niveau %d termine", current_level_));
			++current_level_;
			if (current_level_ > last_level_reached_)
			{
				last_level_reached_ = current_level_;
#ifdef DEBUG
				printf("nouveau niveau atteint : %d\n", last_level_reached_);
#endif
			}
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
	}
	
	sf::FloatRect rect = info.GetRect();
	info.SetPosition((WIN_WIDTH - rect.GetWidth()) / 2,
		(WIN_HEIGHT - rect.GetHeight()) / 2);
	bool running = true;
	
	AC::Action action;
	controls_.SetControls(AC::ALL);
	
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

	if (mode_ == ARCADE)
	{
		int min = (int) PM_.GetBestTime() / 60;
		int sec = (int) PM_.GetBestTime() % 60;
		title.SetText(str_sprintf("Vous avez tenu seulement %d min et %d sec",
			min, sec));
	}
	else // mode_ == PONG
	{
		int winner;
		PM_.Select(player_1_);
		winner = PM_.GetShip()->HP();
		PM_.Select(player_2_);
		 
		if (PM_.GetShip()->HP() > winner)
		{
			winner = -2;
		}
		 
		if (winner == -2)
		{
			winner = 2;
		}
		else
		{
			winner = 1;
			PM_.Select(player_1_); 
		}
		title.SetText(str_sprintf("Le joueur %i gagne\n Il a encore %i vies.\n",
			winner, PM_.GetShip()->HP()));
	}
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
	controls_.SetControls(AC::ALL);
	
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


// choix des niveaux
Game::Choice Game::SelectLevel()
{
#ifdef DEBUG
	puts("[ Game::SelectLevel ]");
#endif
	Menu menu;
	menu.SetOffset(sf::Vector2f(42, 42));
	
	AC::Action action;
	controls_.SetControls(AC::ALL);
	
	bool running = true;
	Choice choice = EXIT_APP;
	int last = levels_.GetLast();
	for (int i = 0; i < last; ++i)
	{
		bool activable = i < last_level_reached_;
		menu.AddItem(str_sprintf("Niveau %d", i + 1), i, activable);
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
					
					Respawn();	

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
	controls_.SetControls(AC::ALL);

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


// retourne true s'il y a encore des ennemis à envoyer plus tard
// sinon false (niveau fini, bravo :D)
bool Game::MoreBadGuys()
{
	PM_.Select(player_1_); // FIXME: On fait quoi en 2 joueurs?
	if (GetMode() == ARCADE)
	{
		
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
	else
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
}



void Game::RemoveEntities()
{
	std::cerr << "RemoveEntities\n";
	std::list<Entity*>::iterator it;
	for (it = entities_.begin(); it != entities_.end(); ++it)
	{
        if (typeid(**it) != typeid(PlayerShip))
        {
            delete *it;
        }
#ifdef DEBUG
		else
		{
		std::cout << "Must remove player @ " << *it << std::endl;
		}
#endif
	}
	entities_.clear();
}


void Game::Respawn()
{
#ifdef DEBUG
	puts("[ Game::Respawn ]");
#endif
	sf::Vector2f offset;
	offset.y = WIN_HEIGHT / 2.0;

	RemoveEntities();

	//	JOUEUR UN
	offset.x = 0.f;
	
	if (player_1_ == UNDEF)
	{
		player_1_ = PM_.New();
		PM_.Select(player_1_);
		PM_.Place(offset);
	}
	else
	{
		PM_.Select(player_1_);	
		PM_.NewShip(offset);
	}

	PM_.SetControlMode(AC::KEYBOARD);
	AddEntity(PM_.GetShip());
	

	if (GetMode() == STORY2X || GetMode() == PONG)
	{
		// JOUEUR DEUX	
		offset.x = WIN_HEIGHT;
		
		if (player_2_ == UNDEF)
		{
			player_2_ = PM_.New();
			PM_.Select(player_2_);

		}

		PM_.Select(player_2_);
		PM_.NewShip(offset, "spaceship-green");

	
		PM_.SetControlMode(AC::JOY_0 | AC::JOY_1);
		AddEntity(PM_.GetShip());
	}
	else
	{
		PM_.SetControlMode(AC::ALL);
	}

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

	Respawn();
	
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
			running = MoreBadGuys();
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


Game::Choice Game::PlayStory()
{
#ifdef DEBUG
	puts("[ Game::PlayStory ]");
#endif
	
	PM_.Select(player_1_);
	AC::Action action;
	bool dead, running = true;
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
				if (GetMode() == STORY2X)
				{
					PM_.Select(player_1_);
					controls_.SetControls(PM_.GetControlMode());
					PM_.GetShip()->HandleAction(action);
					PM_.Select(player_2_);
					controls_.SetControls(PM_.GetControlMode());
				}
				PM_.GetShip()->HandleAction(action);
			}
		}
	
		if (running)
		{
			running = MoreBadGuys() || 
				(GetMode() == STORY2X && entities_.size() > 2) ||
				(GetMode() == STORY && entities_.size() > 1);


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
		
		dead &= false;
		
		for (it = entities_.begin(); it != entities_.end();)
		{
			if ((**it).IsDead())
			{
				if (GetMode()== STORY2X)
				{
					PM_.Select(player_1_);
		
					if (PM_.GetShip() == *it)
					{
							dead = true;
					}
					else
					{
						PM_.Select(player_2_);
					}
				}
				if (PM_.GetShip() == *it)
					dead = true;

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
				if (GetMode() == STORY2X)
				{
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
					}
				}
				if (*it != PM_.GetShip() && 
					PM_.GetShip()->GetRect().Intersects((**it).GetRect()))
				{
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


Game::Choice Game::PlayPong()
{
#ifdef DEBUG
	puts("[ Game::PlayPong ]");
#endif

	
	AC::Action action;
	bool running = true;
	static int lives_1 = 3;
	static int lives_2 = 3;
	
	
	static sf::Vector2f p1x(0.f, WIN_WIDTH / 6);
	static sf::Vector2f p1y(CONTROL_PANEL_HEIGHT, WIN_HEIGHT);
	static sf::Vector2f p2x(5 * WIN_WIDTH /6, WIN_WIDTH);
	static sf::Vector2f p2y(CONTROL_PANEL_HEIGHT, WIN_HEIGHT);

	sf::String p1_infos;
	sf::String p2_infos;
	p1_infos.SetPosition(12, CONTROL_PANEL_HEIGHT + 30.0);
	p1_infos.SetSize(30.0);
	p2_infos.SetPosition(WIN_WIDTH - 56, CONTROL_PANEL_HEIGHT + 30.0);
	p2_infos.SetSize(30.0);

	Respawn();
		
	PM_.Select(player_2_);
	PM_.GetShip()->Flip(true);
	PM_.GetShip()->SetLimits(p2x, p2y);
	PM_.GetShip()->UseLimits(true);
	
	entities_.push_front(new Ball());
	
	Choice what = EXIT_APP;

	
	PM_.Select(player_1_);
	PM_.GetShip()->SetLimits(p1x, p1y);
	PM_.GetShip()->UseLimits(true);
	while (running)
	{
		lives_1 = PM_.GetShip()->HP();
		PM_.Select(player_2_);
		lives_2 = PM_.GetShip()->HP();
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
				PM_.GetShip()->HandleAction(action);
				PM_.Select(player_2_);
				controls_.SetControls(PM_.GetControlMode());
				PM_.GetShip()->HandleAction(action);
			}
		}
	
		if (running)
		{
			PM_.Select(player_1_);
			PM_.GetShip()->NoShot();
			PM_.Select(player_2_);
			PM_.GetShip()->NoShot();
			running = (lives_1 > 0 || lives_2 > 0);
			if (!running)
			{
			std::cout << "lives 1:" << lives_1 << " lives 2:" << lives_2 << "\n";
				// Ne devrait jamais arriver en arcade
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
				std::cerr << "IsDead";
				if (typeid (**it) == typeid(PlayerShip))
				{
					std::cerr << " est PLAYERSHIP\n";
				}
				else
				{
				std::cerr << " est la BALLE\n";

				bullets_.CleanSenders(*it);
				delete *it;
				it = entities_.erase(it);
				}
			}
			else
			{
				(**it).Move(time);
				// collision Joueur <-> autres unités
				
				PM_.Select(player_1_);
				if (*it != PM_.GetShip() && 
					PM_.GetShip()->GetRect().Intersects((**it).GetRect()))
				{
						(**it).Collide(*PM_.GetShip());
				}

					PM_.Select(player_2_);

				if (*it != PM_.GetShip() && 
					PM_.GetShip()->GetRect().Intersects((**it).GetRect()))
				{
					(**it).Collide(*PM_.GetShip());
				}
			}
			++it;
		}
		
		bullets_.Update(time);
		bullets_.Collide(entities_); // fusion avec Update ?
		
		particles_.Update(time);
		
		// Update textes
	
		static std::stringstream sst;

		PM_.Select(player_1_);
		sst.str("");
		sst << PM_.GetShip()->HP();

		p1_infos.SetText(sst.str());
		
		PM_.Select(player_2_);
		sst.str("");
		sst << PM_.GetShip()->HP();
				
		p2_infos.SetText(sst.str());
	
		// rendering
		particles_.Show(app_);
		for (it = entities_.begin(); it != entities_.end(); ++it)
		{
			(**it).Show(app_);
		}
		

		
		bullets_.Show(app_);
		panel_.Show(app_);
		app_.Draw(p1_infos);
		app_.Draw(p2_infos);
		app_.Display();
	}
	
	PM_.SetBestTime(timer_);
	return what;
}

void Game::Hit(int player_id)
{
	if (player_id == 1)
	{
		std::cerr << "Hittage player_1_\n";
		player_id = player_1_;
	}
	else if (player_id == 2)
	{
		std::cerr << "Hittage player_2_\n";
		player_id = player_2_;
	}
	PM_.Select(player_id);
	PM_.GetShip()->Entity::Hit(1);	
}


