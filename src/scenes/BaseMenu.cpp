#include "BaseMenu.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include "core/Services.hpp"
#include "core/Resources.hpp"


gui::WidgetStyle BaseMenu::s_guiStyle;

BaseMenu::BaseMenu():
    gui::Menu(Game::getInstance().getWindow(), s_guiStyle),
    m_uiBackground(Resources::getTexture("gui/main-screen.png")),
    m_scrollingBackground(Resources::getTexture("gui/background.png"))
{
    s_guiStyle.global_font = &Resources::getFont("Vera.ttf");
    s_guiStyle.global_fixed_font = &Resources::getFont("VeraMono.ttf");
    s_guiStyle.global_title_font = &Resources::getFont("hemi-head.ttf");
}


void BaseMenu::onEvent(const sf::Event& event)
{
    // propaging events to gui
    gui::Menu::onEvent(event);
}


void BaseMenu::update(float frametime)
{
    static float y = 0;
    // Apply vertical scrolling
    y -= 30.f * frametime;
    if (y < 0)
    {
        y = APP_HEIGHT;
    }
    m_scrollingBackground.setPosition(0, y);

    // updating gui
    gui::Menu::update(frametime);
}


void BaseMenu::draw(sf::RenderTarget& target) const
{
    // Draw scrolling background twice
    target.draw(m_scrollingBackground);
    m_scrollingBackground.setOrigin(0.f, sfh::height(m_scrollingBackground));
    target.draw(m_scrollingBackground);
    m_scrollingBackground.setOrigin(0.f, 0.f);

    // Draw gui
    target.draw(m_uiBackground);
    gui::Menu::show(target);
    target.draw(m_title);
}


void BaseMenu::setTitle(const sf::String& text, int y)
{
    m_title.setFont(*s_guiStyle.global_title_font);
    m_title.setCharacterSize(50);
    m_title.setString(text);
    m_title.setOutlineColor(sf::Color::Black);
    m_title.setOutlineThickness(2);
    int x = (APP_WIDTH - m_title.getLocalBounds().width) / 2;
    m_title.setPosition(x, y);
}


const sf::Text& BaseMenu::getTitle() const
{
    return m_title;
}


void BaseMenu::onWidgetFocused()
{
    Services::getSoundSystem().playSound("menu-select.ogg");
}
