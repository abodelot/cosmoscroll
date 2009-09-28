#include <iostream>
#include <typeinfo>
#include <SFML/System.hpp>

#include "Game.hpp"
#include "SoundSystem.hpp"
#include "Window.hpp"
#include "LevelManager.hpp"
#include "../entities/EntityManager.hpp"
#include "../utils/MediaManager.hpp"
#include "../menu/Menu.hpp"
#include "../utils/StringUtils.hpp"
#include "../utils/Math.hpp"
#include "../utils/DIE.hpp"
#include "../utils/ConfigParser.hpp"

#define CONFIG_FILE "config/config.txt"

#define MARGIN_X 120

#define XML_WEAPONS    "data/xml/weapons.xml"
#define XML_ANIMATIONS "data/xml/animations.xml"
#define XML_SPACESHIPS "data/xml/spaceships.xml"


Game& Game::GetInstance()
{
	static Game self;
	return self;
}


Game::Game() :
	controls_	(AbstractController::GetInstance()),
	panel_		(ControlPanel::GetInstance()),
	levels_		(LevelManager::GetInstance()),
	particles_	(ParticleSystem::GetInstance()),
	entitymanager_(EntityManager::GetInstance())
{
	puts("boot sur le flex ...");
	// init entity manager
	entitymanager_.LoadWeapons(XML_WEAPONS);
	entitymanager_.LoadAnimations(XML_ANIMATIONS);
	entitymanager_.LoadSpaceShips(XML_SPACESHIPS);
	entitymanager_.SetPosition(0, ControlPanel::HEIGHT);

	particles_.SetPosition(0, ControlPanel::HEIGHT);
	// loading config
	ConfigParser config;
	bool fullscreen = false;
	last_level_reached_ = 1;
	current_level_ = 1;
	best_arcade_time_ = 0;
	music_ = NULL;
	music_name_ = "space";

	if (config.LoadFromFile(CONFIG_FILE))
	{
		config.SeekSection("Settings");
		config.ReadItem("last_level_reached", last_level_reached_);
		config.ReadItem("current_level", current_level_);
		config.ReadItem("best_arcade_time", best_arcade_time_);
		config.ReadItem("fullscreen", fullscreen);
		config.ReadItem("music", music_name_);
		controls_.LoadConfig(config);
	}

	p_ForwardAction_ = NULL;
	p_StopPlay_ = NULL;
	player1_ = player2_ = NULL;
	player_dead_ = false;

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
	const sf::Image& icon = GET_IMG("icon");
	app_.SetIcon(icon.GetWidth(), icon.GetHeight(), icon.GetPixelsPtr());

	background_ = sf::Shape::Rectangle(0, ControlPanel::HEIGHT, WIN_WIDTH, WIN_HEIGHT, sf::Color::White);
}


Game::~Game()
{
	entitymanager_.Clear();
	app_.Close();

	// writing configuration
	ConfigParser config;
	config.SeekSection("Settings");
	config.WriteItem("last_level_reached", last_level_reached_);
	config.WriteItem("current_level", current_level_);
	config.WriteItem("best_arcade_time", best_arcade_time_);
	config.WriteItem("music", music_name_);

	controls_.SaveConfig(config);
	config.SaveToFile(CONFIG_FILE);
}


void Game::Run()
{
	// première scène = Intro
	Scene what = Intro();
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
			case OPTIONS:
				what = Options();
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

	StopMusic();
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
	const int DURATION = 6;
	float time, elapsed = 0;
	bool played = false;

	sf::Sprite background(GET_IMG("background"));

	sf::Sprite title(GET_IMG("cosmoscroll-logo"));
	title.SetCenter(title.GetSize().x / 2, title.GetSize().y / 2);
	title.SetPosition(WIN_WIDTH / 2, WIN_HEIGHT / 2);
	title.Resize(title.GetSize().x * 7, title.GetSize().y * 7);
	MediaManager::GetInstance().SmoothImage("cosmoscroll-logo", true);

	PlayerShip ship(sf::Vector2f(-80, 100), "playership-red");

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
		if (!played && elapsed >= 2)
		{
			played = true;
			SoundSystem::GetInstance().PlaySound("title");
		}

		ship.Update(time);
		ship.Move(170 * time, 25 * time);
		title.Scale(0.99, 0.99); // FIXME: dépendant des FPS
		title.SetColor(sf::Color(255, 255, 255,
			(sf::Uint8) (255 * elapsed / DURATION)));

		app_.Draw(background);
		app_.Draw(title);
		app_.Draw(ship);

		app_.Display();
	}

	if (music_name_ != "NULL")
	{
		LoadMusic(music_name_.c_str());
	}
	SoundSystem::GetInstance().SetSoundVolume(60);
	return what;
}


