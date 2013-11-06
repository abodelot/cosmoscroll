#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics/RenderWindow.hpp>

#include "Input.hpp"
#include "PlayerSave.hpp"

class BaseScene;

/**
 * Game controller singleton
 */
class Game
{
public:
	enum Size
	{
		WIDTH =  640,
		HEIGHT = 480
	};

	enum Scene
	{
		SC_IntroScene,
		SC_InGameScene,
		SC_EndGameScene,
		SC_ArcadeMenu,
		SC_GameOverMenu,
		SC_BestScoresMenu,
		SC_PauseMenu,
		SC_MainMenu,
		SC_AboutMenu,
		SC_LevelMenu,
		SC_ArmoryMenu,
		SC_IntroLevelScene,
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

	/**
	 * Override location of the configuration file
	 * Must be called before Init
	 * @param config_file: directory of file
	 */
	void setConfigFile(const std::string& config_file);

	void loadResources(const std::string& data_dir);

	/**
	 * Lancer une partie de CosmoScroll
	 * @return error code
	 */
	int run();

	/**
	 * @return application rendering window
	 */
	sf::RenderWindow& getWindow();

	/**
	 * Quit CosmoScroll
	 */
	void quit();

	/**
	 * Indiquer la prochaine scène à afficher
	 */
	void setNextScene(Scene scene);

	/**
	 * Holds windowed/fullscreen display mode
	 */
	void setFullscreen(bool full);
	bool isFullscreen() const;

	/**
	 * Holds vertical synchronization property
	 */
	void setVerticalSync(bool vsync);
	bool isVerticalSync() const;

	void reloadScenes();

	bool resourcesChecked() const;

	inline static PlayerSave& getPlayerSave() { return getInstance().playersave_; }

	void panelOnTop(bool top);

private:
	Game();
	~Game();

	/**
	 * Load a configuration file
	 */
	bool loadConfig(const std::string& filename);

	/**
	 * Write the configuration in a file
	 */
	void writeConfig(const std::string& filename) const;

	/**
	 * Take a screenshot and save the image to screenshot_dir_
	 */
	void takeScreenshot(void);

	/**
	 * Check game data files are unaltered
	 */
	bool checkResourcesPurity(const std::string& resources_dir);

	void BSOD(std::string message);

	sf::RenderWindow m_window;
	bool m_fullscreen;
	bool m_vsync;
	bool m_running;
    bool m_resources_checked;

	// event manager
	Input& m_input;

	// scènes
	BaseScene* m_scenes[SC_COUNT];
	BaseScene* m_current_scene;

	PlayerSave playersave_;

	// directories
	std::string m_app_dir;
	std::string m_screenshots_dir;
	std::string m_config_file;
};

#endif // GAME_HPP
