#ifndef BASEMENU_HPP
#define BASEMENU_HPP

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

#include "BaseScene.hpp"
#include "CosmoButton.hpp"
#include "../gui/gui.hpp"

/**
 * Menu navigable pour sélectionner un choix
 */
class BaseMenu: public BaseScene, public gui::Menu
{
public:
	BaseMenu();

	// override
	void OnEvent(const sf::Event& event);

	// override
	void Update(float frametime);

	// override
	void Show(sf::RenderTarget& target) const;

	// override
	void OnFocus();

protected:
	/**
	 * Indiquer le titre du menu (aucun par défaut)
	 * @param text: contenu du titre
	 * @param y: décalage axe Y
	 */
	void SetTitle(const sf::Unicode::Text& text, int y = 20);
	const sf::String& GetTitle() const;

	// override
	void OnWidgetFocused();

private:
	sf::String title_;
	mutable sf::Sprite scrolling_background_;
};

#endif // BASEMENU_HPP

