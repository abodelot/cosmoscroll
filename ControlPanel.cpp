#include "ControlPanel.hpp"
#include "MediaManager.hpp"

#include <sstream>

ControlPanel& ControlPanel::GetInstance()
{
    static ControlPanel self;
    return self;
}


void ControlPanel::SetShipHP(int hp)
{
    std::ostringstream s;
    s << "Vaisseau : " << hp << "HP";
    str_[HP].SetText(s.str());
}

void ControlPanel::SetShield(int n)
{
    std::ostringstream s;
    s << "Bouclier : " << n;
    str_[SHIELD].SetText(s.str());    
}

void ControlPanel::SetInfo(const char* text)
{
    str_[INFO].SetText(text);
}

void ControlPanel::SetHeat(float heat)
{
    std::ostringstream s;
    s << "Chaleur : " << static_cast<int>(heat);
    str_[HEAT].SetText(s.str());
}


void ControlPanel::Show(sf::RenderWindow& app)
{
    app.Draw(panel_);
    for (int i = 0; i < STR_COUNT; ++i)
    {
        app.Draw(str_[i]);
    }
}


ControlPanel::ControlPanel()
{
    panel_.SetImage(GET_IMG("WIP_score-board"));
    if (!font_.LoadFromFile("font/neogrey.otf", 12))
    {
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < STR_COUNT; ++i)
    {
        str_[i].SetSize(12);
        str_[i].SetColor(sf::Color::White);
        str_[i].SetFont(font_);
    }
    str_[HP].SetPosition(50, 5);
    str_[SHIELD].SetPosition(50, 25);
    str_[HEAT].SetPosition(200, 5);
    str_[INFO].SetPosition(200, 25);
    str_[INFO].SetColor(sf::Color::Red);
}


