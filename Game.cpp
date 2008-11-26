#include "Game.hpp"
#include "MediaManager.hpp"
#include "Settings.hpp"

#include "Asteroid.hpp"
#include "Ennemy.hpp"
#include "EvilBoss.hpp"

#include "Menu.hpp"
#include "Misc.hpp"
#include "Level.hpp"

#include <SFML/System.hpp>
#include <typeinfo>

#define CONFIG_FILE "config/settings.txt"
#define HACKY_KEY sf::Key::H

#ifndef JOYSTICK_ENABLED
#define JOYSTICK_ENABLED
#endif

Game& Game::GetInstance()
{
	static Game self;
	return self;
}


Game::Game() :
	bullets_  (BulletManager::GetInstance()),
	particles_(ParticleSystem::GetInstance()),
	panel_	(ControlPanel::GetInstance()),
	level_	(Level::GetInstance()),
	settings_ (Settings::GetInstance()),
	controls_(AbstractController::GetInstance())
{
	Settings& settings = Settings::GetInstance();
	settings.Load(CONFIG_FILE);
	// mise en place de la fenêtre de rendu
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
	key_pause_ = settings.GetKey(Settings::PAUSE);
	
#ifdef JOYSICK_ENABLED
	CalibrateJoystick();
#endif
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
			case MAIN_MENU:
				what = MainMenu();
				break;
			case ARCADE_MODE:
			case STORY_MODE:
				what = Play();
				break;
			case IN_GAME_MENU:
				what = InGameMenu();
				break;
			case GAME_OVER:
				what = GameOver();
				break;
			case CONTINUE:
				what = Continue();
				break;
			case INTERTITRE:
				what = Intertitre();
				break;
			default:
				break;
		}
	} while (what != EXIT_APP);
	// si what == EXIT_APP
#ifndef NO_AUDIO
	music_->Stop();
	delete music_;
	music_ = NULL;
#endif
}

// les méthodes-écrans

Game::Choice Game::Intro()
{
#ifdef DEBUG
	puts("[ Game::Intro ]");
#endif
	Choice what = MAIN_MENU;
	const int DURATION = 5;
	float elapsed = 0;
#ifdef DEBUG
#ifndef NO_AUDIO
	bool played = false;
	sf::Sound intro_sound(GET_SOUNDBUF("title"));
#endif
#endif
	sf::Sprite background;
	background.SetImage(GET_IMG("background"));
	sf::String title;
	title.SetText("CosmoScroll");
	title.SetFont(GET_FONT());
	title.SetSize(1000);
	title.SetColor(sf::Color::White);
	sf::String sfml;
	sfml.SetText("Powered by SFML");
	sfml.SetSize(16);
	sfml.SetColor(sf::Color::White);
	
	sf::FloatRect rect = title.GetRect();
	title.SetCenter(rect.GetWidth() / 2, rect.GetHeight() / 2);
	
	const int PADDING = 10;
	rect = sfml.GetRect();
	sfml.SetPosition(WIN_WIDTH - rect.GetWidth() - PADDING,
		WIN_HEIGHT - rect.GetHeight() - PADDING);
	
	sf::Sprite ship(GET_IMG("spaceship"));
	ship.SetPosition(-20, 100);
	
	AbstractController::Action action;
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
		float time = app_.GetFrameTime();
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
		ship.Move(180 * time, 25 * time); // FIXME: déplacement magique !
		title.Scale(0.99, 0.99); // FIXME: dépendant des FPS
		title.SetColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(255 * elapsed / DURATION)));
		title.SetPosition(WIN_WIDTH / 2, WIN_HEIGHT / 2);
		
		app_.Draw(background);
		app_.Draw(sfml);
		app_.Draw(title);
		app_.Draw(ship);
		app_.Display();
	}
		
	return what;
}


Game::Choice Game::Options()
{
	puts("-> Game::Options");
	puts("not implemented yet (quit)");
	return EXIT_APP;
}


Game::Choice Game::MainMenu()
{
#ifdef DEBUG
	puts("[ Game::MainMenu ]");
#endif
	Menu menu;
	menu.SetOffset(sf::Vector2f(42, 100));
	menu.AddItem("Mode Aventure", STORY_MODE);
	menu.AddItem("Mode Arcade", ARCADE_MODE);
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
			/*else if (event.Type == sf::Event::KeyPressed)
			{
				if (menu.ActionChosen(event.Key, choice))
				{
					running = false;
				}
			}
#ifdef JOYSTICK_ENABLED
			else if (event.Type == sf::Event::JoyButtonPressed)
			{
				if (menu.JActionChosen(event.JoyButton.Button, choice))
				{
					running = false;
				}
			}
			else if (event.Type == sf::Event::JoyMoved && event.JoyMove.Axis == sf::Joy::AxisY)
			{
				menu.JMoved(event.JoyMove.Position + settings_.GetCalibration()->y);
			}
#endif
*/
		}
		menu.Show(app_);
		app_.Display();

	}
	// si on lance une nouvelle partie, on nettoie les restes des éventuelles
	// parties précédentes et l'on respawn un joueur neuf
	if (choice == ARCADE_MODE || choice == STORY_MODE)
	{
		// clean up
		RemoveEntities();
		particles_.Clear();
		bullets_.Clear();
		
		
		// init
		timer_ = 0.0f;
		particles_.AddStars();
		Respawn();
		if (choice == STORY_MODE)
		{	
			cur_lvl_ = 0;
			choice = CONTINUE;
			arcade_ = false;
		}
		else
		{
			arcade_ = true;
		}
	}
	return static_cast<Choice>(choice);
}


