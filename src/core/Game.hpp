#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics/RenderWindow.hpp>

class Screen;

/**
 * Game controller singleton
 */
class Game
{
public:
    enum ScreenID
    {
        SC_IntroScreen,
        SC_PlayScreen,
        SC_GameOverScreen,
        SC_PauseMenu,
        SC_MainMenu,
        SC_AboutMenu,
        SC_LevelMenu,
        SC_ArmoryMenu,
        SC_OptionMenu,
        SC_KeyboardMenu,
        SC_JoystickMenu,
        SC_SettingsMenu,
        SC_AudioMenu,

        SC_COUNT
    };

    static Game& getInstance();

    /**
     * @param path: name by which the program was called
     */
    void init(const std::string& path);

    void loadResources(const std::string& data_dir);

    /**
     * Override location of the configuration file
     * @param config_path: directory of file
     */
    void setConfigFile(const std::string& config_path);

    /**
     * Load the configuration file
     */
    bool loadConfig();

    /**
     * Enter application main loop
     * @return error code
     */
    int run();

    /**
     * Get application rendering window
     */
    sf::RenderWindow& getWindow();

    /**
     * Save settings and exit application
     */
    void quit();

    /**
     * Set screen to be displayed
     */
    void setCurrentScreen(ScreenID screen);

    /**
     * Deallocate loaded screens, except the current one
     */
    void unloadScreens();

    /**
     * Set the render window resolution
     */
    void setResolution(const sf::Vector2u& size);

    /**
     * Holds vertical synchronization property
     */
    void setVerticalSync(bool vsync);
    bool isVerticalSync() const;

private:
    Game();
    ~Game();

    /**
     * Save the configuration
     */
    void writeConfig() const;

    /**
     * Take a screenshot and save the image to DEFAULT_SCREENSHOT_DIR
     */
    void takeScreenshot() const;

    sf::RenderWindow m_window;
    bool m_vsync;
    bool m_running;

    // Screens
    Screen* m_screens[SC_COUNT];
    Screen* m_current_screen;

    std::string m_app_dir; // Directory from which application is running
    std::string m_config_filename;
};

#endif // GAME_HPP
