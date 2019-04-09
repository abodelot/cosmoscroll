#include <iostream>
#include "vendor/md5/md5.hpp"

#include "Game.hpp"
#include "Constants.hpp"
#include "UserSettings.hpp"
#include "LevelManager.hpp"
#include "Resources.hpp"
#include "SoundSystem.hpp"
#include "MessageSystem.hpp"
#include "entities/EntityManager.hpp"
#include "items/ItemManager.hpp"
#include "utils/I18n.hpp"
#include "utils/IniParser.hpp"
#include "utils/FileSystem.hpp"
#include "scenes/scenes.hpp"

// config and data files
#define CONFIG_FILENAME   "cosmoscroll.ini"

#define XML_LEVELS      "/xml/levels.xml"
#define XML_UPGRADES    "/xml/upgrades.xml"
#define XML_WEAPONS     "/xml/weapons.xml"
#define XML_ANIMATIONS  "/xml/animations.xml"
#define XML_SPACESHIPS  "/xml/spaceships.xml"


Game& Game::getInstance()
{
    static Game self;
    return self;
}


Game::Game():
    m_vsync(false),
    m_running(true),
    m_current_screen(NULL)
{
    // Screens will be allocated on the fly
    for (int i = 0; i < SC_COUNT; ++i)
    {
        m_screens[i] = NULL;
    }

    // Default configuration file location
    m_config_filename = FileSystem::initSettingsDirectory(COSMOSCROLL_DIRECTORY) + "/" + CONFIG_FILENAME;
}


Game::~Game()
{
    m_window.close();

    // Delete allocated screens
    for (int i = 0; i < SC_COUNT; ++i)
    {
        if (m_screens[i] != NULL)
        {
            delete m_screens[i];
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
    Resources::setSearchPath(resources_dir);

    // Splash screen
    setResolution(sf::Vector2u(APP_WIDTH, APP_HEIGHT));
    sf::Sprite s(Resources::getTexture("gui/cosmoscroll-logo.png"));
    s.setPosition({(APP_WIDTH - s.getTextureRect().width) / 2.f,
                   (APP_HEIGHT - s.getTextureRect().height) / 2.f});
    m_window.draw(s);
    m_window.display();

    // Init other modules
    I18n::getInstance().setDataPath(resources_dir + "/lang");
    MessageSystem::setFont(Resources::getFont("Vera.ttf"));

    // Check against MD5
    m_resources_checked = checkResourcesPurity(resources_dir);
    if (!m_resources_checked)
        std::cerr << "* warning: MD5 checksum failed" << std::endl;

    // Load XML resources
    std::cout << "* loading " << XML_LEVELS << "..." << std::endl;
    LevelManager::getInstance().loadLevelFile(resources_dir + XML_LEVELS);

    std::cout << "* loading " << XML_UPGRADES << "..." << std::endl;
    ItemManager::getInstance().loadFromXML(resources_dir + XML_UPGRADES);

    std::cout << "* loading " << XML_WEAPONS << "..." << std::endl;
    EntityManager::getInstance().loadWeapons(resources_dir + XML_WEAPONS);

    std::cout << "* loading " << XML_ANIMATIONS << "..." << std::endl;
    EntityManager::getInstance().loadAnimations(resources_dir + XML_ANIMATIONS);

    std::cout << "* loading " << XML_SPACESHIPS << "..." << std::endl;
    EntityManager::getInstance().loadSpaceships(resources_dir + XML_SPACESHIPS);
}


void Game::setConfigFile(const std::string& config_path)
{
    if (FileSystem::isDirectory(config_path))
        m_config_filename = config_path + "/" + CONFIG_FILENAME;

    else
        m_config_filename = config_path;
}


bool Game::loadConfig()
{
    IniParser config;
    if (config.load(m_config_filename))
    {
        std::cout << "* loading configuration from " << m_config_filename << std::endl;
        config.seekSection("Window");

        // Window resolution
        sf::Vector2u size;
        size.x = config.get("width", APP_WIDTH);
        size.y = config.get("height", APP_HEIGHT);
        setResolution(size);

        // Vertical sync
        m_vsync = config.get("vsync", m_vsync);
        if (m_vsync)
            m_window.setVerticalSyncEnabled(m_vsync);

        // Load user settings and player progression
        UserSettings::loadFromConfig(config);
        return true;
    }
    std::cout << "Couldn't load configuration file '" << m_config_filename << "', using default settings" << std::endl;
    I18n::getInstance().loadFromLocale();
    return false;
}


void Game::writeConfig() const
{
    IniParser config;

    // Window
    config.seekSection("Window");
    config.set("width", m_window.getSize().x);
    config.set("height", m_window.getSize().y);
    config.set("vsync", m_vsync);

    // Save user settings and player progression
    UserSettings::saveToConfig(config);

    // Save configuration to file
    if (config.save(m_config_filename))
        std::cout << "* configuration saved to " << m_config_filename << std::endl;
}


int Game::run()
{
    // Set the first displayed scene at launch
    setCurrentScreen(SC_IntroScreen);

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
                // These events are always handled on each screen
                case Action::EXIT_APP:
                    quit();
                    break;
                case Action::TAKE_SCREENSHOT:
                    takeScreenshot();
                    break;
                // Other events are send to the current screen
                default:
                    m_current_screen->onEvent(event);
                    break;
            }
        }
        // Update the current scene
        m_window.clear();
        m_current_screen->update(clock.restart().asSeconds());

        // Display the current scene
        m_current_screen->draw(m_window);
        m_window.display();
    }
    return EXIT_SUCCESS;
}


