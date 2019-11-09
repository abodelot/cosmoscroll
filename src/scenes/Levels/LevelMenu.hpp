#ifndef LEVELMENU_HPP
#define LEVELMENU_HPP

#include "scenes/BaseMenu.hpp"
#include "CreditCounterWidget.hpp"

class LevelManager;

class LevelMenu: public BaseMenu
{
public:
    LevelMenu();

    void onFocus() override;

private:
    sf::Text              m_title;
    LevelManager&         m_levels;
    CreditCounterWidget*  m_credits;
    gui::OptionList<int>* m_opt_levels;
    gui::Label*           m_lab_progresion;
};

#endif // LEVELMENU_HPP
