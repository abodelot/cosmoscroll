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

	/**
	 * @return instance unique
	 */
	static Game& GetInstance(unsigned int level_set = 0);

	/**
	 * @param path: name by which the program was called
	 */
	void Init(const std::string& path);

	/**
	 * Lancer une partie de CosmoScroll
	 */
	void Run();

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
	 * Basculer entre les modes fenêtré et plein écran
	 */
	void SetFullscreen(bool full);
	bool IsFullscreen() const;

	void ReloadScenes();

	inline bool IsPure() const { return pure_; }
	inline static PlayerSave& GetPlayerSave() { return GetInstance().playersave_; }

	void PanelOnTop(bool top);

private:
	Game(unsigned int level_set = 0);
	~Game();

	/**
	 * Load a configuration file
	 */
	bool LoadConfig(const char* filename);

	/**
	 * Write the configuration in a file
	 */
	void WriteConfig(const char* filename) const;

	/**
	 * Take a screenshot and save the image
	 */
	void TakeScreenshot(const char* directory);

	/**
	 * Check game data files are unaltered
	 */
	void CheckPurity();

	sf::RenderWindow app_;
	bool fullscreen_;
	bool running_;
    bool pure_;
	std::string directory_;

	// event manager
	Input& input_;

	// scènes
	BaseScene* scenes_[SC_COUNT];
	BaseScene* current_scene_;
	PlayerSave playersave_;

	// groupe de niveaux
	unsigned int level_set_;
};

#endif // GAME_HPP
