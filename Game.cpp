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
#include "Math.hpp"
#include "Window.hpp"
#include "ConfigParser.hpp"

#define CONFIG_FILE "config/config.txt"

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
	particles_	(ParticleSystem::GetInstance())
{
	// loading config
	ConfigParser config;
	bool fullscreen = false;
	last_level_reached_ = 1;
	current_level_ = 1;
	best_arcade_time_ = 0;
	if (config.LoadFromFile(CONFIG_FILE))
	{
		config.SeekSection("Settings");
		config.ReadItem("last_level_reached", last_level_reached_);
		config.ReadItem("current_level", current_level_);
		config.ReadItem("best_arcade_time", best_arcade_time_);
		config.ReadItem("fullscreen", fullscreen);
		
		controls_.LoadConfig(config);
	}
	
	p_ForwardAction_ = NULL;
	p_StopPlay_ = NULL;
	player1_ = player2_ = NULL;
	
	if (!fullscreen)
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
}


Game::~Game()
{
	RemoveEntities();
	app_.EnableKeyRepeat(true); // bug SFML 1.3 sous linux : il faut réactiver à la main le keyrepeat
	app_.Close();
	
	// writing configuration
	ConfigParser config;
	config.SeekSection("Settings");
	config.WriteItem("last_level_reached", last_level_reached_);
	config.WriteItem("current_level", current_level_);
	config.WriteItem("best_arcade_time", best_arcade_time_);
	
	controls_.SaveConfig(config);
	config.SaveToFile(CONFIG_FILE);
}


void Game::Run()
{
	puts("running...");
	// première scène = Intro
	Scene what = Intro();
#ifndef NO_AUDIO
#ifndef NO_MUSIC
	music_ = GET_MUSIC("space");
	music_->SetVolume(30);
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
			case PLAY:
				what = Play();
				break;
			case IN_GAME_MENU:
				what = InGameMenu();
				break;
			case ABOUT:
				what = About();
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
			case ARCADE_RESULT:
				what = ArcadeResult();
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
Game::Scene Game::Intro()
{
#ifdef DEBUG
	puts("[ Game::Intro ]");
#endif
	AC::Action action;
	
	Scene what = MAIN_MENU;
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
		ship.Move(180 * time, 25 * time);
		title.Scale(0.99, 0.99); // FIXME: dépendant des FPS
		title.SetColor(sf::Color(255, 255, 255,
			(sf::Uint8) (255 * elapsed / DURATION)));
		
		app_.Draw(background);	app_.Draw(sfml);
		app_.Draw(title);		app_.Draw(ship);
		app_.Display();
	}
	return what;
}


Game::Scene Game::MainMenu()
{
#ifdef DEBUG
	puts("[ Game::MainMenu ]");
#endif
	sf::String title("CosmoScroll");
	title.SetFont(GET_FONT());
	title.SetSize(60);
	title.SetY(42);
	title.SetX((WIN_WIDTH - title.GetRect().GetWidth()) / 2);
	
	sf::Sprite back(GET_IMG("main-screen"));
	Menu menu;
	menu.SetOffset(sf::Vector2f(80, 130));
	menu.AddItem("Mode Histoire solo", 0);
	menu.AddItem("Mode Histoire duo", 1);
	menu.AddItem("Mode Arcade", 2);
	//menu.AddItem("Options", 3); -> choix musique, choix bindings
	menu.AddItem(L"À propos", 4);
	//menu.AddItem("Pong", PONG_MODE);
	menu.AddItem("Quitter", 5);

	bool running = true;
	int choice = -1;
	Scene next;
	AC::Action action;
	while (running)
	{
		while (controls_.GetAction(action))
		{
			if (action == AC::EXIT_APP)
			{
				running = false;
				next = EXIT_APP;
			}
			else if (menu.ItemChosen(action, choice))
			{
				running = false;
			}
		}
		app_.Draw(back);
		app_.Draw(title);
		menu.Show(app_);
		app_.Display();
	}

	// si on lance une nouvelle partie, on nettoie les restes des éventuelles
	// parties précédentes et l'on respawn un joueur neuf
	switch (choice)
	{
		case 0:
			mode_ = STORY;
			next = SELECT_LEVEL;

			break;
		case 1:
			mode_ = STORY2X;
			next = SELECT_LEVEL;
			break;
		case 2:
			mode_ = ARCADE;
			next = PLAY;
			panel_.SetGameInfo(str_sprintf("Record : %02d:%02d",
				(int) best_arcade_time_ / 60,
				(int) best_arcade_time_ % 60).c_str());
			Init();
			p_ForwardAction_ = &Game::ForwardAction1P;
			p_StopPlay_ = &Game::ArcadeMoreBadGuys;
			break;
		/*case 3:// not implemented yet
			next = OPTIONS;
			break;*/
		case 4: 
			next = ABOUT;
			break;
		case 5:
			next = EXIT_APP;
			break;
		default:
			assert(choice == -1);
			break;
	}
	return next;
}


