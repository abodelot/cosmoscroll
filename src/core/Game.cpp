#include "Game.hpp"
#include "Constants.hpp"
#include "SoundSystem.hpp"
#include "LevelManager.hpp"
#include "ControlPanel.hpp"
#include "entities/EntityManager.hpp"
#include "items/ItemManager.hpp"
#include "utils/Resources.hpp"
#include "utils/ConfigParser.hpp"
#include "utils/StringUtils.hpp"
#include "utils/I18n.hpp"
#include "utils/FileSystem.hpp"
#include "md5/md5.hpp"
#include "scenes/scenes.hpp"
#include "BigScrollingMessagingAppliance.hpp"

#include <fcntl.h>
#include <sys/stat.h>

// config and data files
#define CONFIG_FILENAME   "cosmoscroll.cfg"

#define XML_LEVELS      "/xml/levels.xml"
#define XML_ITEMS       "/xml/items.xml"
#define XML_ANIMATIONS  "/xml/animations.xml"
#define XML_SPACESHIPS  "/xml/spaceships.xml"

// constantes de configuration de la fenêtre
#define WIN_BPP     32
#define WIN_FPS     60
#define WIN_TITLE   "CosmoScroll"


Game& Game::GetInstance()
{
	static Game self;
	return self;
}


Game::Game():
	app_(sf::VideoMode(WIDTH, HEIGHT, WIN_BPP), WIN_TITLE),
	input_(Input::GetInstance())
{
	// center window on desktop
	sf::VideoMode desktop = sf::VideoMode::GetDesktopMode();
	app_.SetPosition((desktop.Width - WIDTH) / 2, (desktop.Height - HEIGHT) / 2);

	// HACK: display loading screen as early as possible
	sf::String temp("loading..."); app_.Clear() ;app_.Draw(temp); app_.Display();

	config_file_ = FileSystem::InitSettingsDirectory(GAME_NAME) + "/" + CONFIG_FILENAME;
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

	Resources::Unload();
}


void Game::SetCurrentDirectory(const std::string& path)
{
	size_t found = path.find_last_of("/\\");
	current_dir_ = path.substr(0, found + 1);
	if (current_dir_.empty())
		current_dir_ = "./";
}


void Game::OverrideConfigFile(const std::string& config_file)
{
	struct stat buf;
	config_file_ = config_file;

	if (!stat(config_file_.c_str(), &buf))
	{
		if (buf.st_mode & S_IFDIR)
			(config_file_ += "/") += CONFIG_FILENAME;
		else if (!buf.st_mode & S_IFREG)		// wtf is this file?
			puts("Error: configuration file is nor a regular file nor a directory");
	}
	else
		puts("Error: couldn't stat configuration file");

}


void Game::Init(const std::string& data_path, int level_set)
{

	input_.Init(app_.GetInput());

	// init resources directory
	data_dir_ = current_dir_ + data_path;
	Resources::SetDataPath(data_dir_);
	I18n::GetInstance().SetDataPath(data_dir_);

	CheckPurity();

	// load XML resources
	LevelManager::GetInstance().ParseFile(data_dir_ + XML_LEVELS, level_set);
	ItemManager::GetInstance().LoadItems(data_dir_ + XML_ITEMS);

	EntityManager& entities = EntityManager::GetInstance();
	entities.LoadAnimations(data_dir_ + XML_ANIMATIONS);
	entities.LoadSpaceShips(data_dir_ + XML_SPACESHIPS);
	entities.SetY(ControlPanel::HEIGHT);

	// load config
#ifdef DEBUG
	printf("[config] configuration file is: %s\n", config_file_.c_str());
#endif
	LoadConfig(config_file_);


	SetFullscreen(fullscreen_);
	app_.SetFramerateLimit(WIN_FPS);
	app_.ShowMouseCursor(false);
	app_.EnableKeyRepeat(false);
	app_.UseVerticalSync(vsync_);

	// scenes will be allocated only if requested
	for (int i = 0; i < SC_COUNT; ++i)
	{
		scenes_[i] = NULL;
	}
	current_scene_ = NULL;
	running_ = true;
}


