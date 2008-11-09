#include "ControlPanel.hpp"
#include "MediaManager.hpp"
#include "Misc.hpp"


ControlPanel& ControlPanel::GetInstance()
{
    static ControlPanel self;
    return self;
}


void ControlPanel::SetShipHP(int value)
{
    pbars_[HP].SetPercent(value);
}


void ControlPanel::SetMaxShipHP(int max)
{
    pbars_[HP].max_value = max;
}


void ControlPanel::SetShield(int value)
{
    pbars_[SHIELD].SetPercent(value);
}


void ControlPanel::SetMaxShield(int max)
{
    pbars_[SHIELD].max_value = max;
}


void ControlPanel::SetHeat(int value)
{
    pbars_[HEAT].SetPercent(value);
}


void ControlPanel::SetMaxHeat(int max)
{
    pbars_[HEAT].max_value = max;
}


void ControlPanel::SetInfo(const char* text)
{
    info_.SetText(text);
}


void ControlPanel::SetCoolers(int coolers)
{
	coolers_.SetText(str_sprintf("Glaçons : %d", coolers));
}


void ControlPanel::SetTimer(float seconds)
{
    int s = (int) seconds; // arrondi
    timer_.SetText(str_sprintf("Temps : %02d:%02d", s / 60, s % 60));
}


void ControlPanel::Show(sf::RenderWindow& app) const
{
    app.Draw(panel_);
    for (int i = 0; i < PBAR_COUNT; ++i)
    {
        app.Draw(pbars_[i].label);
        app.Draw(pbars_[i].background);
        app.Draw(pbars_[i].bar);
    }
    app.Draw(info_);
    app.Draw(timer_);
    app.Draw(coolers_);
}


ControlPanel::ControlPanel()
{
    panel_.SetImage(GET_IMG("score-board"));
    
    pbars_[HP].SetPosition(50, 10);
    pbars_[HP].label.SetText("Coque");
    pbars_[HP].bar.SetImage(GET_IMG("bar_hp"));
    pbars_[SHIELD].SetPosition(50, 30);
    pbars_[SHIELD].label.SetText("Bouclier");
    pbars_[SHIELD].bar.SetImage(GET_IMG("bar_shield"));
    pbars_[HEAT].SetPosition(220, 10);
    pbars_[HEAT].label.SetText("Chaleur");
    pbars_[HEAT].bar.SetImage(GET_IMG("bar_heat"));
    
    timer_.SetPosition(400, 2);
    timer_.SetSize(14);
    timer_.SetColor(sf::Color::White);
    
    info_.SetPosition(400, 16);
    info_.SetSize(14);
    info_.SetColor(sf::Color(255, 128 ,0));
    
    coolers_.SetPosition(400, 30);
    coolers_.SetSize(14);
    coolers_.SetColor(sf::Color::White);
}

#define LABEL_LENGTH 55
#define BAR_HEIGHT 15

ControlPanel::ProgressBar::ProgressBar()
{
    background = sf::Shape::Rectangle(0, 0, 100, BAR_HEIGHT, sf::Color(64, 64, 64), 1.f);
}


void ControlPanel::ProgressBar::SetPosition(float x, float y)
{
	label.SetSize(12);
	label.SetColor(sf::Color::White);
    label.SetPosition(x, y);
    background.SetPosition(x + LABEL_LENGTH, y);
    bar.SetPosition(x + LABEL_LENGTH, y);
}


void ControlPanel::ProgressBar::SetPercent(int value)
{
	float length = (float) value / max_value * 100;
	if (length == 0.0f)
	{
		length = 0.1f;
	}
    bar.Resize(length, BAR_HEIGHT);
}


