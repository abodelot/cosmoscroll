#ifndef LEVELMENU_HPP
#define LEVELMENU_HPP

#include "scenes/BaseMenu.hpp"
#include "CreditCounterWidget.hpp"

class LevelMenu: public BaseMenu
{
public:
    LevelMenu();

    void onFocus() override;

private:
    sf::Text              m_title;
    CreditCounterWidget*  m_credits;
    gui::OptionList<int>* m_optLevels;
    gui::Label*           m_labProgresion;
};

#endif
