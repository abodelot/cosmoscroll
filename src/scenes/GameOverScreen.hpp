#ifndef GAMEOVERSCREEN_HPP
#define GAMEOVERSCREEN_HPP

#include "Screen.hpp"

class EntityManager;
class ControlPanel;

class GameOverScreen: public Screen
{
public:
    GameOverScreen();

    void onEvent(const sf::Event& event) override;

    void onFocus() override;

    void update(float frametime) override;

    void draw(sf::RenderTarget& target) const override;

private:
    void goNextScreen() const;

    sf::Clock            m_started_at;
    sf::Text             m_text;
    const EntityManager& m_entities;
    const ControlPanel&  m_panel;
};

#endif // GAMEOVERSCREEN_HPP

