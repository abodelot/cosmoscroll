#include "BaseMenu.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include "core/SoundSystem.hpp"
#include "core/Resources.hpp"


gui::WidgetStyle BaseMenu::m_gui_style;

BaseMenu::BaseMenu():
    gui::Menu(Game::getInstance().getWindow(), m_gui_style),
    m_ui_background(Resources::getTexture("gui/main-screen.png")),
    m_scrolling_background(Resources::getTexture("gui/background.png"))
{
    m_gui_style.global_font = &Resources::getFont("hemi-head.ttf");
    m_gui_style.global_fixed_font = &Resources::getFont("VeraMono.ttf");
}


void BaseMenu::onEvent(const sf::Event& event)
{
    // propaging events to gui
    gui::Menu::OnEvent(event);
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
    m_scrolling_background.setPosition(0, y);

    // updating gui
    gui::Menu::Update(frametime);
}


void BaseMenu::draw(sf::RenderTarget& target) const
{
    // Draw scrolling background twice
    target.draw(m_scrolling_background);
    m_scrolling_background.setOrigin(0.f, sfh::height(m_scrolling_background));
    target.draw(m_scrolling_background);
    m_scrolling_background.setOrigin(0.f, 0.f);

    // Draw gui
    target.draw(m_ui_background);
    gui::Menu::Show(target);
    target.draw(m_title);
}


void BaseMenu::setTitle(const sf::String& text, int y)
{
    m_title.setFont(*m_gui_style.global_font);
    m_title.setCharacterSize(40);
    m_title.setString(text);
    int x = (APP_WIDTH - m_title.getLocalBounds().width) / 2;
    m_title.setPosition(x, y);
}


const sf::Text& BaseMenu::getTitle() const
{
    return m_title;
}


void BaseMenu::OnWidgetFocused()
{
    SoundSystem::playSound("menu-select.ogg");
}
