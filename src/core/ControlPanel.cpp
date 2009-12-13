#include "ControlPanel.hpp"
#include "../entities/Bonus.hpp"
#include "../utils/MediaManager.hpp"
#include "../utils/StringUtils.hpp"

#define LABEL_LENGTH 60   // longueur du texte des progess bars
#define BONUS_LENGTH 25   // longueur des icones bonus
#define BAR_HEIGHT   15   // hauteur progress bar
#define BAR_COLOR    sf::Color(64, 64, 64) // couleur fond progress bar

#define Y_LINE_1       10 // Y première ligne
#define Y_LINE_2       30 // Y deuxième ligne
#define TEXT_PADDING_Y 3  // décalage du texte en Y

#define FONT_SIZE 15
#define FONT_FILENAME "data/font/digital-7.ttf"


ControlPanel& ControlPanel::GetInstance()
{
    static ControlPanel self;
    return self;
}


ControlPanel::ControlPanel()
{
	panel_.SetImage(GET_IMG("gui/score-board"));

	font_.LoadFromFile(FONT_FILENAME, FONT_SIZE);

	pbars_[HP].Init(font_, 45, Y_LINE_1);
	pbars_[HP].label.SetText("Coque");
	pbars_[HP].bar.SetImage(GET_IMG("gui/bar-hp"));
	pbars_[SHIELD].Init(font_, 45, Y_LINE_2);
	pbars_[SHIELD].label.SetText("Bouclier");
	pbars_[SHIELD].bar.SetImage(GET_IMG("gui/bar-shield"));
	pbars_[HEAT].Init(font_, 220, Y_LINE_1);
	pbars_[HEAT].label.SetText("Chaleur");
	pbars_[HEAT].bar.SetImage(GET_IMG("gui/bar-heat"));
	sf::Vector2f pos = pbars_[HEAT].bar.GetPosition();
	info_.SetPosition(pos.x + 8, pos.y - 6);
	info_.SetFont(font_);
	info_.SetSize(FONT_SIZE);
	info_.SetColor(sf::Color::Red);

	coolers_.Init(Bonus::GetSubRect(Bonus::COOLER), 230, Y_LINE_2);
	coolers_.count.SetFont(font_);
	missiles_.Init(Bonus::GetSubRect(Bonus::MISSILE), 300, Y_LINE_2);
	missiles_.count.SetFont(font_);

	timer_.SetPosition(480, Y_LINE_1 - TEXT_PADDING_Y);
	timer_.SetFont(font_);
	timer_.SetSize(FONT_SIZE);
	timer_.SetColor(sf::Color::White);

	game_info_.SetPosition(480, Y_LINE_2 - TEXT_PADDING_Y);
	game_info_.SetFont(font_);
	game_info_.SetSize(FONT_SIZE);
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
    background = sf::Shape::Rectangle(0, 0, 100, BAR_HEIGHT, BAR_COLOR, 1.f);
}


void ControlPanel::ProgressBar::Init(const sf::Font& font, float x, float y)
{
	label.SetFont(font);
	label.SetSize(FONT_SIZE);
	label.SetColor(sf::Color::White);
    label.SetPosition(x, y - TEXT_PADDING_Y);
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
	count.SetPosition(x + BONUS_LENGTH, y);
	count.SetSize(FONT_SIZE);
	count.SetColor(sf::Color::White);
	count.SetText("x 0");
}

