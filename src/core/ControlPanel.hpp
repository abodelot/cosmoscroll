#ifndef CONTROLPANEL_HPP
#define CONTROLPANEL_HPP

#include <SFML/Graphics.hpp>
#include "entities/EntityManager.hpp"
#include "entities/PowerUp.hpp"

/**
 * HUD: panel displaying various data about player status, current level, ...
 */
class ControlPanel: public sf::Drawable, public sf::Transformable
{
public:
	// height in pixels, width is Game::WIDTH
	enum { HEIGHT = 56 };

	static ControlPanel& getInstance();

	void setMode(EntityManager::Mode mode);

	void update(float frametime);

	/**
	 * @param text: texte d'information
	 */
	void setGameInfo(const sf::String& text);

	void setScore(int score);
	void setHighscore(int highscore);

	/**
	 * @param seconds: elapsed time
	 */
	void setElapsedTime(float seconds);

	// Set durée du niveau courant, en secondes
	void setLevelDuration(int seconds);

	// setters progress bars --------------------------------------------------

	/**
	 * Set current player HP and max HP
	 */
	void setHP(int value);
	void setMaxHP(int max);

	/**
	 * Set current shield points and max shield points
	 */
	void setShield(int value);
	void setMaxShield(int max);

	/**
	 * Set current heat and max heat
	 */
	void setHeat(float value);
	void setMaxHeat(float max);

	/**
	 * Enable/disable overheat effect on heat progress bar
	 */
	void setOverheat(bool overheat);

	// setters bonus slots ----------------------------------------------------

	// Set icecubess count
	void setIcecubes(int count);

	// Set missiles count
	void setMissiles(int count);

	void activeSpeedPowerUp(int seconds);

	void activeAttackPowerUp(int seconds, PowerUp::Type bonus_type);

	void refreshTextTranslations();

private:
	ControlPanel();
	ControlPanel(const ControlPanel& other);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	struct ProgressBar
	{
		enum
		{
			SHIELD, HP, HEAT, _PBAR_COUNT
		};

		ProgressBar();

		void init(const sf::String& text, const sf::Font& font, const sf::Color& color);

		// Set progress bar value
		void setValue(int value);

		// Set widget position
		void setPosition(int x, int y);

		sf::Text label_;
		sf::RectangleShape bar_;
		sf::Text value_;
		int max_value_;
	};

	struct PowerUpSlot: public sf::Drawable
	{
		enum Type { COUNTER, TIMER };

		void init(PowerUp::Type bonus_type, Type type, const sf::Font& font);

		// Set widget position
		void setPosition(int x, int y);

		void setValue(int value);

		void update(float frametime);

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		sf::Sprite icon_;
		sf::Text label_;
		sf::Sprite glow_;
		float timer_;
		enum GlowingStatus { UP, DOWN, STOP } glowing_;
		Type type_;
	};

	ProgressBar pbars_[ProgressBar::_PBAR_COUNT];
	PowerUpSlot bs_coolers_;
	PowerUpSlot bs_missiles_;
	PowerUpSlot bs_attack_;
	PowerUpSlot bs_speed_;

	sf::Text timer_;
	sf::Text game_info_;
	sf::Sprite m_background;

	EntityManager::Mode game_mode_;
	// levels
	int level_duration_;
	sf::Sprite level_cursor_;
	sf::Sprite level_bar_;
	// infinity mode
	sf::Text str_points_;
	sf::Sprite bar_mask_;
};

#endif // CONTROLPANEL_HPP