Game::Scene Game::Options()
{
	puts("[ Game::Options ]");
#ifdef DEBUG
	puts("not implemented yet");
#endif
	return MAIN_MENU;
}

// choix des niveaux
Game::Scene Game::SelectLevel()
{
#ifdef DEBUG
	puts("[ Game::SelectLevel ]");
#endif
	p_StopPlay_ = &Game::StoryMoreBadBuys;

	p_ForwardAction_ = mode_ == STORY2X ?
		&Game::ForwardAction2P : &Game::ForwardAction1P;
	
	sf::Sprite back(GET_IMG("main-screen"));
	Menu menu;
	menu.SetOffset(sf::Vector2f(80, 130));
	
	AC::Action action;
	
	bool running = true;
	Scene next = EXIT_APP;
	int last = levels_.GetLast();
	for (int i = 0; i < last; ++i)
	{
		bool activable = i < last_level_reached_;
		menu.AddItem(str_sprintf("Niveau %d", i + 1), i, activable);
	}
	menu.AddItem("Retour au menu principal", last);
	menu.SelectItem(current_level_ - 1);
	
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
					next = MAIN_MENU;
				}
				else
				{
					current_level_ = level + 1;
					
					Init();	
					levels_.Set(current_level_);
					panel_.SetGameInfo(str_sprintf("Niveau %i", current_level_).c_str());
					next = LEVEL_CAPTION;
				}
			}
		}
		app_.Draw(back);
		menu.Show(app_);
		app_.Display();
	}
	return next;
}


Game::Scene Game::LevelCaption()
{
#ifdef DEBUG
	puts("[ Game::LevelCaption ]");
#endif

	AC::Action action;
	bool running = true;
	Scene what;
	sf::Sprite back(GET_IMG("background"));
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
	
	while (running)
	{
		while (controls_.GetAction(action))
		{
			if (action == AC::EXIT_APP)
			{
				running = false;
				puts("LevelCaption: EXIT_APP");
				what = EXIT_APP;
			}
			else if (action == AC::VALID)
			{
				running = false;
				puts("LevelCaption: PLAY");
				what = PLAY;
			}
		}
		app_.Draw(back);
		app_.Draw(description);
		app_.Display();
	}
	return what;
}


