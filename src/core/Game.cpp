#include "Game.hpp"
#include "Constants.hpp"
#include "PlayerSave.hpp"
#include "SoundSystem.hpp"
#include "LevelManager.hpp"
#include "ControlPanel.hpp"
#include "Resources.hpp"
#include "MessageSystem.hpp"
#include "entities/EntityManager.hpp"
#include "items/ItemManager.hpp"
#include "utils/IniParser.hpp"
#include "utils/I18n.hpp"
#include "utils/FileSystem.hpp"
#include "utils/Error.hpp"
#include "utils/md5/md5.hpp"
#include "scenes/scenes.hpp"

// config and data files
#define CONFIG_FILENAME   "cosmoscroll.ini"

#define XML_LEVELS      "/xml/levels.xml"
#define XML_ITEMS       "/xml/items.xml"
#define XML_ANIMATIONS  "/xml/animations.xml"
#define XML_SPACESHIPS  "/xml/spaceships.xml"


Game& Game::getInstance()
{
	static Game self;
	return self;
}


Game::Game():
	m_input(Input::GetInstance())
{
	// scenes will be allocated only if requested
	for (int i = 0; i < SC_COUNT; ++i)
	{
		m_scenes[i] = NULL;
	}
	m_current_scene = NULL;
	m_running = true;

	// default config location
	m_config_file = FileSystem::initSettingsDirectory(GAME_NAME) + "/" + CONFIG_FILENAME;
}


Game::~Game()
{
	m_window.close();

	// delete allocated scenes
	for (int i = 0; i < SC_COUNT; ++i)
	{
		if (m_scenes[i] != NULL)
		{
			delete m_scenes[i];
		}
	}
}


void Game::init(const std::string& path)
{
	size_t found = path.find_last_of("/\\");
	m_app_dir = path.substr(0, found + 1);
	if (m_app_dir.empty())
		m_app_dir = "./";
}


void Game::setConfigFile(const std::string& config_file)
{
	m_config_file = config_file;
	if (FileSystem::isDirectory(config_file))
	{
		m_config_file = m_config_file + "/" + CONFIG_FILENAME;
	}
	else if (!FileSystem::isFile(config_file))
	{
		std::cerr << "Error: configuration file is nor a regular file nor a directory" << std::endl;
	}
}


void Game::loadResources(const std::string& data_path)
{
	m_input.Init();

	// init resources directory
	std::string resources_dir = m_app_dir + data_path;
	Resources::setDataPath(resources_dir);
	I18n::getInstance().setDataPath(resources_dir + "/lang");
	m_screenshots_dir = DEFAULT_SCREENSHOT_DIR;
	MessageSystem::setFont(Resources::getFont("Ubuntu-R.ttf"));

	// create window and display loading screen as early as possible
	m_vsync = true;
	setFullscreen(false);

	// load XML resources
	try
	{
		printf("* checking resources md5sum...\n");
		m_resources_checked = checkResourcesPurity(resources_dir);
		printf("    test %s\n", m_resources_checked ? "succeeded" : "failed");
		printf("* loading %s...\n", XML_LEVELS);
		printf("    %d levels found\n", LevelManager::GetInstance().ParseFile(resources_dir + XML_LEVELS));
		printf("* loading %s...\n", XML_ITEMS);
		printf("    %d items found\n", ItemManager::GetInstance().LoadItems(resources_dir + XML_ITEMS));
		printf("* loading %s...\n", XML_ANIMATIONS);
		printf("    %d animations found\n", EntityManager::getInstance().LoadAnimations(resources_dir + XML_ANIMATIONS));
		printf("* loading %s\n", XML_SPACESHIPS);
		printf("    %d spaceships found\n", EntityManager::getInstance().LoadSpaceShips(resources_dir + XML_SPACESHIPS));
	}
	catch (std::runtime_error& error)
	{
		std::cerr << error.what() << std::endl;
		Error::dump();
		quit();
	}
	// load config
#ifdef DEBUG
	printf("configuration file is: %s\n", m_config_file.c_str());
#endif
	loadConfig(m_config_file);

	m_window.setFramerateLimit(APP_FPS);
	m_window.setMouseCursorVisible(false);
	m_window.setKeyRepeatEnabled(false);
	m_window.setVerticalSyncEnabled(m_vsync);
}


bool Game::loadConfig(const std::string& filename)
{
	IniParser config;
	m_fullscreen = false;
	m_vsync = true;

	if (config.LoadFromFile(filename.c_str()))
	{
		// -- General settings --
		config.SeekSection("Settings");
		// language
		std::string lang = config.Get("language");
		if (lang.empty() || !I18n::getInstance().loadFromCode(lang))
		{
			I18n::getInstance().loadSystemLanguage();
		}

		// fullscreen & vsync
		config.Get("vsync", m_vsync);
		config.Get("fullscreen", m_fullscreen);
		if (m_fullscreen)
			setFullscreen(m_fullscreen);

		// panel pop
		bool top = true;
		config.Get("panel_on_top", top);
		panelOnTop(top);

		// screenshot directory
		config.Get("screenshots", m_screenshots_dir);

		// -- Audio settings --
		SoundSystem::GetInstance().LoadFromConfig(config);

		// -- Keyboard and joystick bindings --
		m_input.LoadFromConfig(config);

		// -- Player progression and highscore --
		PlayerSave::loadFromConfig(config);
		return true;
	}
	I18n::getInstance().loadSystemLanguage();
	return false;
}


