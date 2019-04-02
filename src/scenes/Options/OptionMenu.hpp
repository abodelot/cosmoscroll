#ifndef OPTIONMENU_HPP
#define OPTIONMENU_HPP

#include "scenes/BaseMenu.hpp"

class OptionMenu: public BaseMenu
{
public:
    OptionMenu();

private:
    void EventCallback(int id) override;
};

#endif // OPTIONMENU_HPP