// la boucle de jeu
Game::Scene Game::Play()
{
#ifdef DEBUG
	puts("[ Game::Play ]");
#endif
	timer_ = 0.f;
	AC::Action action;
	AC::Device device;
	bool running = true;
	Scene next = EXIT_APP;
	std::list<Entity*>::iterator it;
	
	while (running)
	{
		while (controls_.GetAction(action, &device))
		{
			if (action == AC::EXIT_APP)
			{
				running = false;
			}
			else if (action == AC::PAUSE)
			{
				running = false;
				next = IN_GAME_MENU;
			}
			else
			{
				(this->*p_ForwardAction_)(action, device);
			}
		}
		
		if (running && (this->*p_StopPlay_)())
		{
			running = false;
			next = END_PLAY;
		}
		
		// action
		for (it = entities_.begin(); it != entities_.end(); ++it)
		{
			(**it).Action();
		}
		
		// moving
		float frametime = app_.GetFrameTime();
		timer_ += frametime;
		panel_.SetTimer(timer_);
		Entity* ship = player1_; // FIXME toujours player1_
		sf::FloatRect player_rect = ship->GetRect();
		for (it = entities_.begin(); it != entities_.end();)
		{
			if ((**it).IsDead())
			{
				if (ship == *it)
				{
					running = false;
					next = END_PLAY;
					break;
				}
				bullets_.CleanSenders(*it);
				delete *it;
				it = entities_.erase(it);
			}
			else
			{
				(**it).Move(frametime);
				// collision Joueur <-> autres unités
				if (ship != *it && player_rect.Intersects((**it).GetRect()))
				{
					// collision sur les deux éléments;
					ship->Collide(**it);
					(**it).Collide(*ship);
					particles_.AddImpact((**it).GetPosition(), 10);
				}
				++it;
			}
		}
		
		bullets_.Update(frametime);
		bullets_.Collide(entities_);
		particles_.Update(frametime);
		
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
	return next;
}


Game::Scene Game::About()
{

	bool running = true;
	int choice = MAIN_MENU;
	AC::Action action;
	Menu menu;
	sf::String info;
	info.SetText(COSMOSCROLL_ABOUT);
	info.SetFont(GET_FONT());
	info.SetColor(sf::Color::White);
	info.SetPosition(120, 60);
	
	sf::Sprite back(GET_IMG("main-screen"));
	menu.SetOffset(sf::Vector2f(120, 320));
	menu.AddItem("Retour", MAIN_MENU);	
	

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
		app_.Draw(back);
		app_.Draw(info);
		menu.Show(app_);
		app_.Display();
	}
	return static_cast<Scene>(choice);
}


// pause avec un menu
Game::Scene Game::InGameMenu()
{
#ifdef DEBUG
	puts("[ Game::InGameMenu ]");
#endif
	AC::Action action;

	sf::String title("PAUSE");
	title.SetPosition(200, 130);
	title.SetSize(60);
	title.SetFont(GET_FONT());
	
	Menu menu;
	menu.SetOffset(sf::Vector2f(200, 210));
	menu.AddItem("Reprendre la partie", PLAY);
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
				what = PLAY;
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

	return static_cast<Scene>(what);

}


// la partie est finie
Game::Scene Game::EndPlay()
{
#ifdef DEBUG
	puts("[ Game::EndPlay ]");
#endif
#ifndef NO_AUDIO
	sf::Sound sound;
#endif
	const float DURATION = 5;
	float timer = 0;
	Scene what;
	sf::String info;
	info.SetSize(70);
	info.SetColor(sf::Color::White);
	info.SetFont(GET_FONT());
	
	// si perdu
	if ((entities_.size() > 1 && mode_ != STORY2X) ||
		(entities_.size() > 2 && mode_ == STORY2X))
	{
#ifndef NO_AUDIO
		sound.SetBuffer(GET_SOUNDBUF("game-over"));
#endif
		info.SetText("Game Over");
		what = mode_ == ARCADE ? ARCADE_RESULT : MAIN_MENU;
	}
	else if (current_level_ < levels_.GetLast()) // on ne peut pas gagner en arcade
	{
#ifndef NO_AUDIO
		sound.SetBuffer(GET_SOUNDBUF("end-level"));
#endif
		info.SetText(str_sprintf(L"Niveau %d terminé", current_level_));
		++current_level_;
		if (current_level_ > last_level_reached_)
		{
			last_level_reached_ = current_level_;
#ifdef DEBUG
			printf("nouveau niveau atteint : %d\n", last_level_reached_);
#endif
		}
		what = SELECT_LEVEL;
	}
	else // si dernier niveau du jeu
	{
#ifndef NO_AUDIO
		sound.SetBuffer(GET_SOUNDBUF("end-level"));
#endif
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
#ifndef NO_AUDIO
	sound.Play();
#endif	
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
			(sf::Uint8)(255 - 255 * timer / DURATION)));
		
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
Game::Scene Game::ArcadeResult()
{
#ifdef DEBUG
	puts("[ Game::ArcadeResult ]");
#endif
	assert(mode_ = ARCADE);
	
	sf::String title;
	std::string content;
	int min = (int) timer_ / 60;
	int sec = (int) timer_ % 60;
	if (timer_ > best_arcade_time_)
	{
		best_arcade_time_ = timer_;
		content = str_sprintf("Nouveau record ! %d min et %d sec !", min, sec);
	}
	else
	{
		content = str_sprintf("Vous avez tenu seulement %d min et %d sec",
			min, sec);
	}
	title.SetText(content);
	title.SetFont(GET_FONT());
	title.SetColor(sf::Color::White);
	title.SetPosition(42, 42);
	sf::Sprite back(GET_IMG("background"));
	
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
		app_.Draw(back);
		app_.Draw(title);
		menu.Show(app_);
		app_.Display();
	}
	return static_cast<Scene>(choice);
}



