#include "ControlPanel.hpp"
#include "../entities/Bonus.hpp"
#include "../utils/MediaManager.hpp"
#include "../utils/StringUtils.hpp"

#define LABEL_LENGTH 60
#define BAR_HEIGHT   15

#define FONT_1 18
#define FONT_2 26


ControlPanel& ControlPanel::GetInstance()
{
    static ControlPanel self;
    return self;
}


ControlPanel::ControlPanel()
{
	panel_.SetImage(GET_IMG("gui/score-board"));

	font_.LoadFromFile("data/font/visitor2.ttf", FONT_1);
	font_big_.LoadFromFile("data/font/visitor2.ttf", FONT_2);

	pbars_[HP].Init(font_, 45, 10);
	pbars_[HP].label.SetText("Coque");
	pbars_[HP].bar.SetImage(GET_IMG("gui/bar-hp"));
	pbars_[SHIELD].Init(font_, 45, 30);
	pbars_[SHIELD].label.SetText("Bouclier");
	pbars_[SHIELD].bar.SetImage(GET_IMG("gui/bar-shield"));
	pbars_[HEAT].Init(font_, 220, 10);
	pbars_[HEAT].label.SetText("Chaleur");
	pbars_[HEAT].bar.SetImage(GET_IMG("gui/bar-heat"));
	sf::Vector2f pos = pbars_[HEAT].bar.GetPosition();
	info_.SetPosition(pos.x + 8, pos.y - 6);
	info_.SetFont(font_);
	info_.SetSize(FONT_1);
	info_.SetColor(sf::Color::Red);

	coolers_.Init(Bonus::GetSubRect(Bonus::COOLER), 230, 30);
	coolers_.count.SetFont(font_);
	missiles_.Init(Bonus::GetSubRect(Bonus::MISSILE), 300, 30);
	missiles_.count.SetFont(font_);

	timer_.SetPosition(450, 0);
	timer_.SetFont(font_big_);
	timer_.SetSize(FONT_2);
	timer_.SetColor(sf::Color::White);

	game_info_.SetPosition(450, 18);
	game_info_.SetFont(font_big_);
	game_info_.SetSize(FONT_2);
	game_info_.SetColor(sf::Color::White);
}


void ControlPanel::SetGameInfo(const sf::Unicode::Text& text)
{
	game_info_.SetText(text);
}


void ControlPanel::SetOverheatText(const sf::Unicode::Text& text)
{
	info_.SetText(text);
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


void ControlPanel::SetCoolers(int coolers)
{
	coolers_.count.SetText(str_sprintf("x %d", coolers));
}


void ControlPanel::SetMissiles(int count)
{
	missiles_.count.SetText(str_sprintf("x %d", count));
}


void ControlPanel::SetTimer(float seconds)
{
    static int previous = -1; // negative, to force update at first call
    int rounded = (int) seconds;
    if (rounded != previous)
    {
		timer_.SetText(str_sprintf("Temps : %02d:%02d", rounded / 60, rounded % 60));
		previous = rounded;
    }
}


void ControlPanel::Render(sf::RenderTarget& target) const
{
    target.Draw(panel_);
    for (int i = 0; i < PBAR_COUNT; ++i)
    {
        target.Draw(pbars_[i].label);
        target.Draw(pbars_[i].background);
        target.Draw(pbars_[i].bar);
    }
	target.Draw(game_info_);
    target.Draw(info_);
    target.Draw(timer_);

    target.Draw(coolers_.icon);
    target.Draw(coolers_.count);
    target.Draw(missiles_.icon);
    target.Draw(missiles_.count);
}


ControlPanel::ProgressBar::ProgressBar()
{
    background = sf::Shape::Rectangle(0, 0, 100, BAR_HEIGHT, sf::Color(64, 64, 64), 1.f);
}


void ControlPanel::ProgressBar::Init(const sf::Font& font, float x, float y)
{
	label.SetFont(font);
	label.SetSize(FONT_1);
	label.SetColor(sf::Color::White);
    label.SetPosition(x, y - 6);
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


void ControlPanel::BonusCount::Init(const sf::IntRect& subrect, int x, int y)
{
	icon.SetPosition(x, y);
	icon.SetImage(GET_IMG("bonus"));
	icon.SetSubRect(subrect);
	count.SetPosition(x + 20, y - 6);
	count.SetSize(FONT_1);
	count.SetColor(sf::Color::White);
	count.SetText("x 0");
}

