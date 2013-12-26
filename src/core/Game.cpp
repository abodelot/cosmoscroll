#include "Game.hpp"
#include "Constants.hpp"
#include "UserSettings.hpp"
#include "SoundSystem.hpp"
#include "LevelManager.hpp"
#include "Resources.hpp"
#include "MessageSystem.hpp"
#include "entities/EntityManager.hpp"
#include "items/ItemManager.hpp"
#include "utils/I18n.hpp"
#include "utils/IniParser.hpp"
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
	m_fullscreen(false),
	m_vsync(true),
	m_running(true)
{
	// Scenes will be allocated only if requested
	for (int i = 0; i < SC_COUNT; ++i)
	{
		m_scenes[i] = NULL;
	}
	m_current_scene = NULL;

	// Default configuration file location
	m_config_file = FileSystem::initSettingsDirectory(GAME_NAME) + "/" + CONFIG_FILENAME;
}


Game::~Game()
{
	m_window.close();

	// Delete allocated scenes
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


void Game::loadResources(const std::string& data_path)
{
	// Init resources directory
	std::string resources_dir = m_app_dir + data_path;
	Resources::setDataPath(resources_dir);

	// Splash screen
	sf::Sprite s(Resources::getTexture("gui/cosmoscroll-logo.png"));
	m_window.create(sf::VideoMode(s.getTextureRect().width, s.getTextureRect().height), APP_TITLE, sf::Style::None);
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	m_window.setPosition({(desktop.width - m_window.getSize().x) / 2,
	                      (desktop.height - m_window.getSize().y) / 2});
	m_window.draw(s);
	m_window.display();

	// Init other modules
	I18n::getInstance().setDataPath(resources_dir + "/lang");
	MessageSystem::setFont(Resources::getFont("Vera.ttf"));

	// Load XML resources
	try
	{
		printf("* checking resources md5sum...");
		m_resources_checked = checkResourcesPurity(resources_dir);
		printf("\t[test %s]\n", m_resources_checked ? "succeeded" : "failed");
		printf("* loading %s...\n", XML_LEVELS);
		LevelManager::getInstance().loadLevelFile(resources_dir + XML_LEVELS);
		printf("* loading %s...\n", XML_ITEMS);
		ItemManager::GetInstance().LoadItems(resources_dir + XML_ITEMS);
		printf("* loading %s...\n", XML_ANIMATIONS);
		EntityManager::getInstance().loadAnimations(resources_dir + XML_ANIMATIONS);
		printf("* loading %s...\n", XML_SPACESHIPS);
		EntityManager::getInstance().loadSpaceships(resources_dir + XML_SPACESHIPS);
	}
	catch (std::runtime_error& error)
	{
		std::cerr << error.what() << std::endl;
		Error::dump();
		quit();
	}
}


void Game::setConfigFile(const std::string& config_path)
{
	if (FileSystem::isFile(config_path))
		m_config_file = config_path;

	else if (FileSystem::isDirectory(config_path))
		m_config_file = config_path + "/" + CONFIG_FILENAME;
}


bool Game::loadConfig()
{
	IniParser config;
	if (config.LoadFromFile(m_config_file.c_str()))
	{
		std::cout << "* loading configuration from " << m_config_file << std::endl;
		// Window
		config.SeekSection("Window");
		config.Get("vsync", m_vsync);
		config.Get("fullscreen", m_fullscreen);

		// User settings & player attributes
		UserSettings::loadFromConfig(config);

		// Audio settings
		SoundSystem::GetInstance().LoadFromConfig(config);
		return true;
	}
	std::cout << "no configuration loaded, using default settings" << std::endl;
	I18n::getInstance().loadFromLocale();
	return false;
}


void Game::writeConfig() const
{
	IniParser config;

	// Window
	config.SeekSection("Window");
	config.Set("fullscreen", m_fullscreen);
	config.Set("vsync", m_vsync);

	// User settings & player attributes
	UserSettings::saveToConfig(config);

	// Audio settings
	SoundSystem::GetInstance().SaveToConfig(config);

	// Save configuration to file
	if (config.SaveToFile(m_config_file))
		std::cout << "* configuration saved to " << m_config_file << std::endl;
}


int Game::run()
{
	createWindow();

	// Set the first displayed scene at launch
	setNextScene(SC_IntroScene);

	sf::Clock clock;
	while (m_running)
	{
		// Poll events
		sf::Event event;
		while (m_window.pollEvent(event))
		{
			Action::ID action = Input::feedEvent(event);
			switch (action)
			{
				// These events are always handled on each scene
				case Action::EXIT_APP:
					quit();
					break;
				case Action::TAKE_SCREENSHOT:
					takeScreenshot();
					break;
				// Other events are send to the current scene
				default:
					m_current_scene->OnEvent(event);
					break;
			}
		}
		// Update the current scene
		m_window.clear();
		m_current_scene->Update(clock.restart().asSeconds());

		// Display the current scene
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
	writeConfig();
}


void Game::takeScreenshot() const
{
	// Create screenshots directory if it doesn't exist yet
	std::string screenshot_dir = m_app_dir + DEFAULT_SCREENSHOT_DIR;
	if (!FileSystem::isDirectory(screenshot_dir))
		FileSystem::createDirectory(screenshot_dir);

	char current_time[20]; // YYYY-MM-DD_HH-MM-SS + \0
	time_t t = time(NULL);
	strftime(current_time, sizeof current_time, "%Y-%m-%d_%H-%M-%S", localtime(&t));
	std::string filename = screenshot_dir + "/" + current_time + ".png";

	if (m_window.capture().saveToFile(filename))
		std::cout << "screenshot saved to " << filename << std::endl;
}


void Game::createWindow()
{
	if (m_window.isOpen())
		m_window.close();

	int style = m_fullscreen ? sf::Style::Fullscreen : sf::Style::Close;
	m_window.create(sf::VideoMode(APP_WIDTH, APP_HEIGHT), APP_TITLE, style);
	m_window.setKeyRepeatEnabled(false);

	if (m_vsync)
		m_window.setVerticalSyncEnabled(m_vsync);
	else
		m_window.setFramerateLimit(APP_FPS);

	if (!m_fullscreen)
	{
		// Center window on desktop
		sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
		m_window.setPosition(sf::Vector2i((desktop.width - APP_WIDTH) / 2, (desktop.height - APP_HEIGHT) / 2));

		// Set window app icon
		static sf::Image icon = Resources::getTexture("gui/icon.bmp").copyToImage();
		icon.createMaskFromColor(sf::Color(0xff, 0, 0xff));
		m_window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	}
}


void Game::setFullscreen(bool fullscreen)
{
	m_fullscreen = fullscreen;
	createWindow();
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
	// Delete all allocated scenes but the current one
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