sf::RenderWindow& Game::getWindow()
{
    return m_window;
}


void Game::setCurrentScreen(ScreenID screen_id)
{
#define CASE_SCENE(__screenclass__) \
    case Game::SC_ ## __screenclass__:\
        new_sreen = new __screenclass__();\
        break

    if (m_screens[screen_id] == NULL)
    {
        Screen* new_sreen = NULL;
        switch (screen_id)
        {
            CASE_SCENE(IntroScreen);
            CASE_SCENE(MainMenu);
            CASE_SCENE(PlayScreen);
            CASE_SCENE(GameOverScreen);
            CASE_SCENE(InfinityModeMenu);
            CASE_SCENE(SendScoreMenu);
            CASE_SCENE(LeaderboardMenu);
            CASE_SCENE(PauseMenu);
            CASE_SCENE(AboutMenu);
            CASE_SCENE(LevelMenu);
            CASE_SCENE(ArmoryMenu);
            CASE_SCENE(OptionMenu);
            CASE_SCENE(KeyboardMenu);
            CASE_SCENE(JoystickMenu);
            CASE_SCENE(AudioMenu);
            CASE_SCENE(SettingsMenu);

            default:
                return;
        }
        m_screens[screen_id] = new_sreen;

    }
    m_current_screen = m_screens[screen_id];
    m_current_screen->onFocus();
}


void Game::unloadScreens()
{
    for (int i = 0; i < SC_COUNT; ++i)
    {
        // Do not deallocate the current screen
        if (m_screens[i] != NULL && m_screens[i] != m_current_screen)
        {
            delete m_screens[i];
            m_screens[i] = NULL;
        }
    }
}


void Game::quit()
{
    m_running = false;
    SoundSystem::stopAll();
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

    sf::Texture texture;
    texture.create(m_window.getSize().x, m_window.getSize().y);
    texture.update(m_window);
    if (texture.copyToImage().saveToFile(filename))
    {
        std::cout << "screenshot saved to " << filename << std::endl;
    }
}


void Game::setResolution(const sf::Vector2u& size)
{
    if (size == m_window.getSize())
        return;

    // Create window
    m_window.create(sf::VideoMode(size.x, size.y, 16), APP_TITLE, sf::Style::Close);
    sf::View view = sf::View(sf::FloatRect(0, 0, APP_WIDTH, APP_HEIGHT));
    m_window.setView(view);

    if (m_vsync)
        m_window.setVerticalSyncEnabled(m_vsync);
    else
        m_window.setFramerateLimit(APP_FPS);

    // Center window on desktop
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    m_window.setPosition(sf::Vector2i((desktop.width - size.x) / 2, (desktop.height - size.y) / 2));

    // Set window app icon
    sf::Image icon = Resources::getTexture("gui/icon.bmp").copyToImage();
    icon.createMaskFromColor(sf::Color(0xff, 0, 0xff));
    m_window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
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


bool Game::checkResourcesPurity(const std::string& resources_dir)
{
    bool check_passed = true;
    std::ifstream file;
    MD5 md5;

    file.open((resources_dir + XML_WEAPONS).c_str());
    check_passed &= (md5.calculate(file) == MD5SUM_WEAPONS);
    file.close();

    file.open((resources_dir + XML_UPGRADES).c_str());
    check_passed &= (md5.calculate(file) == MD5SUM_UPGRADES);
    file.close();

    file.open((resources_dir + XML_SPACESHIPS).c_str());
    check_passed &= (md5.calculate(file) == MD5SUM_SPACESHIPS);
    file.close();

    file.open((resources_dir + XML_ANIMATIONS).c_str());
    check_passed &= (md5.calculate(file) == MD5SUM_ANIMATIONS);
    file.close();

    return check_passed;
}


bool Game::resourcesChecked() const
{
    return m_resources_checked;
}
