#include "WidgetStyle.hpp"

#define DEFAULT_COLOR    sf::Color(0, 0xde, 0)
#define DEFAULT_BG_COLOR sf::Color(0x60, 0x5a, 0x5c)
#define FOCUS_COLOR      sf::Color(0x70, 0xde, 0)
#define FOCUS_BG_COLOR   sf::Color(0xaa, 0xa0, 0xa0)

using namespace gui;

WidgetStyle::WidgetStyle()
{
	global_text_size = 20;
	global_font = &sf::Font::GetDefaultFont();
	global_border_color = sf::Color(0xa5, 0xa5, 0xba);

	label_text_color = sf::Color::White;

	button_text_color = sf::Color::White;
	button_text_color_focus = sf::Color::Green;

	ckbox_v_color = DEFAULT_COLOR;
	ckbox_v_color_focus = FOCUS_COLOR;
	ckbox_bg_color = DEFAULT_BG_COLOR;
	ckbox_bg_color_focus = FOCUS_BG_COLOR;
	ckbox_size = 20; // todo

	slider_handle_color = DEFAULT_COLOR;
	slider_handle_color_focus = FOCUS_COLOR;
	slider_bg_color = DEFAULT_BG_COLOR;
	slider_bg_color_focus = FOCUS_BG_COLOR;

	optlist_arrow_color = DEFAULT_COLOR;
	optlist_arrow_color_focus = FOCUS_COLOR;
	optlist_bg_color = DEFAULT_BG_COLOR;
	optlist_bg_color_focus = FOCUS_BG_COLOR;

	textbox_bg_color = DEFAULT_BG_COLOR;
	textbox_bg_color_focus = FOCUS_BG_COLOR;
}
