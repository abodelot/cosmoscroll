#ifndef GUI_WIDGETSTYLE_HPP
#define GUI_WIDGETSTYLE_HPP

#include <SFML/Graphics.hpp>

namespace gui
{

class WidgetStyle
{
public:
	WidgetStyle();

	// global
	int global_text_size;
	const sf::Font* global_font;
	const sf::Font* global_fixed_font;
	sf::Color global_border_color;

	// label
	sf::Color label_text_color;

	// button
	sf::Color button_text_color;
	sf::Color button_text_color_focus;

	// checkbox
	sf::Color ckbox_v_color;
	sf::Color ckbox_v_color_focus;
	sf::Color ckbox_bg_color;
	sf::Color ckbox_bg_color_focus;
	int ckbox_size;

	// slider
	sf::Color slider_handle_color;
	sf::Color slider_handle_color_focus;
	sf::Color slider_bg_color;
	sf::Color slider_bg_color_focus;

	// optionlist
	sf::Color optlist_arrow_color;
	sf::Color optlist_arrow_color_focus;
	sf::Color optlist_bg_color;
	sf::Color optlist_bg_color_focus;

	// textbox
	sf::Color textbox_color;
	sf::Color textbox_bg_color;
	sf::Color textbox_bg_color_focus;
};

}

#endif // GUI_WIDGETSTYLE_HPP