void Game::writeConfig(const std::string& filename) const
{
	IniParser config;

	// General Settings
	config.SeekSection("Settings");
	config.Set("fullscreen", m_fullscreen);
	config.Set("vsync", m_vsync);
	config.Set("panel_on_top", ControlPanel::GetInstance().IsOnTop());
	config.Set("language", I18n::getInstance().getLangCode());
	config.Set("screenshots", m_screenshots_dir);

	// Audio settings
	SoundSystem::GetInstance().SaveToConfig(config);

	// Keyboard and joystick bindings
	m_input.SaveToConfig(config);

	// Player data
	PlayerSave::saveToConfig(config);

	// saving configuration to file
	config.SaveToFile(filename.c_str());
}


int Game::run()
{
	// set the first displayed scene at launch
	setNextScene(SC_IntroScene);
	m_window.display();

	// game main loop which handle the current scene
	sf::Event event;
	Input::Action action;
	sf::Clock clock;
	while (m_running)
	{
		// 1. polling events
		while (m_window.pollEvent(event))
		{
			action = m_input.EventToAction(event);
			switch (action)
			{
				// these events are always handled on each scene
				case Input::EXIT_APP:
					quit();
					break;
				case Input::TAKE_SCREENSHOT:
					takeScreenshot();
					break;
				// other events are send to the current scene
				default:
					m_current_scene->OnEvent(event);
					break;
			}
		}
		// 2. updating the current scene
		m_window.clear();
		m_current_scene->Update(clock.restart().asSeconds());

		// 3. displaying the current scene
		m_current_scene->Show(m_window);
		m_window.display();
	}
	return EXIT_SUCCESS;
}


sf::RenderWindow& Game::getWindow()
{
	return m_window;
}


void Game::setNextScene(Scene enum_scene)
{
#define CASE_SCENE(__scene__) \
	case Game::SC_ ## __scene__:\
		new_scene = new __scene__();\
		break

	if (m_scenes[enum_scene] == NULL)
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
		m_scenes[enum_scene] = new_scene;

	}
	m_current_scene = m_scenes[enum_scene];
	m_current_scene->OnFocus();
}


void Game::quit()
{
	m_running = false;
	SoundSystem::GetInstance().StopAll();
	writeConfig(m_config_file);
}


void Game::takeScreenshot()
{
	// Create screenshots directory if it doesn't exist yet
	if (!FileSystem::isDirectory(m_screenshots_dir))
		FileSystem::createDirectory(m_screenshots_dir);

	char current_time[20]; // YYYY-MM-DD_HH-MM-SS + \0
	time_t t = time(NULL);
	strftime(current_time, sizeof current_time, "%Y-%m-%d_%H-%M-%S", localtime(&t));
	std::string filename = m_screenshots_dir + "/" + current_time + ".png";

	if (m_window.capture().saveToFile(filename))
		std::cout << "screenshot saved to " << filename << std::endl;
}


void Game::setFullscreen(bool full)
{
	if (m_window.isOpen())
		m_window.close();

	int style = full ? sf::Style::Fullscreen : sf::Style::Close;
	m_window.create(sf::VideoMode(APP_WIDTH, APP_HEIGHT), APP_TITLE, style);
	m_window.setVerticalSyncEnabled(m_vsync);
	m_fullscreen = full;
	if (!full)
	{
		// set window icon
		static sf::Image icon = Resources::getTexture("gui/icon.bmp").copyToImage();
		icon.createMaskFromColor(sf::Color(0xff, 0, 0xff));
		m_window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

		// center window on desktop
		sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
		m_window.setPosition(sf::Vector2i((desktop.width - APP_WIDTH) / 2, (desktop.height - APP_HEIGHT) / 2));
	}
}


bool Game::isFullscreen() const
{
	return m_fullscreen;
}


void Game::setVerticalSync(bool vsync)
{
	m_window.setVerticalSyncEnabled(vsync);
	m_vsync = vsync;
}


bool Game::isVerticalSync() const
{
	return m_vsync;
}


void Game::reloadScenes()
{
	// delete allocated scenes, except the current one
	for (int i = 0; i < SC_COUNT; ++i)
	{
		if (m_scenes[i] != NULL && m_scenes[i] != m_current_scene)
		{
			delete m_scenes[i];
			m_scenes[i] = NULL;
		}
	}
}


bool Game::checkResourcesPurity(const std::string& resources_dir)
{
	bool check_passed = true;
	std::ifstream file;
	MD5 md5sum;

	file.open((resources_dir + XML_ITEMS).c_str());
	check_passed &= (md5sum.Calculate(file) == MD5SUM_ITEMS);
	file.close();

	file.open((resources_dir + XML_SPACESHIPS).c_str());
	check_passed &= (md5sum.Calculate(file) == MD5SUM_SPACESHIPS);
	file.close();

	file.open((resources_dir + XML_ANIMATIONS).c_str());
	check_passed &= (md5sum.Calculate(file) == MD5SUM_ANIMATIONS);
	file.close();
	return check_passed;
}


bool Game::resourcesChecked() const
{
	return m_resources_checked;
}


void Game::panelOnTop(bool top)
{
	if (top)
	{
		ControlPanel::GetInstance().setPosition(0, 0);
		EntityManager::getInstance().setPosition(0, ControlPanel::HEIGHT);
	}
	else
	{
		ControlPanel::GetInstance().setPosition(0, APP_HEIGHT - ControlPanel::HEIGHT);
		EntityManager::getInstance().setPosition(0, 0);
	}
}

