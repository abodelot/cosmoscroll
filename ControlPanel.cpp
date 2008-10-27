#include "ControlPanel.hpp"
#include "MediaManager.hpp"
#include "Misc.hpp"


ControlPanel& ControlPanel::GetInstance()
{
    static ControlPanel self;
    return self;
}


void ControlPanel::SetShipHP(int hp)
{
    str_[HP].SetText(str_sprintf("Vaisseau : %d HP", hp));
}


void ControlPanel::SetShield(int n)
{
    str_[SHIELD].SetText(str_sprintf("Bouclier : %d", n));    
}


void ControlPanel::SetInfo(const char* text)
{
    str_[INFO].SetText(text);
}


void ControlPanel::SetHeat(float heat)
{
    str_[HEAT].SetText(str_sprintf("Chaleur : %02.0f %%", heat));
}


void ControlPanel::SetChrono(float seconds)
{
    int s = (int) seconds; // arrondi
    str_[CHRONO].SetText(str_sprintf("Temps : %02d:%02d", s / 60, s % 60));
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
    panel_.SetImage(GET_IMG("score-board"));
    for (int i = 0; i < STR_COUNT; ++i)
    {
        str_[i].SetFont(GET_FONT());
        str_[i].SetSize(16);
        str_[i].SetColor(sf::Color::White);
    }
    str_[HP].SetPosition(50, 2);
    str_[SHIELD].SetPosition(50, 20);
    str_[HEAT].SetPosition(200, 2);
    str_[INFO].SetPosition(200, 20);
    str_[INFO].SetColor(sf::Color(255, 128 ,0));
    str_[CHRONO].SetPosition(400, 2);
}


