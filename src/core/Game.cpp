#include "Game.hpp"
#include "SoundSystem.hpp"
#include "LevelManager.hpp"
#include "ControlPanel.hpp"
#include "../entities/EntityManager.hpp"
#include "../utils/ConfigParser.hpp"
#include "../utils/StringUtils.hpp"

// scenes
#include "../scenes/BaseScene.hpp"
#include "../scenes/IntroScene.hpp"
#include "../scenes/MainMenu.hpp"
#include "../scenes/InGameScene.hpp"
#include "../scenes/EndGameScene.hpp"
#include "../scenes/EndGameMenu.hpp"
#include "../scenes/PauseMenu.hpp"
#include "../scenes/AboutMenu.hpp"
#include "../scenes/LevelMenu.hpp"
#include "../scenes/IntroLevelScene.hpp"
#include "../scenes/OptionMenu.hpp"
#include "../scenes/KeyboardMenu.hpp"
#include "../scenes/JoystickMenu.hpp"
#include "../scenes/AudioMenu.hpp"

// config and data files
#define CONFIG_FILE    "config/config.cfg"
#define LEVEL_FILE     "data/levels/levels.xml"
#define XML_WEAPONS    "data/xml/weapons.xml"
#define XML_ANIMATIONS "data/xml/animations.xml"
#define XML_SPACESHIPS "data/xml/spaceships.xml"

// constantes de configuration de la fenêtre
#define WIN_BPP     32
#define WIN_FPS     60
#define WIN_TITLE   "CosmoScroll"

Game& Game::GetInstance()
{
	static Game self;
	return self;
}


Game::Game() :
	input_	      (Input::GetInstance()),
	levels_		  (LevelManager::GetInstance()),
	particles_	  (ParticleSystem::GetInstance()),
	entitymanager_(EntityManager::GetInstance())
{
	// init level manager
	levels_.ParseFile(LEVEL_FILE);
	printf("info: %d levels loaded\n", levels_.CountLevel());

	// loading config
	LoadConfig(CONFIG_FILE);

	if (!fullscreen_)
	{
		app_.Create(sf::VideoMode(Game::WIDTH, Game::HEIGHT, WIN_BPP), WIN_TITLE);
	}
	else
	{
		app_.Create(sf::VideoMode(Game::WIDTH, Game::HEIGHT, WIN_BPP), WIN_TITLE,
			sf::Style::Fullscreen);
	}
	app_.Display();
	app_.SetFramerateLimit(WIN_FPS);
	app_.ShowMouseCursor(false);
	app_.EnableKeyRepeat(false);

	const sf::Image& icon = GET_IMG("gui/icon");
	app_.SetIcon(icon.GetWidth(), icon.GetHeight(), icon.GetPixelsPtr());

	// init entity manager
	entitymanager_.LoadWeapons(XML_WEAPONS);
	entitymanager_.LoadAnimations(XML_ANIMATIONS);
	entitymanager_.LoadSpaceShips(XML_SPACESHIPS);
	entitymanager_.SetPosition(0, ControlPanel::HEIGHT);

	input_.Init(app_.GetInput());

	// scenes will be allocated only if requested
	for (int i = 0; i < SC_COUNT; ++i)
	{
		scenes_[i] = NULL;
	}
	current_scene_ = NULL;
	running_ = true;
}


Game::~Game()
{
	SoundSystem::GetInstance().StopAll();
	MediaManager::GetInstance().Unload();

	entitymanager_.Clear();
	app_.Close();
	WriteConfig(CONFIG_FILE);

	// delete allocated scenes
	for (int i = 0; i < SC_COUNT; ++i)
	{
		if (scenes_[i] != NULL)
		{
			delete scenes_[i];
		}
	}
}