Game::Choice Game::Play()
{
#ifdef DEBUG
	puts("[ Game::Play ]");
#endif
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
				GetPlayer()->HandleAction(action);
			}
			/*
			else if (event.Type == sf::Event::KeyPressed)
			{
				GetPlayer()->HandleKey(event.Key);
				if (event.Key.Code == key_pause_)
				{
					what = IN_GAME_MENU;
					running = false;
				}
				else if (event.Key.Code == sf::Key::Escape) // QUICK QUIT
				{
					running = false;
				}
			}
#ifdef JOYSTICK_ENABLED
			else if (event.Type == sf::Event::JoyButtonPressed)
			{
				GetPlayer()->HandleJoyButton(event.JoyButton);
				if (event.JoyButton.Button == settings_.GetJoyKey(Settings::jRETURN))
				{
					running = false;
				}
				else if (event.JoyButton.Button == settings_.GetJoyKey(Settings::jPAUSE))
				{
					what = IN_GAME_MENU;
					running = false;
				}
			}
#endif
*/
		}
	
		if (running)
		{
			running = MoreBadGuys() || entities_.size() > 1;
			if (!running)
			{
				// il n'y a plus d'ennemis en liste d'attente, et plus
				// d'ennemis dans le vecteur = VICTOIRE EPIC WIN
				// TODO: mettre un flag changer l'enum pour indiquer la victoire
				
				// On demande niveau suivant, si pas de niveau suivant, fin.

				if (cur_lvl_ > level_.GetLastID())
				{
					std::cerr << "Fin du dernier niveau\n";
					what = GAME_OVER;
				}
				else
				{
					what = INTERTITRE;
				   
				}
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
				if (player_.ship == *it)
				{
					running = false;
					what = GAME_OVER;
#ifdef DEBUG
					puts("\nyou are dead :(\n");
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
				if (player_.ship != *it &&
					player_.ship->GetRect().Intersects((**it).GetRect()))
				{
					// collision sur les deux éléments
					player_.ship->Collide(**it);
					(**it).Collide(*player_.ship);
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
	
	player_.best_time = timer_;
	return what;
}


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
	Choice resume = arcade_ ? ARCADE_MODE : STORY_MODE;
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
			/*if (event.Type == sf::Event::KeyPressed)
			{
				if (event.Key.Code == key_pause_)
				{
					
				}
				else if (menu.ActionChosen(event.Key, what))
				{
					paused = false;
				}
			}
#ifdef JOYSTICK_ENABLED
			else if (event.Type == sf::Event::JoyMoved && event.JoyMove.Axis == sf::Joy::AxisY)
			{
				menu.JMoved(event.JoyMove.Position + settings_.GetCalibration()->y);
			}
			else if (event.Type == sf::Event::JoyButtonPressed)
			{
					if (menu.JActionChosen(event.JoyButton.Button, what) )
					{
						paused = false;
					}
				if (event.JoyButton.Button == settings_.GetJoyKey(Settings::jRETURN))
				{
					what = EXIT_APP;
				}
			}
#endif*/
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


Game::Choice Game::GameOver()
{
#ifdef DEBUG
	puts("[ Game::GameOver ]");
#endif
	sf::String title;
	if (arcade_)
	{
		int min = (int) player_.best_time / 60;
		int sec = (int) player_.best_time % 60;
		title.SetText(str_sprintf("Tu as tenu seulement %d min et %d sec",
			min, sec));
	}
	else
	{
		title.SetText("Fin de niveau");
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
			}/*
			else if (event.Type == sf::Event::KeyPressed)
			{
				if (menu.ActionChosen(event.Key, choice))
				{
					running = false;
				}
			}
#ifdef JOYSTICK_ENABLED
			else if (event.Type == sf::Event::JoyButtonPressed)
			{
				if (event.JoyButton.Button == settings_.GetJoyKey(Settings::jRETURN))
				{
					choice = EXIT_APP;
				}
			}
			else if (event.Type == sf::Event::JoyMoved && event.JoyMove.Axis == sf::Joy::AxisY)
			{
				menu.JMoved(event.JoyMove.Position + settings_.GetCalibration()->y);
			}
#endif*/
		}
		app_.Draw(title);
		menu.Show(app_);
		app_.Display();
	}
	return static_cast<Choice>(choice);
}


Game::Choice Game::Intertitre()
{
#ifdef DEBUG
	puts("[ Game::Intertitre ]");
#endif
	sf::String title;
	sf::String subtitle1;
	sf::String subtitle2;
	int min = (int) player_.best_time / 60;
	int sec = (int) player_.best_time % 60;
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
		level_.Set(++cur_lvl_, level_desc_);
		find_replace(level_desc_, "\\n", "\n");
		title.SetText(str_sprintf("Niveau %d", cur_lvl_));
		subtitle.SetText(level_desc_);
		
		menu.AddItem("Jouer", STORY_MODE);  
		 player_.Place();
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
			/*if (event.Type == sf::Event::Closed)
			{
				running = false;
			}
			else if (event.Type == sf::Event::KeyPressed)
			{
				if (menu.ActionChosen(event.Key, choice))
				{
					running = false;
				}
				if (event.Key.Code == HACKY_KEY)
				{
					//<HACK>
					Passwd_HACK();
					level_.Set(cur_lvl_, level_desc_);
					find_replace(level_desc_, "\\n", "\n");
					title.SetText(str_sprintf("Niveau %d", cur_lvl_));
					subtitle.SetText(level_desc_);
					//</HACK>
				}
			}*/
		}
		app_.Draw(subtitle);
		app_.Draw(title);
		menu.Show(app_);
		app_.Display();
	}
	return static_cast<Choice>(choice);
}

