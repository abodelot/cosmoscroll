#ifndef CONTROLPANEL_HPP
#define CONTROLPANEL_HPP

#include <SFML/Graphics.hpp>

#define CONTROL_PANEL_HEIGHT 56

class ControlPanel
{
public:
    static ControlPanel& GetInstance();

    void SetShipHP(int value);

    void SetMaxShipHP(int max);

    void SetShield(int value);

    void SetMaxShield(int max);

    void SetHeat(int value);

    void SetMaxHeat(int max);


    void SetInfo(const char* text);
    
    void SetTimer(float seconds);
    
    void SetCoolers(int coolers);
    
    void Show(sf::RenderWindow& app) const;
    
private:
    ControlPanel();
    ControlPanel(const ControlPanel& other);
    
    enum
    {
        SHIELD, HP, HEAT, PBAR_COUNT
    };
    
    struct ProgressBar
    {
        sf::String label;
        sf::Shape background;
        sf::Sprite bar;
        int max_value;
        
        ProgressBar();
        // redimensionne la barre
        void SetPercent(int value);
        void SetPosition(float x, float y);
    };
    
    ProgressBar pbars_[PBAR_COUNT];
    sf::String timer_;
    sf::String info_;
    sf::String coolers_;
    sf::Sprite panel_;
    sf::Font font_;
};

#endif /* guard CONTROLPANEL_HPP */

