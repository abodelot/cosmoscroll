#include "WidgetStyle.hpp"

#define DEFAULT_COLOR    sf::Color(0x3e, 0xde, 0)
#define DEFAULT_BG_COLOR sf::Color(0x70, 0x70, 0x70)
#define FOCUS_COLOR      sf::Color(0x3e, 0xde, 0)
#define FOCUS_BG_COLOR   sf::Color(0x50, 0x50, 0x50)

using namespace gui;

WidgetStyle::WidgetStyle()
{
	global_text_size = 20;
	global_font = NULL;
	global_fixed_font = NULL;
	global_border_color = sf::Color(0x30, 0x30, 0x30);

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

	textbox_color = sf::Color::Black;
	textbox_bg_color = sf::Color(0xcc, 0xcc, 0xcc);
	textbox_bg_color_focus = sf::Color::White;
}
