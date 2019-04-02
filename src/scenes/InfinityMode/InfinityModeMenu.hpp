#ifndef INFINITYMODEMENU_HPP
#define INFINITYMODEMENU_HPP

#include "scenes/BaseMenu.hpp"

class InfinityModeMenu: public BaseMenu
{
public:
    InfinityModeMenu();

private:
    void EventCallback(int id) override;
};

#endif // INFINITYMODEMENU_HPP