void Game::RemoveEntities()
{
	std::cerr << "RemoveEntities\n";
	std::list<Entity*>::iterator it;
	for (it = entities_.begin(); it != entities_.end(); ++it)
	{
		delete *it;
	}
	entities_.clear();
}


void Game::AddEntity(Entity* entity)
{
	entities_.push_front(entity);
}


void Game::Init()
{
	RemoveEntities();
	
	assert(entities_.empty());
	sf::Vector2f offset;
	offset.x = 0;
	player1_ = new PlayerShip(sf::Vector2f(), "spaceship-red");
	AddEntity(player1_);
	
	if (mode_ == STORY2X)
	{
		player2_ = new PlayerShip(sf::Vector2f(), "spaceship-green");
		AddEntity(player2_);
		
		player1_->SetControls(AC::KEYBOARD);
		player2_->SetControls(AC::JOYSTICK);
		offset.y = WIN_HEIGHT * 2 / 3;
		player2_->SetPosition(offset);
		offset.y = WIN_HEIGHT / 3;
	}
	else
	{
		offset.y = WIN_HEIGHT / 2;
	}
	player1_->SetPosition(offset);
	
	bullets_.Clear();
	particles_.Clear();
	particles_.AddStars();
}


/* behaviors */

void Game::ForwardAction1P(AC::Action action, AC::Device device)
{
	(void) device;
	player1_->HandleAction(action);	
}


void Game::ForwardAction2P(AC::Action action, AC::Device device)
{
	if (!player1_->IsDead() && device == player1_->GetControls())
	{
		player1_->HandleAction(action);
	}
	else if (!player2_->IsDead() && device == player2_->GetControls())
	{
		player2_->HandleAction(action);
	}
}


bool Game::ArcadeMoreBadGuys()
{
	if (entities_.size() < (size_t) timer_ / 10 + 5)
	{
		// ajout d'une ennemi au hasard
		// <hack>
		Entity* ship = player1_; // toujours le joueur 1 ... :/
		sf::Vector2f offset;
		offset.x = WIN_WIDTH;
		offset.y = sf::Randomizer::Random(CONTROL_PANEL_HEIGHT, WIN_HEIGHT);
		int random = sf::Randomizer::Random(0, 10);
		if (random > 6)
		{
			AddEntity(Ennemy::Make(Ennemy::INTERCEPTOR, offset, ship));
		}
		else if (random > 4)
		{
			AddEntity(Ennemy::Make(Ennemy::BLORB, offset, ship));
		}
		else if (random > 2)
		{
			AddEntity(Ennemy::Make(Ennemy::DRONE, offset, ship));
		}
		else
		{
			AddEntity(new Asteroid(offset, Asteroid::BIG));
		}
		// </hack>
	}
	return false;
}


bool Game::StoryMoreBadBuys()
{
	Entity* p = levels_.GiveNext(timer_);
	while (p != NULL)
	{
		AddEntity(p);
		p = levels_.GiveNext(timer_);
	}
	
	// le niveau n'est pas fini tant qu'il reste des ennemis, soit en file
	// d'attente, soit dans le conteur entities_
	return (levels_.RemainingEntities() == 0)
		&& (entities_.size() == (mode_ == STORY2X ? 2 : 1));
}


/*
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
}*/