Game::Scene Game::MainMenu()
{
#ifdef DEBUG
	puts("[ Game::MainMenu ]");
#endif
	sf::Sprite title(GET_IMG("cosmoscroll-logo"));

	title.SetY(42);
	title.SetX((WIN_WIDTH - title.GetSize().x) / 2);

	sf::Sprite back(GET_IMG("main-screen"));
	Menu menu;
	menu.SetOffset(sf::Vector2f(MARGIN_X, 130));
	menu.AddItem("Mode Histoire"/*"Mode Histoire solo"*/, 0);
	//menu.AddItem("Mode Histoire duo", 1);
	menu.AddItem("Mode Arcade", 2);
	menu.AddItem("Options", 3);
	menu.AddItem(L"À propos", 4);
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
			timer_ = 0.f;
			p_ForwardAction_ = &Game::ForwardAction1P;
			p_StopPlay_ = &Game::ArcadeMoreBadGuys;
			SetBackgroundColor(sf::Color::Black, sf::Color::Black);
			break;
		case 3:
			next = OPTIONS;
			break;
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


// choix des niveaux
Game::Scene Game::SelectLevel()
{
#ifdef DEBUG
	puts("[ Game::SelectLevel ]");
#endif
	p_StopPlay_ = &Game::StoryMoreBadBuys;

	p_ForwardAction_ = mode_ == STORY2X ?
		&Game::ForwardAction2P : &Game::ForwardAction1P;
	timer_ = 0.f;

	sf::Sprite back(GET_IMG("main-screen"));
	Menu menu;
	menu.SetOffset(sf::Vector2f(MARGIN_X, 130));

	AC::Action action;

	bool running = true;
	Scene next = EXIT_APP;
	int last = levels_.CountLevel();
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
					levels_.SetCurrent(current_level_);
					panel_.SetGameInfo(str_sprintf("Niveau %i", current_level_).c_str());
					next = LEVEL_CAPTION;
					SetBackgroundColor(levels_.GetTopColor(current_level_),
									   levels_.GetBottomColor(current_level_));
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
	Scene what = EXIT_APP;
	sf::Sprite back(GET_IMG("background"));
	std::string content = str_sprintf("Niveau %d\n\n%s", current_level_,
		levels_.GetDescription(current_level_));
	str_replace(content, "\\n", "\n");

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
			}
			else if (action == AC::VALID)
			{
				running = false;
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
			else if (action == AC::TAKE_SCREENSHOT)
			{
				TakeScreenshot("screenshot");
			}
			else
			{
				(this->*p_ForwardAction_)(action, device);
			}
		}

		if ((this->*p_StopPlay_)() || player_dead_)
		{
			running = false;
			next = END_PLAY;
		}
		else
		{
			app_.Clear();

			float frametime = app_.GetFrameTime();
			timer_ += frametime;
			panel_.SetTimer(timer_);

			entitymanager_.Update(frametime);
			particles_.Update(frametime);

			// RENDER
			app_.Draw(background_);
			app_.Draw(particles_);
			app_.Draw(entitymanager_);

			panel_.Show(app_);
			app_.Display();
		}
	}
	return next;
}


void Game::NotifyPlayerDead()
{
	player_dead_ = true;
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
	info.SetPosition(MARGIN_X, 60);

	sf::Sprite back(GET_IMG("main-screen"));
	menu.SetOffset(sf::Vector2f(MARGIN_X, 340));
	menu.AddItem("Retour", MAIN_MENU);

	sf::Sprite logos(GET_IMG("libs-logo"));
	logos.SetY(WIN_HEIGHT - logos.GetSize().y - 10);

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
		app_.Draw(logos);
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
		app_.Draw(entitymanager_);
		app_.Draw(particles_);
		panel_.Show(app_);

		app_.Draw(title);
		menu.Show(app_);
		app_.Display();
		app_.Clear();
	}

	return static_cast<Scene>(what);

}


