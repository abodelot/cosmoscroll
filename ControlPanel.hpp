#ifndef CONTROLPANEL_HPP
#define CONTROLPANEL_HPP

#include <SFML/Graphics.hpp>

#define CONTROL_PANEL_HEIGHT 56

class ControlPanel
{
public:
    static ControlPanel& GetInstance();

    void SetShipHP(int hp);

    void SetShield(int n);

    void SetInfo(const char* text);

    void SetHeat(float heat);

    void SetChrono(float seconds);
    
    void Show(sf::RenderWindow& app);

private:
    ControlPanel();
    ControlPanel(const ControlPanel& other);
    
    enum 
    {
        SHIELD, HP, HEAT, INFO, CHRONO, STR_COUNT
    };
    sf::String str_[STR_COUNT];
    sf::Sprite panel_;
    sf::Font font_;
};

#endif /* guard CONTROLPANEL_HPP */

