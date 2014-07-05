#ifndef BASEMENU_HPP
#define BASEMENU_HPP

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

#include "Screen.hpp"
#include "CosmoButton.hpp"
#include "../gui/gui.hpp"

/**
 * Base class for scenes with interactive gui
 */
class BaseMenu: public Screen, public gui::Menu
{
public:
	BaseMenu();

	void onEvent(const sf::Event& event) override;

	void update(float frametime) override;

	void draw(sf::RenderTarget& target) const override;

protected:
	/**
	 * Set menu title (top of the screen)
	 */
	void setTitle(const sf::String& text, int y = 20);
	const sf::Text& getTitle() const;

	void OnWidgetFocused() override;

private:
	sf::Text           m_title;
	sf::Sprite         m_ui_background;
	mutable sf::Sprite m_scrolling_background;
	static gui::WidgetStyle m_gui_style;
};

#endif // BASEMENU_HPP