bool Game::LoadConfig(const char* filename)
{
	ConfigParser config;
	fullscreen_ = false;
	if (config.LoadFromFile(filename))
	{
		// reading settings
		config.SeekSection("Settings");
		int current_level = 1, last_unlocked_level = 1;
		config.ReadItem("current_level", current_level);
		levels_.SetCurrent(current_level);
		config.ReadItem("unlocked", last_unlocked_level);
		levels_.SetLastUnlocked(last_unlocked_level);

		float record = 0;
		config.ReadItem("best_arcade_time", record);
		entitymanager_.SetArcadeRecord(record);
		config.ReadItem("fullscreen", fullscreen_);

		int enable_music = 1;
		config.ReadItem("enable_music", enable_music);
		SoundSystem::GetInstance().EnableMusic(enable_music);
		std::string music;
		config.ReadItem("music_name", music);
		SoundSystem::GetInstance().SetMusicName(music);

		// reading keyboard and joystick bindings
		input_.LoadFromConfig(config);

		return true;
	}
	return false;
}


void Game::WriteConfig(const char* filename) const
{
	ConfigParser config;

	// writing settings
	config.SeekSection("Settings");
	config.WriteItem("last_unlocked_level", levels_.GetLastUnlocked());
	config.WriteItem("current_level", levels_.GetCurrent());
	config.WriteItem("best_arcade_time", entitymanager_.GetArcadeRecord());
	config.WriteItem("enable_music", (int) SoundSystem::GetInstance().IsMusicEnabled());
	config.WriteItem("music_name", SoundSystem::GetInstance().GetMusicName());

	// writing keyboard and joystick bindings
	input_.SaveToConfig(config);

	// saving configuration to file
	config.SaveToFile(filename);
}


void Game::Run()
{
	// set the first displayed scene at launch
	SetNextScene(SC_IntroScene);
	app_.Display();

	// game main loop which handle the current scene
	sf::Event event;
	Input::Action action;
	while (running_)
	{
		// 1. polling events
		while (app_.GetEvent(event))
		{
			action = input_.EventToAction(event);
			switch (action)
			{
				// these events are always handled on each scene
				case Input::EXIT_APP:
					running_ = false;
					break;
				case Input::TAKE_SCREENSHOT:
					TakeScreenshot("screenshot");
					break;
				// other events are send to the current scene
				default:
					current_scene_->OnEvent(event);
					break;
			}
		}
		app_.Clear();
		// 2. updating the current scene
		current_scene_->Update(app_.GetFrameTime());
		// 3. displaying the current scene
		current_scene_->Show(app_);
		app_.Display();
	}
}


sf::RenderWindow& Game::GetApp()
{
	return app_;
}


void Game::SetNextScene(Scene enum_scene)
{
#define CASE_SCENE(__scene__, scene_ptr) \
	case Game::SC_ ## __scene__:\
		scene_ptr = new __scene__();\
		break

	if (scenes_[enum_scene] == NULL)
	{
		BaseScene* new_scene = NULL;
		switch (enum_scene)
		{
			CASE_SCENE(IntroScene, new_scene);
			CASE_SCENE(MainMenu, new_scene);
			CASE_SCENE(InGameScene, new_scene);
			CASE_SCENE(EndGameScene, new_scene);
			CASE_SCENE(EndGameMenu, new_scene);
			CASE_SCENE(PauseMenu, new_scene);
			CASE_SCENE(AboutMenu, new_scene);
			CASE_SCENE(LevelMenu, new_scene);
			CASE_SCENE(IntroLevelScene, new_scene);
			CASE_SCENE(OptionMenu, new_scene);
			CASE_SCENE(KeyboardMenu, new_scene);
			CASE_SCENE(JoystickMenu, new_scene);
			CASE_SCENE(AudioMenu, new_scene);

			default:
				puts("warning: can't set next requested scene");
				abort();
				break;
		}
		scenes_[enum_scene] = new_scene;
	}
	current_scene_ = scenes_[enum_scene];
	current_scene_->Poke();
}


void Game::Quit()
{
	running_ = false;
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


