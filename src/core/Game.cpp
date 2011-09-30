#include "Game.hpp"
#include "Constants.hpp"
#include "SoundSystem.hpp"
#include "LevelManager.hpp"
#include "ControlPanel.hpp"
#include "entities/EntityManager.hpp"
#include "items/ItemManager.hpp"
#include "utils/MediaManager.hpp"
#include "utils/ConfigParser.hpp"
#include "utils/StringUtils.hpp"
#include "utils/I18n.hpp"
#include "utils/DIE.hpp"
#include "md5/md5.hpp"
#include "scenes/scenes.hpp"

// config and data files
#define CONFIG_FILE    "config/config.cfg"
#define LEVEL_FILE     "data/levels/levels.xml"
#define XML_ITEMS      "data/xml/items.xml"
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
	entitymanager_(EntityManager::GetInstance())
{
	CheckPurity();

	input_.Init(app_.GetInput());

	// init level manager
	levels_.ParseFile(LEVEL_FILE);

	// init entity manager
	ItemManager::GetInstance().LoadItems(XML_ITEMS);
	entitymanager_.LoadAnimations(XML_ANIMATIONS);
	entitymanager_.LoadSpaceShips(XML_SPACESHIPS);
	entitymanager_.SetPosition(0, ControlPanel::HEIGHT);

	// loading config
	LoadConfig(CONFIG_FILE);
	SetFullscreen(fullscreen_);
	app_.SetFramerateLimit(WIN_FPS);
	app_.ShowMouseCursor(false);
	app_.EnableKeyRepeat(false);

	MediaManager::GetInstance().CreateImageMask("gui/icon", sf::Color(0xff, 0, 0xff));
	const sf::Image& icon = GET_IMG("gui/icon");
	app_.SetIcon(icon.GetWidth(), icon.GetHeight(), icon.GetPixelsPtr());

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
	app_.Close();

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
		// General settings
		config.SeekSection("Settings");
		std::string str;
		if (!config.ReadItem("language", str) || !I18n::GetInstance().LoadFromCode(str))
		{
			I18n::GetInstance().LoadSystemLanguage();
		}

		int record = 0;
		config.ReadItem("arcade_high_score", record);
		entitymanager_.SetArcadeRecord(record);
		config.ReadItem("fullscreen", fullscreen_);

		int top = 1;
		config.ReadItem("panel_on_top", top);
		if (top == 0)
		{
			ControlPanel::GetInstance().SetY(Game::HEIGHT - ControlPanel::HEIGHT);
			entitymanager_.SetY(0);
		}

		// Audio settings
		config.SeekSection("Audio");
		int enabled = 1;
		config.ReadItem("enable_music", enabled);
		SoundSystem::GetInstance().EnableMusic(enabled);

		enabled = 1;
		config.ReadItem("enable_sound", enabled);
		SoundSystem::GetInstance().EnableSound(enabled);

		std::string music;
		if (config.ReadItem("music_name", music))
			SoundSystem::GetInstance().SetMusic(music);
		int volume;
		if (config.ReadItem("music_volume", volume))
			SoundSystem::GetInstance().SetMusicVolume(volume);
		if (config.ReadItem("sound_volume", volume))
			SoundSystem::GetInstance().SetSoundVolume(volume);

		// reading keyboard and joystick bindings
		input_.LoadFromConfig(config);

		// reading story progression
		playersave_.LoadFromConfig(config);
		return true;
	}
	I18n::GetInstance().LoadSystemLanguage();
	return false;
}


void Game::WriteConfig(const char* filename) const
{
	ConfigParser config;

	// General Settings
	config.SeekSection("Settings");
	config.WriteItem("fullscreen", (int) fullscreen_);
	config.WriteItem("panel_on_top", (int) ControlPanel::GetInstance().IsOnTop());
	config.WriteItem("arcade_high_score", entitymanager_.GetArcadeRecord());
	config.WriteItem("language", I18n::GetInstance().GetCurrentCode());

	// Audio settings
	const SoundSystem& snd = SoundSystem::GetInstance();
	config.SeekSection("Audio");
	config.WriteItem("music_name",   snd.GetMusicName());
	config.WriteItem("enable_music", (int) snd.IsMusicEnabled());
	config.WriteItem("music_volume", snd.GetMusicVolume());
	config.WriteItem("enable_sound", (int) snd.IsSoundEnabled());
	config.WriteItem("sound_volume", snd.GetSoundVolume());

	// writing keyboard and joystick bindings
	input_.SaveToConfig(config);

	// levels
	playersave_.SaveToConfig(config);

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
					Quit();
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
			CASE_SCENE(ArcadeMenu, new_scene);
			CASE_SCENE(GameOverMenu, new_scene);
			CASE_SCENE(BestScoresMenu, new_scene);
			CASE_SCENE(PauseMenu, new_scene);
			CASE_SCENE(AboutMenu, new_scene);
			CASE_SCENE(LevelMenu, new_scene);
			CASE_SCENE(ArmoryMenu, new_scene);
			CASE_SCENE(IntroLevelScene, new_scene);
			CASE_SCENE(OptionMenu, new_scene);
			CASE_SCENE(KeyboardMenu, new_scene);
			CASE_SCENE(JoystickMenu, new_scene);
			CASE_SCENE(AudioMenu, new_scene);
			CASE_SCENE(SettingsMenu, new_scene);

			default:
				DIE("can't set next requested scene");
				break;
		}
		scenes_[enum_scene] = new_scene;
	}
	current_scene_ = scenes_[enum_scene];
	current_scene_->OnFocus();
}


void Game::Quit()
{
	running_ = false;
	SoundSystem::GetInstance().StopAll();
	WriteConfig(CONFIG_FILE);
}


void Game::TakeScreenshot(const char* directory)
{
	char current_time[256];
	time_t t = time(NULL);
	strftime(current_time, sizeof current_time, "%d-%m-%Y_%H-%M-%S", localtime(&t));

	std::string filename = std::string(directory) + "/" + current_time + ".png";

	printf("screenshot saved to %s\n", filename.c_str());
	app_.Capture().SaveToFile(filename);
}


void Game::SetFullscreen(bool full)
{
	if (app_.IsOpened())
		app_.Close();

	int style = full ? sf::Style::Fullscreen : sf::Style::Close;
	app_.Create(sf::VideoMode(Game::WIDTH, Game::HEIGHT, WIN_BPP), WIN_TITLE, style);
	app_.UseVerticalSync(true);
	fullscreen_ = full;
}


bool Game::IsFullscreen() const
{
	return fullscreen_;
}


void Game::ReloadScenes()
{
	// delete allocated scenes, except the current one
	for (int i = 0; i < SC_COUNT; ++i)
	{
		if (scenes_[i] != NULL && scenes_[i] != current_scene_)
		{
			delete scenes_[i];
			scenes_[i] = NULL;
		}
	}
}


void Game::CheckPurity()
{
	printf("* checking resources purity... ");
	pure_ = true;
	std::ifstream file;
	MD5 md5sum;

	file.open(XML_ITEMS);
	pure_ &= (md5sum.Calculate(file) == MD5SUM_WEAPONS);
	file.close();

	file.open(XML_SPACESHIPS);
	pure_ &= (md5sum.Calculate(file) == MD5SUM_SPACESHIPS);
	file.close();

	file.open(XML_ANIMATIONS);
	pure_ &= (md5sum.Calculate(file) == MD5SUM_ANIMATIONS);
	file.close();
	puts(pure_ ? "[OK]" : "[FAILED]");
}