// retourne true s'il y a encore des ennemis à envoyer plus tard
// sinon false (niveau fini, bravo :D)
bool Game::MoreBadGuys()
{
	// si mode arcade
	if (arcade_)
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
				AddEntity(Ennemy::Make(Ennemy::INTERCEPTOR, offset, GetPlayer()));
			}
			else if (random > 4)
			{
				AddEntity(Ennemy::Make(Ennemy::BLORB, offset, GetPlayer()));
			}
			else if (random > 2)
			{
				AddEntity(Ennemy::Make(Ennemy::DRONE, offset, GetPlayer()));
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
	
	Level& level = Level::GetInstance();
	// sinon mode histoire
	Entity* p = level.GiveNext(timer_);
	while (p != NULL)
	{
		AddEntity(p);
		p = level.GiveNext(timer_);
	}
	
	
	// si le niveau n'est pas fini, alors il y a encore des ennemis
	return level.RemainingEntities() > 0;

}


void Game::AddEntity(Entity* entity)
{
	entities_.push_front(entity);
}


void Game::RemoveEntities()
{
	// Ne pas détruire l'instance si elle héberge une thread
	// qui tourne, sinon vlam :)
	// alexandre: euh... est-ce un TODO ? si oui -> à faire dans ~PlayerShip, non ?
	std::list<Entity*>::iterator it;
	for (it = entities_.begin(); it != entities_.end(); ++it)
	{
		delete *it;
	}
	entities_.clear();
}


void Game::Respawn()
{
	sf::Vector2f offset;
	offset.x = 0;
	offset.y = WIN_HEIGHT / 2.0;
	player_.ship = new PlayerShip(offset);
	AddEntity(GetPlayer());

}


PlayerShip* Game::GetPlayer() const
{
	return player_.ship;
}


std::string Game::MakePassword()
{
	Password pass;	
	unsigned char lives, level, shield, icecubes;
	
	lives = static_cast<unsigned char>(GetPlayer()->GetHP());
	level = static_cast<unsigned char>(cur_lvl_);
	icecubes = static_cast<unsigned char>(GetPlayer()->GetCoolers());
	shield = static_cast<unsigned char>(GetPlayer()->GetShield());
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

	GetPlayer()->SetHP(static_cast<int>(pass_1_));
	GetPlayer()->SetShield(static_cast<int>(pass_3_));
	GetPlayer()->SetCoolers(static_cast<int>(pass_4_));
	
	panel_.SetShipHP(GetPlayer()->GetHP());
	panel_.SetShield(GetPlayer()->GetShield());
	panel_.SetCoolers(GetPlayer()->GetCoolers());
	
	if (level_.Set(pass_2_, level_desc_) == Level::SUCCESS) 
	{
		cur_lvl_ = static_cast<int>(pass_2_);
		find_replace(level_desc_, "\\n", "\n");
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
/*
void Game::CalibrateJoystick() 
{
	sf::Input i;
	sf::Vector2f res;
	res.x = 0.f - i.GetJoystickAxis(1, sf::Joy::AxisX);
	res.y = 0.f - i.GetJoystickAxis(1, sf::Joy::AxisY);
	settings_.SetCalibration(res);
}*/