// la partie est finie
Game::Scene Game::EndPlay()
{
#ifdef DEBUG
	puts("[ Game::EndPlay ]");
#endif
	const float DURATION = 7;
	float timer = 0;
	Scene what;
	sf::String info;
	info.SetSize(70);
	info.SetColor(sf::Color::White);
	info.SetFont(GET_FONT());

	// si perdu
	if ((entitymanager_.Count() > 1 && mode_ != STORY2X) ||
		(entitymanager_.Count() > 2 && mode_ == STORY2X))
	{
		SoundSystem::GetInstance().PlaySound("game-over");

		info.SetText("Game Over");
		what = mode_ == ARCADE ? ARCADE_RESULT : MAIN_MENU;
	}
	else if (current_level_ < levels_.CountLevel()) // on ne peut pas gagner en arcade
	{
		SoundSystem::GetInstance().PlaySound("end-level");

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
		SoundSystem::GetInstance().PlaySound("end-level");

		std::wstring epic_win = str_sprintf(L"Félicitations !\n\n"
			"Vous avez fini les %d niveaux du jeu.\n"
			"Vous êtes vraiment doué(e) ! :D", current_level_);
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

		// rendering
		app_.Draw(particles_);
		app_.Draw(entitymanager_);
		panel_.Show(app_);
		app_.Draw(info);
		app_.Display();
		app_.Clear();
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
	title.SetPosition(42, 100);
	sf::Sprite back(GET_IMG("background"));

	Menu menu;
	menu.SetOffset(sf::Vector2f(MARGIN_X, 200));
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


void Game::Init()
{
	entitymanager_.Clear();

	sf::Vector2f offset;
	offset.x = 0;
	player_dead_ = false;
	player1_ = new PlayerShip(sf::Vector2f(), "playership-red");
	entitymanager_.AddEntity(player1_);

	if (mode_ == STORY2X)
	{
		player2_ = new PlayerShip(sf::Vector2f(), "playership-green");
		entitymanager_.AddEntity(player2_);

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

	particles_.Clear();
	particles_.AddStars();
}


/* behaviors */

void Game::ForwardAction1P(AC::Action action, AC::Device)
{
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
	// number of max bad guys = time / STEP + START
	const int STEP = 8;
	const int START = 4;
	if (entitymanager_.Count() < timer_ / STEP + START)
	{
		Entity* entity = entitymanager_.CreateRandomEntity();
		sf::Vector2f pos;
		pos.x = WIN_WIDTH;
		pos.y = sf::Randomizer::Random(0, GAME_HEIGHT - (int) entity->GetSize().y);
		entity->SetPosition(pos);
		entity->SetTarget(player1_);

		entitymanager_.AddEntity(entity);
	}
	// always false, kill you till you die
	return false;
}


bool Game::StoryMoreBadBuys()
{
	Entity* p = levels_.GiveNextEntity(timer_);
	while (p != NULL)
	{
		entitymanager_.AddEntity(p);
		p = levels_.GiveNextEntity(timer_);
	}

	// le niveau n'est pas fini tant qu'il reste des ennemis, soit en file
	// d'attente, soit dans le gestionnaire d'entités
	return (levels_.RemainingEntities() == 0)
		&& (entitymanager_.Count() == (mode_ == STORY2X ? 2 : 1));
}


void Game::LoadMusic(const char* music_name)
{
	StopMusic();
#ifndef NO_DUMB_MUSIC
	music_ = GET_DUMB_MUSIC(music_name);
	music_->SetVolume(30);
	music_->Play();
#endif
	music_name_ = music_name;
}


void Game::StopMusic()
{
#ifndef NO_DUMB_MUSIC
	if (music_ != NULL)
	{
		music_->Stop();
		delete music_;
		music_ = NULL;
	}
#endif
}

// Menu des options

enum OptionMenu
{
	OPT_MAIN, OPT_MUSIC, OPT_JOYSTICK, OPT_KEYBOARD
};


void load_menu(OptionMenu what, Menu& menu, sf::String& title)
{
	menu.Clear();
	AC& controls = AC::GetInstance();
	switch (what)
	{
		case OPT_MAIN:
			title.SetText("Options");
			menu.AddItem("Configuration clavier", 1);
			menu.AddItem("Configuration joystick", 2);
			menu.AddItem("Musique", 3);
			break;
		case OPT_MUSIC:
			title.SetText("Musique");
			menu.AddItem("Space", 1);
			menu.AddItem("Aurora", 2);
			menu.AddItem("Escape For Assault", 3);
			menu.AddItem("Pas de musique", 4);
			break;
		case OPT_KEYBOARD:
			title.SetText("Clavier");
			menu.SetLineSpace(4);
			menu.AddItem(str_sprintf("Haut : %s", AC::KeyToString(
				controls.GetBinding(AC::MOVE_UP, AC::KEYBOARD))), 1);
			menu.AddItem(str_sprintf("Bas : %s", AC::KeyToString(
				controls.GetBinding(AC::MOVE_DOWN, AC::KEYBOARD))), 2);
			menu.AddItem(str_sprintf("Gauche : %s", AC::KeyToString(
				controls.GetBinding(AC::MOVE_LEFT, AC::KEYBOARD))), 3);
			menu.AddItem(str_sprintf("Droite : %s", AC::KeyToString(
				controls.GetBinding(AC::MOVE_RIGHT, AC::KEYBOARD))), 4);
			menu.AddItem(str_sprintf("Arme 1 : %s", AC::KeyToString(
				controls.GetBinding(AC::WEAPON_1, AC::KEYBOARD))), 5);
			menu.AddItem(str_sprintf("Arme 2 : %s", AC::KeyToString(
				controls.GetBinding(AC::WEAPON_2, AC::KEYBOARD))), 6);
			menu.AddItem(str_sprintf(L"Utiliser bonus Glaçon : %s", AC::KeyToString(
				controls.GetBinding(AC::USE_COOLER, AC::KEYBOARD))), 7);
			menu.AddItem(str_sprintf("Pause : %s", AC::KeyToString(
				controls.GetBinding(AC::PAUSE, AC::KEYBOARD))), 8);
			break;
		case OPT_JOYSTICK:
			title.SetText("Joystick");
			menu.AddItem(str_sprintf("Arme 1 : bouton %u",
				controls.GetBinding(AC::WEAPON_1, AC::JOYSTICK)), 1);
			menu.AddItem(str_sprintf("Arme 2 : bouton %u",
				controls.GetBinding(AC::WEAPON_2, AC::JOYSTICK)), 2);
			menu.AddItem(str_sprintf(L"Utiliser bonus Glaçon : bouton %u",
				controls.GetBinding(AC::USE_COOLER, AC::JOYSTICK)), 3);
			menu.AddItem(str_sprintf("Pause : bouton %u",
				controls.GetBinding(AC::PAUSE, AC::JOYSTICK)), 4);
			menu.AddItem(str_sprintf("Valider : bouton %u",
				controls.GetBinding(AC::VALID, AC::JOYSTICK)), 5);
			break;
	}
	menu.AddItem("Retour", 0);
}


bool get_input(AC::Device device, unsigned int& sfml_code)
{
	sf::String prompt;
	if (device == AC::KEYBOARD)
	{
		prompt.SetText(L"Appuyez sur une touche\n(Échap pour annuler)");
	}
	else if (device == AC::JOYSTICK)
	{
		prompt.SetText(L"Appuyez sur un bouton du contrôleur\n(Échap pour annuler)");
	}
	prompt.SetColor(sf::Color::White);
	sf::FloatRect rect = prompt.GetRect();
	prompt.SetPosition((WIN_WIDTH - rect.GetWidth()) / 2,
		(WIN_HEIGHT - rect.GetHeight()) / 2);

	sf::Event event;
	bool running = true;
	bool valid = true;
	sf::RenderWindow& app = Game::GetInstance().GetApp();
	while (running)
	{
		while (app.GetEvent(event))
		{
			if (event.Type == sf::Event::KeyPressed)
			{
				if (event.Key.Code == sf::Key::Escape)
				{
					running = false;
					valid = false;
				}
				else if (device == AC::KEYBOARD)
				{
					running = false;
					sfml_code = event.Key.Code;
				}
			}
			else if (event.Type == sf::Event::JoyButtonPressed
				&& device == AC::JOYSTICK)
			{
				running = false;
				sfml_code = event.JoyButton.Button;
			}
		}
		app.Draw(prompt);
		app.Display();
	}
	return valid;
}


Game::Scene Game::Options()
{
#ifdef DEBUG
	puts("[ Game::Options ]");
#endif

	OptionMenu current_menu = OPT_MAIN;
	Menu menu;
	menu.SetOffset(sf::Vector2f(MARGIN_X, 100));
	sf::String title;
	load_menu(current_menu, menu, title);
	title.SetFont(GET_FONT());
	title.SetColor(sf::Color::White);
	title.SetSize(40);
	title.SetY(40);
	title.SetX((WIN_WIDTH - title.GetRect().GetWidth()) / 2);

	sf::Sprite back(GET_IMG("main-screen"));
	bool running = true;
	AC::Action action;
	int id;
	Scene next = EXIT_APP;
	while (running)
	{
		while (controls_.GetAction(action))
		{
			if (action == AC::EXIT_APP)
			{
				running = false;
			}
			else if (menu.ItemChosen(action, id))
			{
				if (id == 0)
				{
					if (current_menu == OPT_MAIN)
					{
						running = false;
						next = MAIN_MENU;
					}
					else
					{
						// reloading OPT_MAIN menu
						current_menu = OPT_MAIN;
						load_menu(current_menu, menu, title);
					}
				}
				else
				{
					unsigned int sfml_code;
					AC::Action action_bind;
					switch (current_menu)
					{
						// comportement de l'id en fonction du menu courant
						case OPT_MAIN:
							// on va dans un sous menu
							switch (id)
							{
								case 1:
									current_menu = OPT_KEYBOARD;
									break;
								case 2:
									current_menu = OPT_JOYSTICK;
									break;
								case 3:
									current_menu = OPT_MUSIC;
									break;
								default:
									assert(0);
							}
							load_menu(current_menu, menu, title);
							break;
						case OPT_KEYBOARD:
							// on récupère le nouveau binding clavier
							if (!get_input(AC::KEYBOARD, sfml_code))
							{
								break;
							}
							switch (id)
							{
								case 1:
									action_bind = AC::MOVE_UP;
									break;
								case 2:
									action_bind = AC::MOVE_DOWN;
									break;
								case 3:
									action_bind = AC::MOVE_LEFT;
									break;
								case 4:
									action_bind = AC::MOVE_RIGHT;
									break;
								case 5:
									action_bind = AC::WEAPON_1;
									break;
								case 6:
									action_bind = AC::WEAPON_2;
									break;
								case 7:
									action_bind = AC::USE_COOLER;
									break;
								case 8:
									action_bind = AC::PAUSE;
									break;
								default:
									assert(0);
							}
							controls_.SetBinding(action_bind, AC::KEYBOARD, sfml_code);
							load_menu(OPT_KEYBOARD, menu, title);
							break;
						case OPT_JOYSTICK:
							// on récupère le nouveau binding joystick
							if (!get_input(AC::JOYSTICK, sfml_code))
							{
								break;
							}
							switch (id)
							{
								case 1:
									action_bind = AC::WEAPON_1;
									break;
								case 2:
									action_bind = AC::WEAPON_2;
									break;
								case 3:
									action_bind = AC::USE_COOLER;
									break;
								case 4:
									action_bind = AC::PAUSE;
									break;
								case 5:
									action_bind = AC::VALID;
									break;
								default:
									abort();
							}
							controls_.SetBinding(action_bind, AC::JOYSTICK, sfml_code);
							load_menu(OPT_JOYSTICK, menu, title);
							break;
						case OPT_MUSIC:
							// on change la musique
							switch (id)
							{
								case 1:
									LoadMusic("space");
									break;
								case 2:
									LoadMusic("aurora");
									break;
								case 3:
									LoadMusic("escape_for_assault");
									break;
								case 4:
									StopMusic();
									music_name_ = "NULL";
									break;
								default:
									abort();
							}
							break;
					}
				}
			}
		}
		app_.Draw(back);
		app_.Draw(title);
		menu.Show(app_);
		app_.Display();
		app_.Clear();
	}
	return next;
}


Entity* Game::GetPlayerShip() const
{
	if (player1_ == NULL)
	{
		DIE("can't retrieve player: playership is not allocated yet");
	}
	return player1_;
}


void Game::SetBackgroundColor(const sf::Color& topcolor, const sf::Color& bottomcolor)
{
	background_.SetPointColor(0, topcolor);
	background_.SetPointColor(1, topcolor);
	background_.SetPointColor(2, bottomcolor);
	background_.SetPointColor(3, bottomcolor);
}


void Game::TakeScreenshot(const char* directory)
{
	char current_time[256];
	time_t t = time(NULL);
	strftime(current_time, sizeof current_time, "%d-%m-%Y_%H-%M-%S", localtime(&t));

	std::string filename = str_sprintf("%s/%s.png", directory, current_time, t);

	printf("screenshot saved to %s\n", filename.c_str());
	app_.Capture().SaveToFile(filename);
}
