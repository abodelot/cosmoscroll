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

	enum
	{
		SHIELD, HP, HEAT, PBAR_COUNT
	};
	struct ProgressBar
	{
		sf::String label;
		sf::Shape background;
		sf::Sprite bar;
		int max_value;

		ProgressBar();

		void Init(const sf::Unicode::Text& text, const sf::Font& font, const sf::Image& img);

		// redimensionne la barre
		void SetPercent(int value);

		void SetPosition(int x, int y);

	};

	struct BonusCount
	{
		sf::Sprite icon;
		sf::String count;

		void Init(const sf::IntRect& subrect, int x, int y);
	};

	ProgressBar pbars_[PBAR_COUNT];
	BonusCount coolers_;
	BonusCount missiles_;

	sf::String timer_;
	sf::String info_;
	sf::String game_info_;
	sf::Sprite panel_;

	EntityManager::Mode game_mode_;
	// story -----------
	int level_duration_;
	sf::Sprite level_cursor_;
	sf::Sprite level_bar_;
	// arcade
	sf::String str_points_;
};

#endif // CONTROLPANEL_HPP

