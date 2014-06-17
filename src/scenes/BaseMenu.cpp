#include "BaseMenu.hpp"
#include "core/Constants.hpp"
#include "core/SoundSystem.hpp"
#include "core/Resources.hpp"
#include "utils/SFML_Helper.hpp"


gui::WidgetStyle BaseMenu::global_style_;

BaseMenu::BaseMenu(): gui::Menu(global_style_)
{
	SetBackground(sf::Sprite(Resources::getTexture("gui/main-screen.png")));
	scrolling_background_.setTexture(Resources::getTexture("gui/background.png"));

	GetWidgetStyle().global_font = &Resources::getFont("hemi-head.ttf");
	GetWidgetStyle().global_fixed_font = &Resources::getFont("VeraMono.ttf");
}


void BaseMenu::OnEvent(const sf::Event& event)
{
	// propaging events to gui
	gui::Menu::OnEvent(event);
}


void BaseMenu::Update(float frametime)
{
	static const int SPEED = 30;
	static float y = 0;
	y -= SPEED * frametime;
	if (y < 0)
	{
		y = APP_HEIGHT;
	}
	scrolling_background_.setPosition(0, y);

	// updating gui
	gui::Menu::Update(frametime);
}


void BaseMenu::Show(sf::RenderTarget& target) const
{
	// drawing background twice for scrolling purpose
	target.draw(scrolling_background_);
	scrolling_background_.setOrigin(0.f, sfh::height(scrolling_background_));
	target.draw(scrolling_background_);
	scrolling_background_.setOrigin(0.f, 0.f);

	// drawing gui
	gui::Menu::Show(target);
	target.draw(title_);
}


void BaseMenu::SetTitle(const sf::String& text, int y)
{
	title_.setFont(*GetWidgetStyle().global_font);
	title_.setCharacterSize(40);
	title_.setString(text);
	title_.setPosition((APP_WIDTH - sfh::width(title_)) / 2, y);
}


const sf::Text& BaseMenu::GetTitle() const
{
	return title_;
}


void BaseMenu::OnWidgetFocused()
{
	SoundSystem::playSound("menu-select.ogg");
}
