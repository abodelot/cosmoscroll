#ifndef PLAYSCREEN_HPP
#define PLAYSCREEN_HPP

#include "Screen.hpp"

class EntityManager;
class MessageSystem;
class Game;

/**
 * Playing state
 */
class PlayScreen: public Screen
{
public:
    PlayScreen();

    void onEvent(const sf::Event& event) override;

    void onFocus() override;

    void update(float frametime) override;

    void draw(sf::RenderTarget& target) const override;

private:
    EntityManager& m_entities;
    Game& m_game;
    MessageSystem& m_messageSystem;
};

#endif // PLAYSCREEN_HPP

