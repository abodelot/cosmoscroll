#ifndef GAMEOVERSCREEN_HPP
#define GAMEOVERSCREEN_HPP

#include "Screen.hpp"

class EntityManager;

class GameOverScreen: public Screen {
public:
    GameOverScreen();

    void onEvent(const sf::Event& event) override;

    void onFocus() override;

    void update(float frametime) override;

    void draw(sf::RenderTarget& target) const override;

private:
    sf::Clock            m_startedAt;
    sf::Text             m_text;
    const EntityManager& m_entities;
};

#endif
