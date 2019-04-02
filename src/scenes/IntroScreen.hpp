#ifndef INTROSCREEN_HPP
#define INTROSCREEN_HPP

#include "Screen.hpp"

class EntityManager;
class Player;

/**
 * Interactive splash screen displayed at game launch
 */
class IntroScreen: public Screen
{
public:
    IntroScreen();

    void onEvent(const sf::Event& event) override;

    void update(float frametime) override;

    void draw(sf::RenderTarget& target) const override;

private:
    float          m_elapsed;
    sf::Sprite     m_background;
    sf::Sprite     m_title;
    EntityManager& m_entities;
    Player*        m_spaceship;
};

#endif // INTROSCREEN_HPP

