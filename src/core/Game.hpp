#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics/RenderWindow.hpp>

#include "Input.hpp"
#include "PlayerSave.hpp"

class BaseScene;

/**
 * Gestion du déroulement du jeu
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

	static Game& GetInstance();

	/**
	 * @param path: name by which the program was called
	 */
	void SetCurrentDirectory(const std::string& path);

	/**
	 * Override location of the configuration file
	 * Must be called before Init
	 * @param config_file: directory of file
	 */
	void OverrideConfigFile(const std::string& config_file);

	void Init(const std::string& data_dir, int level_set);

	/**
	 * Lancer une partie de CosmoScroll
	 * @return error code
	 */
	int Run();

	/**
	 * @return application rendering window
	 */
	sf::RenderWindow& GetApp();

	/**
	 * Quit CosmoScroll
	 */
	void Quit();

	/**
	 * Indiquer la prochaine scène à afficher
	 */
	void SetNextScene(Scene scene);

	/**
	 * Holds windowed/fullscreen display mode
	 */
	void SetFullscreen(bool full);
	bool IsFullscreen() const;

	/**
	 * Holds vertical synchronization property
	 */
	void SetVerticalSync(bool vsync);
	bool IsVerticalSync() const;

	void ReloadScenes();

	inline bool IsPure() const { return pure_; }
	inline static PlayerSave& GetPlayerSave() { return GetInstance().playersave_; }

	void PanelOnTop(bool top);

private:
	Game();
	~Game();

	/**
	 * Load a configuration file
	 */
	bool LoadConfig(const std::string& filename);

	/**
	 * Write the configuration in a file
	 */
	void WriteConfig(const std::string& filename) const;

	/**
	 * Take a screenshot and save the image to screenshot_dir_
	 */
	void TakeScreenshot(void);

	/**
	 * Check game data files are unaltered
	 */
	bool CheckResourcesPurity();

	void BSOD(std::string message);

	sf::RenderWindow app_;
	bool fullscreen_;
	bool vsync_;
	bool running_;
    bool pure_;

	// event manager
	Input& input_;

	// scènes
	BaseScene* scenes_[SC_COUNT];
	BaseScene* current_scene_;

	PlayerSave playersave_;

	// directories
	std::string current_dir_;
	std::string data_dir_;
	std::string screenshot_dir_;
	std::string config_file_;

	// groupe de niveaux
	unsigned int level_set_;
};

#endif // GAME_HPP
