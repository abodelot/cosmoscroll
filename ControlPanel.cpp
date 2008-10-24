#include "ControlPanel.hpp"
#include "MediaManager.hpp"

#include <cstdarg>

// <3 printf format
static std::string epic_sprintf(const char format[], ...)
{
    va_list args;
    va_start(args, format);
    
    int length = vsnprintf(NULL, 0, format, args);
    char* p = new char [length + 1];
    va_end(args);
    va_start(args, format);
    vsprintf(p, format, args);
    va_end(args);    
    
    std::string str(p);
    delete [] p;
    return str;
}


ControlPanel& ControlPanel::GetInstance()
{
    static ControlPanel self;
    return self;
}


void ControlPanel::SetShipHP(int hp)
{
    str_[HP].SetText(epic_sprintf("Vaisseau : %d HP", hp));
}


void ControlPanel::SetShield(int n)
{
    str_[SHIELD].SetText(epic_sprintf("Bouclier : %d", n));    
}


void ControlPanel::SetInfo(const char* text)
{
    str_[INFO].SetText(text);
}


void ControlPanel::SetHeat(float heat)
{
    str_[HEAT].SetText(epic_sprintf("Chaleur : %02.0f %%", heat));
}


void ControlPanel::SetChrono(int seconds)
{
    str_[CHRONO].SetText(epic_sprintf("Temps : %02d:%02d", seconds / 60, seconds % 60));
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
    str_[INFO].SetColor(sf::Color::Red);
    str_[CHRONO].SetPosition(400, 2);
}


