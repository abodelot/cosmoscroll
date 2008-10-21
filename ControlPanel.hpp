#ifndef H_CONTROL_PANEL
#define H_CONTROL_PANEL

#include <SFML/Graphics.hpp>

class ControlPanel
{
public:
    static ControlPanel& GetInstance();

    void SetShipHP(int hp);

    void SetShield(int n);

    void SetInfo(const char* text);

    void SetHeat(float heat);

    void Show(sf::RenderWindow& app);


private:
    ControlPanel();
    ControlPanel(const ControlPanel& other);
    
    enum 
    {
        SHIELD, HP, HEAT, INFO, STR_COUNT
    };
    sf::String str_[STR_COUNT];
    sf::Sprite panel_;
    sf::Font font_;
};

#endif /* guard H_CONTROL_PANEL */

