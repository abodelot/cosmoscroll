#ifndef CONTROLPANEL_HPP
#define CONTROLPANEL_HPP

#include <SFML/Graphics.hpp>


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
	 * @param text: texte sur la barre de chaleur
	 */
	void SetOverheatText(const sf::Unicode::Text& text);

	void SetShipHP(int value);

	void SetMaxShipHP(int max);

	void SetShield(int value);

	void SetMaxShield(int max);

	void SetHeat(int value);

	void SetMaxHeat(int max);

	void SetTimer(float seconds);

	void SetCoolers(int coolers);

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
	sf::Font font_big_;
};

#endif // CONTROLPANEL_HPP

