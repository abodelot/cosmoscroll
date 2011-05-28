#ifndef CONTROLPANEL_HPP
#define CONTROLPANEL_HPP

#include <SFML/Graphics.hpp>
#include "../entities/EntityManager.hpp"


/**
 * Panel d'affichage des informations du joueur et de la partie en cours
 */
class ControlPanel: public sf::Drawable
{
public:
	// hauteur en pixels
	enum { HEIGHT = 56 };

	static ControlPanel& GetInstance();

	void Init(EntityManager::Mode mode);

	void Update(float frametime);

	/**
	 * @param text: texte d'information
	 */
	void SetGameInfo(const sf::Unicode::Text& text);

	void SetPoints(int points);

	/**
	 * @param seconds: nombre de secondes écoulées
	 */
	void SetTimer(float seconds);

	/// nb points de vie
	void SetShipHP(int value);

	/// maximum points de vie
	void SetMaxShipHP(int max);

	/// nb points de bouclier
	void SetShield(int value);

	/// maximum points de bouclier
	void SetMaxShield(int max);

	/// chaleur courante
	void SetHeat(int value);

	/// chaleur maximum
	void SetMaxHeat(int max);

	/**
	 * Enable/disable overheat label on overheat bar
	 */
	void SetOverheat(bool overheat);

	/// nb bonus coolers
	void SetCoolers(int coolers);

	/// nb bonus missiles
	void SetMissiles(int count);

	/// @return true si le panel est en haut de l'écran
	bool IsOnTop() const;

	// *-----
	void SetLevelDuration(int seconds);

private:
	ControlPanel();
	ControlPanel(const ControlPanel& other);

	// inherited
	void Render(sf::RenderTarget& target) const;


	struct ProgressBar
	{
		enum
		{
			SHIELD, HP, HEAT, _PBAR_COUNT
		};

		ProgressBar();

		void Init(const sf::Unicode::Text& text, const sf::Font& font, const sf::Color& color);

		// redimensionne la barre
		void SetValue(int value);

		// position de l'ensemble label/bar
		void SetPosition(int x, int y);

		sf::String label_;

		sf::Sprite bar_;
		int max_value_;
		int initial_x_;
	};

	class BonusCounter
	{
	public:
		void Init(const sf::IntRect& subrect, int x, int y);

		void SetValue(int value);

		void Update(float frametime);

		void Show(sf::RenderTarget& target) const;

	private:
		sf::Sprite icon_;
		sf::String count_;
		sf::Sprite glow_;
		float timer_;
		enum GlowingStatus { UP, DOWN, STOP } glowing_;
	};

	ProgressBar pbars_[ProgressBar::_PBAR_COUNT];
	BonusCounter coolers_;
	BonusCounter missiles_;

	sf::String timer_;
	sf::String info_;
	sf::String game_info_;
	sf::Sprite panel_;

	EntityManager::Mode game_mode_;
	// story
	int level_duration_;
	sf::Sprite level_cursor_;
	sf::Sprite level_bar_;
	// arcade
	sf::String str_points_;
	sf::Sprite bar_mask_;
};

#endif // CONTROLPANEL_HPP

