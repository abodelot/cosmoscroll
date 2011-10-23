#include "WidgetStyle.hpp"

#define DEFAULT_COLOR   sf::Color(0, 0x53, 0x9f)
#define FOCUS_COLOR     sf::Color(0, 0x6c, 0xcf)
#define FOCUS_BG_COLOR  sf::Color(0xf6, 0xe7, 0xa0)

using namespace gui;

WidgetStyle::WidgetStyle()
{
	global_text_size = 20;
	global_font = &sf::Font::GetDefaultFont();
	global_border_color = sf::Color(0x80, 0x80, 0x80);

	label_text_color = sf::Color::White;

	button_text_color = sf::Color::White;
	button_text_color_focus = sf::Color::Green;

	ckbox_v_color = DEFAULT_COLOR;
	ckbox_v_color_focus = FOCUS_COLOR;
	ckbox_bg_color = sf::Color::White;
	ckbox_bg_color_focus = FOCUS_BG_COLOR;
	ckbox_size = 20; // todo

	slider_handle_color = DEFAULT_COLOR;
	slider_handle_color_focus = FOCUS_COLOR;
	slider_bg_color = sf::Color::White;
	slider_bg_color_focus = FOCUS_BG_COLOR;

	optlist_arrow_color = DEFAULT_COLOR;
	optlist_arrow_color_focus = FOCUS_COLOR;
	optlist_bg_color = sf::Color::White;
	optlist_bg_color_focus = FOCUS_BG_COLOR;

	textbox_bg_color = sf::Color::White;
	textbox_bg_color_focus = FOCUS_BG_COLOR;
}