bool Game::LoadConfig(const std::string& filename)
{
	ConfigParser config;
	fullscreen_ = false;
	vsync_ = true;

	screenshot_dir_ = DEFAULT_SCREENSHOT_DIR;

	if (config.LoadFromFile(filename.c_str()))
	{
		// Directories
		config.SeekSection("Directories");
		config.ReadItem("screenshots", screenshot_dir_);
		// Todo, screenshot directory
		// General settings
		config.SeekSection("Settings");
		std::string str;
		if (!config.ReadItem("language", str) || !I18n::GetInstance().LoadFromCode(str))
		{
			I18n::GetInstance().LoadSystemLanguage();
		}

		int record = 0;
		config.ReadItem("arcade_high_score", record);
		EntityManager::GetInstance().SetArcadeRecord(record);
		config.ReadItem("fullscreen", fullscreen_);
		config.ReadItem("vsync", vsync_);

		int top = 1;
		config.ReadItem("panel_on_top", top);
		PanelOnTop(top);

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


void Game::WriteConfig(const std::string& filename) const
{
	ConfigParser config;

	// Directories
	config.SeekSection("Directories");
	config.WriteItem("screenshots", screenshot_dir_);
	// General Settings
	config.SeekSection("Settings");
	config.WriteItem("fullscreen", (int) fullscreen_);
	config.WriteItem("vsync", (int) vsync_);
	config.WriteItem("panel_on_top", (int) ControlPanel::GetInstance().IsOnTop());
	config.WriteItem("arcade_high_score", EntityManager::GetInstance().GetArcadeRecord());
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

	// Player data
	playersave_.SaveToConfig(config);

	// saving configuration to file
	config.SaveToFile(filename.c_str());
}


int Game::Run()
{
	// set the first displayed scene at launch
	SetNextScene(SC_IntroScene);
	app_.Display();

	// <HACK>
	BSMA *screen_ = &BSMA::GetInstance();
	static bool to = false;
	// </HACK>

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
					TakeScreenshot();
					break;
				case Input::DEBUG_ACTION:
					to = true;
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

		// <HACK>
		if (to)
		{
			BSMA::text_message t;
			t.str = sf::Unicode::Text(L"Téstàge xD xD   ...:<3:<3:<3:<3:<3:<3:<3:...");
			t.delay = 1.f;
			screen_->Push(t);
			to = false;
		}
		screen_->Update(app_.GetFrameTime());
		// </HACK>
		// 3. displaying the current scene
		current_scene_->Show(app_);
		// <HACK>
		screen_->Show(app_);
		app_.Display();
	}
	return (EXIT_SUCCESS);
}


sf::RenderWindow& Game::GetApp()
{
	return app_;
}


void Game::SetNextScene(Scene enum_scene)
{
#define ALLOC_SCENE() \
    BaseScene *new_scene = NULL;
#define CASE_SCENE(__scene__) \
	case Game::SC_ ## __scene__:\
		new_scene = new __scene__();\
		break

	if (scenes_[enum_scene] == NULL)
	{
		ALLOC_SCENE();
		switch (enum_scene)
		{
			CASE_SCENE(IntroScene);
			CASE_SCENE(MainMenu);
			CASE_SCENE(InGameScene);
			CASE_SCENE(EndGameScene);
			CASE_SCENE(ArcadeMenu);
			CASE_SCENE(GameOverMenu);
			CASE_SCENE(BestScoresMenu);
			CASE_SCENE(PauseMenu);
			CASE_SCENE(AboutMenu);
			CASE_SCENE(LevelMenu);
			CASE_SCENE(ArmoryMenu);
			CASE_SCENE(IntroLevelScene);
			CASE_SCENE(OptionMenu);
			CASE_SCENE(KeyboardMenu);
			CASE_SCENE(JoystickMenu);
			CASE_SCENE(AudioMenu);
			CASE_SCENE(SettingsMenu);

			default:
				return;
		}
		scenes_[enum_scene] = new_scene;
	}
	current_scene_type_ = enum_scene;
	current_scene_ = scenes_[enum_scene];
	current_scene_->OnFocus();
}


void Game::Quit()
{
	running_ = false;
	SoundSystem::GetInstance().StopAll();
	WriteConfig(config_file_);
}

//Todo verifier si screenshotdir est valide
void Game::TakeScreenshot(void)
{
	char current_time[256];
	time_t t = time(NULL);
	strftime(current_time, sizeof current_time, "%d-%m-%Y_%H-%M-%S", localtime(&t));

	std::string filename = screenshot_dir_ + "/" + current_time + ".png";

	printf("screenshot saved to %s\n", filename.c_str());
	app_.Capture().SaveToFile(filename);
}


void Game::SetFullscreen(bool full)
{
	if (app_.IsOpened())
		app_.Close();

	int style = full ? sf::Style::Fullscreen : sf::Style::Close;
	app_.Create(sf::VideoMode(Game::WIDTH, Game::HEIGHT, WIN_BPP), WIN_TITLE, style);
	app_.UseVerticalSync(vsync_);
	fullscreen_ = full;

	if (!full)
	{
		// set window icon
		sf::Image& icon = Resources::GetImage("gui/icon.bmp");
		icon.CreateMaskFromColor(sf::Color(0xff, 0, 0xff));
		app_.SetIcon(icon.GetWidth(), icon.GetHeight(), icon.GetPixelsPtr());
	}
}


bool Game::IsFullscreen() const
{
	return fullscreen_;
}


void Game::SetVerticalSync(bool vsync)
{
	app_.UseVerticalSync(vsync);
	vsync_ = vsync;
}


bool Game::IsVerticalSync() const
{
	return vsync_;
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

	file.open((data_dir_ + XML_ITEMS).c_str());
	pure_ &= (md5sum.Calculate(file) == MD5SUM_ITEMS);
	file.close();

	file.open((data_dir_ + XML_SPACESHIPS).c_str());
	pure_ &= (md5sum.Calculate(file) == MD5SUM_SPACESHIPS);
	file.close();

	file.open((data_dir_ + XML_ANIMATIONS).c_str());
	pure_ &= (md5sum.Calculate(file) == MD5SUM_ANIMATIONS);
	file.close();
	puts(pure_ ? "[OK]" : "[FAILED]");
}


void Game::PanelOnTop(bool top)
{
	if (top)
	{
		ControlPanel::GetInstance().SetY(0);
		EntityManager::GetInstance().SetY(ControlPanel::HEIGHT);
	}
	else
	{
		ControlPanel::GetInstance().SetY(Game::HEIGHT - ControlPanel::HEIGHT);
		EntityManager::GetInstance().SetY(0);
	}
}
