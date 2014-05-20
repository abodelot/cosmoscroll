#ifndef BASEMENU_HPP
#define BASEMENU_HPP

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

#include "BaseScene.hpp"
#include "CosmoButton.hpp"
#include "../gui/gui.hpp"

/**
 * Base class for scenes with interactive gui
 */
class BaseMenu: public BaseScene, public gui::Menu
{
public:
	BaseMenu();

	void OnEvent(const sf::Event& event) override;

	void Update(float frametime) override;

	void Show(sf::RenderTarget& target) const override;

protected:
	/**
	 * Indiquer le titre du menu (aucun par défaut)
	 * @param text: contenu du titre
	 * @param y: décalage axe Y
	 */
	void SetTitle(const sf::String& text, int y = 20);
	const sf::Text& GetTitle() const;

	void OnWidgetFocused() override;

private:
	sf::Text title_;
	mutable sf::Sprite scrolling_background_;
	static gui::WidgetStyle global_style_;
};

#endif // BASEMENU_HPP

