#include "Game.hpp"
#include "Constants.hpp"
#include "SoundSystem.hpp"
#include "LevelManager.hpp"
#include "ControlPanel.hpp"
#include "Resources.hpp"
#include "entities/EntityManager.hpp"
#include "items/ItemManager.hpp"
#include "utils/IniParser.hpp"
#include "utils/StringUtils.hpp"
#include "utils/I18n.hpp"
#include "utils/FileSystem.hpp"
#include "utils/sfml_helper.hpp"
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
	app_.close();

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


void Game::Init(const std::string& data_path)
{
	input_.Init();

	// init resources directory
	data_dir_ = current_dir_ + data_path;
	Resources::setDataPath(data_dir_);
	I18n::GetInstance().SetDataPath(data_dir_);
	screenshot_dir_ = DEFAULT_SCREENSHOT_DIR;

	// create window and display loading screen as early as possible
	vsync_ = true;
	SetFullscreen(false);
	sf::Text temp("loading"); app_.clear() ;app_.draw(temp); app_.display();

	// load XML resources
	try
	{
		printf("* checking resources purity... %49s\n",
			CheckResourcesPurity() ? "[OK]" : "[FAILED]");
		printf("* loading %-59s [%2d found]\n", "levels...",
			LevelManager::GetInstance().ParseFile(data_dir_ + XML_LEVELS));
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

	app_.setFramerateLimit(WIN_FPS);
	app_.setMouseCursorVisible(false);
	app_.setKeyRepeatEnabled(false);
	app_.setVerticalSyncEnabled(vsync_);


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
	app_.display();

	// game main loop which handle the current scene
	sf::Event event;
	Input::Action action;
	sf::Clock clock;
	while (running_)
	{
		// 1. polling events
		while (app_.pollEvent(event))
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
		app_.clear();
		current_scene_->Update(clock.restart().asSeconds());

		// 3. displaying the current scene
		current_scene_->Show(app_);
		app_.display();
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
	app_.capture().saveToFile(filename);
}


void Game::SetFullscreen(bool full)
{
	if (app_.isOpen())
		app_.close();

	int style = full ? sf::Style::Fullscreen : sf::Style::Close;
	app_.create(sf::VideoMode(Game::WIDTH, Game::HEIGHT, WIN_BPP), WIN_TITLE, style);
	app_.setVerticalSyncEnabled(vsync_);
	fullscreen_ = full;
	if (!full)
	{
		// set window icon
		static sf::Image icon = Resources::getTexture("gui/icon.bmp").copyToImage();
		icon.createMaskFromColor(sf::Color(0xff, 0, 0xff));
		app_.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

		// center window on desktop
		sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
		app_.setPosition(sf::Vector2i((desktop.width - WIDTH) / 2, (desktop.height - HEIGHT) / 2));
	}
}


bool Game::IsFullscreen() const
{
	return fullscreen_;
}


void Game::SetVerticalSync(bool vsync)
{
	app_.setVerticalSyncEnabled(vsync);
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
		ControlPanel::GetInstance().setPosition(0, 0);
		EntityManager::GetInstance().setPosition(0, ControlPanel::HEIGHT);
	}
	else
	{
		ControlPanel::GetInstance().setPosition(0, Game::HEIGHT - ControlPanel::HEIGHT);
		EntityManager::GetInstance().setPosition(0, 0);
	}
}


void Game::BSOD(std::string message)
{
	xsf::Text title("COSMIC ERROR");
	title.setCharacterSize(20);
	title.setColor(sf::Color(0, 0, 0x88));
	sf::RectangleShape title_bg(sf::Vector2f(title.getWidth() + 20, title.getHeight() + 20));
	title_bg.setFillColor(sf::Color(0xaa, 0xaa, 0xaa));

	int x = (WIDTH - title.getWidth()) / 2;
	title.setPosition(x, 60);
	title_bg.setPosition(x - 10, 50);

	sf::Text str;
	message += "\n\nPress any key to exit.";
	str.setString(message);
	str.setCharacterSize(20);
	str.setPosition(50, 200);

	while (true)
	{
		sf::Event event;
		while (app_.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed)
			{
				app_.close();
				exit(EXIT_FAILURE);
			}
		}
		app_.clear(sf::Color(0, 0, 0x88));
		app_.draw(title_bg);
		app_.draw(title);
		app_.draw(str);
		app_.display();
	}
}
