#ifndef CONTROLPANEL_HPP
#define CONTROLPANEL_HPP

#include <SFML/Graphics.hpp>
#include "entities/EntityManager.hpp"
#include "entities/PowerUp.hpp"
#include "utils/sfml_helper.hpp"


/**
 * HUD: panel displaying various data about player status, current level, ...
 */
class ControlPanel: public sf::Drawable, public sf::Transformable
{
public:
	// height in pixels, width is Game::WIDTH
	enum { HEIGHT = 56 };

	static ControlPanel& GetInstance();

	void Init(EntityManager::Mode mode);

	void Update(float frametime);

	/**
	 * @param text: texte d'information
	 */
	void SetGameInfo(const sf::String& text);

	void setScore(int score);

	/**
	 * @param seconds: nombre de secondes écoulées
	 */
	void SetTimer(float seconds);

	/// @return true si le panel est en haut de l'écran
	bool IsOnTop() const;

	// Set durée du niveau courant, en secondes
	void SetLevelDuration(int seconds);

	// setters progress bars --------------------------------------------------

	/**
	 * Set current player HP and max HP
	 */
	void SetShipHP(int value);
	void SetMaxShipHP(int max);

	/**
	 * Set current shield points and max shield points
	 */
	void SetShield(int value);
	void SetMaxShield(int max);

	/**
	 * Set current heat and max heat
	 */
	void SetHeat(int value);
	void SetMaxHeat(int max);

	/**
	 * Enable/disable overheat effect on heat progress bar
	 */
	void SetOverheat(bool overheat);

	// setters bonus slots ----------------------------------------------------

	// Set coolers count
	void SetCoolers(int coolers);

	// Set missiles count
	void SetMissiles(int count);

	void ActiveSpeedPowerUp(int seconds);

	void ActiveAttackPowerUp(int seconds, PowerUp::Type bonus_type);

	void RefreshTextTranslations();

private:
	ControlPanel();
	ControlPanel(const ControlPanel& other);

	// inherited
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;


	struct ProgressBar
	{
		enum
		{
			SHIELD, HP, HEAT, _PBAR_COUNT
		};

		ProgressBar();

		void Init(const sf::String& text, const sf::Font& font, const sf::Color& color);

		// redimensionne la barre
		void SetValue(int value);

		// position de l'ensemble label/bar
		void setPosition(int x, int y);

		xsf::Text label_;

		sf::RectangleShape bar_;
		xsf::Text value_;
		int max_value_;
	};

	class PowerUpSlot
	{
	public:
		enum Type { COUNTER, TIMER };

		void Init(PowerUp::Type bonus_type, Type type);
		// set widget position
		void setPosition(int x, int y);

		void SetValue(int value);

		void Update(float frametime);

		void Show(sf::RenderTarget& target) const;

	private:
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
	sf::Sprite panel_;

	EntityManager::Mode game_mode_;
	// story
	int level_duration_;
	xsf::Sprite level_cursor_;
	xsf::Sprite level_bar_;
	// arcade
	sf::Text str_points_;
	sf::Sprite bar_mask_;
};

#endif // CONTROLPANEL_HPP

