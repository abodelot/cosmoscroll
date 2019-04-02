#ifndef PAUSEMENU_HPP
#define PAUSEMENU_HPP

#include "BaseMenu.hpp"

class PauseMenu: public BaseMenu
{
public:
    PauseMenu();

    void onEvent(const sf::Event& event) override;

    void onFocus() override;

    void update(float frametime) override;

    void draw(sf::RenderTarget& target) const override;

private:
    void EventCallback(int id) override;
};

#endif // PAUSEMENU_HPP

