#ifndef CONTROLPANEL_HPP
#define CONTROLPANEL_HPP

#include <SFML/Graphics.hpp>


/**
 * Panel d'affichage des informations du joueur et de la partie en cours
 */
class ControlPanel: public sf::Drawable
{
public:
	// hauteur en pixels
	enum { HEIGHT = 56 };

	static ControlPanel& GetInstance();

	/**
	 * @param text: texte d'information
	 */
	void SetGameInfo(const sf::Unicode::Text& text);

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
	 * @param text: texte sur la barre de chaleur
	 */
	void SetOverheatText(const sf::Unicode::Text& text);

	/// nb bonus coolers
	void SetCoolers(int coolers);

	/// nb bonus missiles
	void SetMissiles(int count);

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
		// redimensionne la barre
		void SetPercent(int value);
		void Init(const sf::Font& font, float x, float y);
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
	sf::Font font_;
};

#endif // CONTROLPANEL_HPP

