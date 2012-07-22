#include "Game.hpp"
#include "Constants.hpp"
#include "SoundSystem.hpp"
#include "LevelManager.hpp"
#include "ControlPanel.hpp"
#include "entities/EntityManager.hpp"
#include "items/ItemManager.hpp"
#include "utils/Resources.hpp"
#include "utils/IniParser.hpp"
#include "utils/StringUtils.hpp"
#include "utils/I18n.hpp"
#include "utils/FileSystem.hpp"
#include "md5/md5.hpp"
#include "scenes/scenes.hpp"

#include <stdexcept>
#include <sys/stat.h>

// config and data files
#define CONFIG_FILENAME   "cosmoscroll.ini"

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
	input_(Input::GetInstance())
{
	// default location
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
	screenshot_dir_ = DEFAULT_SCREENSHOT_DIR;

	// create window and display loading screen as early as possible
	vsync_ = true;
	SetFullscreen(false);
	sf::String temp("loading"); app_.Clear() ;app_.Draw(temp); app_.Display();

	// load XML resources
	try
	{
		printf("* checking resources purity... %49s\n",
			CheckResourcesPurity() ? "[OK]" : "[FAILED]");
		printf("* loading %-59s [%2d found]\n", "levels...",
			LevelManager::GetInstance().ParseFile(data_dir_ + XML_LEVELS, level_set));
		printf("* loading %-59s [%2d found]\n", "items...",
			ItemManager::GetInstance().LoadItems(data_dir_ + XML_ITEMS));
		printf("* loading %-59s [%2d found]\n", "animations...",
			EntityManager::GetInstance().LoadAnimations(data_dir_ + XML_ANIMATIONS));
		printf("* loading %-59s [%2d found]\n", "spaceships definitions...",
			EntityManager::GetInstance().LoadSpaceShips(data_dir_ + XML_SPACESHIPS));
	}
	catch (std::runtime_error& error)
	{
		BSOD(error.what());
	}
	// load config
#ifdef DEBUG
	printf("configuration file is: %s\n", config_file_.c_str());
#endif
	LoadConfig(config_file_);

	app_.UseVerticalSync(vsync_);
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
	IniParser config;
	fullscreen_ = false;
	vsync_ = true;

	if (config.LoadFromFile(filename.c_str()))
	{
		// -- General settings --
		config.SeekSection("Settings");
		// language
		std::string lang = config.Get("language");
		if (lang.empty() || !I18n::GetInstance().LoadFromCode(lang))
		{
			I18n::GetInstance().LoadSystemLanguage();
		}

		// high-score
		int high_score = 0;
		config.Get("arcade_high_score", high_score);
		EntityManager::GetInstance().SetArcadeRecord(high_score);

		// fullscreen & vsync
		config.Get("vsync", vsync_);
		config.Get("fullscreen", fullscreen_);
		if (fullscreen_)
			SetFullscreen(fullscreen_);

		// panel pop
		bool top = true;
		config.Get("panel_on_top", top);
		PanelOnTop(top);

		// screenshot directory
		config.Get("screenshots", screenshot_dir_);

		// -- Audio settings --
		SoundSystem::GetInstance().LoadFromConfig(config);

		// -- Keyboard and joystick bindings --
		input_.LoadFromConfig(config);

		// -- Story mode progression --
		playersave_.LoadFromConfig(config);
		return true;
	}
	I18n::GetInstance().LoadSystemLanguage();
	return false;
}


void Game::WriteConfig(const std::string& filename) const
{
	IniParser config;

	// General Settings
	config.SeekSection("Settings");
	config.Set("fullscreen", fullscreen_);
	config.Set("vsync", vsync_);
	config.Set("panel_on_top", ControlPanel::GetInstance().IsOnTop());
	config.Set("arcade_high_score", EntityManager::GetInstance().GetArcadeRecord());
	config.Set("language", I18n::GetInstance().GetCurrentCode());
	config.Set("screenshots", screenshot_dir_);

	// Audio settings
	SoundSystem::GetInstance().SaveToConfig(config);

	// Keyboard and joystick bindings
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
				// other events are send to the current scene
				default:
					current_scene_->OnEvent(event);
					break;
			}
		}
		// 2. updating the current scene
		app_.Clear();
		current_scene_->Update(app_.GetFrameTime());

		// 3. displaying the current scene
		current_scene_->Show(app_);
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
#define CASE_SCENE(__scene__) \
	case Game::SC_ ## __scene__:\
		new_scene = new __scene__();\
		break

	if (scenes_[enum_scene] == NULL)
	{
		BaseScene *new_scene = NULL;
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

	std::string filename = current_dir_ + screenshot_dir_ + "/" + current_time + ".png";
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

		// center window on desktop
		sf::VideoMode desktop = sf::VideoMode::GetDesktopMode();
		app_.SetPosition((desktop.Width - WIDTH) / 2, (desktop.Height - HEIGHT) / 2);
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


bool Game::CheckResourcesPurity()
{
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
	return pure_;
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


void Game::BSOD(std::string message)
{
	sf::String title("COSMIC ERROR");
	title.SetSize(20);
	title.SetColor(sf::Color(0, 0, 0x88));
	sf::Shape title_bg = sf::Shape::Rectangle(-10, -10, title.GetRect().GetWidth() + 10, title.GetRect().GetHeight() + 10, sf::Color(0xaa, 0xaa, 0xaa));

	int x = (WIDTH - title.GetRect().GetWidth()) / 2;
	title.SetPosition(x, 60);
	title_bg.SetPosition(x, 60);

	sf::String str;
	message += "\n\nPress any key to exit.";
	str.SetText(message);
	str.SetSize(20);
	str.SetPosition(50, 200);

	while (true)
	{
		sf::Event event;
		while (app_.GetEvent(event))
		{
			if (event.Type == sf::Event::KeyPressed)
			{
				app_.Close();
				exit(EXIT_FAILURE);
			}
		}
		app_.Clear(sf::Color(0, 0, 0x88));
		app_.Draw(title_bg);
		app_.Draw(title);
		app_.Draw(str);
		app_.Display();
	}
}
