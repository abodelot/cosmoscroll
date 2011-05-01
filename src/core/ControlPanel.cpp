#include "ControlPanel.hpp"
#include "entities/Bonus.hpp"
#include "utils/MediaManager.hpp"
#include "utils/StringUtils.hpp"
#include "utils/I18n.hpp"

#define BONUS_LENGTH 25   // longueur des icones bonus

#define PROG_BAR_WIDTH       100
#define PROG_BAR_HEIGHT      15
#define PROG_BAR_BG          sf::Color(64, 64, 64) // couleur de fond
#define PROG_BAR_TEXT_LENGTH 60   // longueur du texte

#define Y_LINE_1       10 // Y première ligne
#define Y_LINE_2       30 // Y deuxième ligne
#define TEXT_PADDING_Y 3  // décalage du texte en Y
#define TEXT_SIZE      15

#define LEVEL_BAR_X 425
#define LEVEL_BAR_Y 41


ControlPanel& ControlPanel::GetInstance()
{
	static ControlPanel self;
	return self;
}


ControlPanel::ControlPanel()
{
	panel_.SetImage(GET_IMG("gui/score-board"));

	const sf::Font& font = MediaManager::GetInstance().GetFixedFont();
	pbars_[ProgressBar::HP].Init(I18n::t("panel.bar_hp"), font, GET_IMG("gui/bar-hp"));
	pbars_[ProgressBar::HP].SetPosition(45, Y_LINE_1);

	pbars_[ProgressBar::SHIELD].Init(I18n::t("panel.bar_shield"), font, GET_IMG("gui/bar-shield"));
	pbars_[ProgressBar::SHIELD].SetPosition(45, Y_LINE_2);

	pbars_[ProgressBar::HEAT].Init(I18n::t("panel.bar_heat"), font, GET_IMG("gui/bar-heat"));
	pbars_[ProgressBar::HEAT].SetPosition(220, Y_LINE_1);

	sf::Vector2f pos = pbars_[ProgressBar::HEAT].bar_.GetPosition();
	info_.SetPosition(pos.x + 8, pos.y - TEXT_PADDING_Y);
	info_.SetFont(font);
	info_.SetSize(TEXT_SIZE);
	info_.SetColor(sf::Color::Red);

	coolers_.Init(Bonus::GetSubRect(Bonus::COOLER), 230, Y_LINE_2);
	coolers_.count.SetFont(font);
	missiles_.Init(Bonus::GetSubRect(Bonus::MISSILE), 300, Y_LINE_2);
	missiles_.count.SetFont(font);

	timer_.SetPosition(430, 12);
	timer_.SetFont(font);
	timer_.SetSize(TEXT_SIZE);
	timer_.SetColor(sf::Color::White);

	game_info_.SetPosition(530, 12);
	game_info_.SetFont(font);
	game_info_.SetSize(TEXT_SIZE);
	game_info_.SetColor(sf::Color::White);

	// story mode
	level_bar_.SetImage(GET_IMG("gui/level-bar"));
	level_bar_.SetPosition(LEVEL_BAR_X, LEVEL_BAR_Y);
	level_cursor_.SetImage(GET_IMG("gui/level-cursor"));
	level_cursor_.SetPosition(LEVEL_BAR_X, LEVEL_BAR_Y - 2);
	level_duration_ = 0;

	// arcade
	str_points_.SetPosition(530, 30);
	str_points_.SetFont(font);
	str_points_.SetSize(TEXT_SIZE);

}


void ControlPanel::Init(EntityManager::Mode mode)
{
	game_mode_ = mode;
	switch (mode)
	{
		case EntityManager::MODE_STORY:
			level_cursor_.SetX(LEVEL_BAR_X);
			break;
		case EntityManager::MODE_ARCADE:
			SetPoints(0);
			break;
	}
}


void ControlPanel::SetGameInfo(const sf::Unicode::Text& text)
{
	game_info_.SetText(text);
}


void ControlPanel::SetPoints(int points)
{
	str_points_.SetText(str_sprintf("points: %d", points));
}


void ControlPanel::SetOverheat(bool overheat)
{
	if (overheat)
	{
		info_.SetText(I18n::t("panel.overheat"));
	}
	else
	{
		info_.SetText("");
	}
}


void ControlPanel::SetShipHP(int value)
{
	pbars_[ProgressBar::HP].SetValue(value);
}


void ControlPanel::SetMaxShipHP(int max)
{
	pbars_[ProgressBar::HP].max_value_ = max;
}


void ControlPanel::SetShield(int value)
{
	pbars_[ProgressBar::SHIELD].SetValue(value);
}


void ControlPanel::SetMaxShield(int max)
{
	pbars_[ProgressBar::SHIELD].max_value_ = max;
}


void ControlPanel::SetHeat(int value)
{
	pbars_[ProgressBar::HEAT].SetValue(value);
}


void ControlPanel::SetMaxHeat(int max)
{
	pbars_[ProgressBar::HEAT].max_value_ = max;
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
		timer_.SetText(str_sprintf(I18n::t("panel.timer").c_str(), rounded / 60, rounded % 60));
		previous = rounded;
		if (game_mode_ == EntityManager::MODE_STORY)
		{
			int max_progress = level_bar_.GetSize().x - level_cursor_.GetSize().x;
			int progress = max_progress * rounded / level_duration_;
			int x = LEVEL_BAR_X + (progress > max_progress ? max_progress : progress);
			level_cursor_.SetX(x);
		}
	}
}


bool ControlPanel::IsOnTop() const
{
	return (int) GetPosition().y == 0;
}


void ControlPanel::SetLevelDuration(int seconds)
{
	level_duration_ = seconds;
}


void ControlPanel::Render(sf::RenderTarget& target) const
{
	target.Draw(panel_);
	for (int i = 0; i < ProgressBar::_PBAR_COUNT; ++i)
	{
		target.Draw(pbars_[i].label_);
		target.Draw(pbars_[i].background_);
		target.Draw(pbars_[i].bar_);
	}
	target.Draw(game_info_);
	target.Draw(info_);
	target.Draw(timer_);

	target.Draw(coolers_.icon);
	target.Draw(coolers_.count);
	target.Draw(missiles_.icon);
	target.Draw(missiles_.count);
	switch (game_mode_)
	{
		case EntityManager::MODE_STORY:
			target.Draw(level_bar_);
			target.Draw(level_cursor_);
			break;
		case EntityManager::MODE_ARCADE:
			target.Draw(str_points_);
			break;
	}
}


ControlPanel::ProgressBar::ProgressBar()
{
	background_ = sf::Shape::Rectangle(0, 0, PROG_BAR_WIDTH, PROG_BAR_HEIGHT, PROG_BAR_BG, 1.f);
	initial_x_ = 0;
}


void ControlPanel::ProgressBar::Init(const sf::Unicode::Text& text, const sf::Font& font, const sf::Image& img)
{
	label_.SetFont(font);
	label_.SetText(text);
	label_.SetSize(TEXT_SIZE);
	label_.SetColor(sf::Color::White);
	bar_.SetImage(img);

}


void ControlPanel::ProgressBar::SetPosition(int x, int y)
{
	label_.SetPosition(x, y - TEXT_PADDING_Y);
	int x_bar = x + PROG_BAR_TEXT_LENGTH;
	background_.SetPosition(x_bar, y);
	bar_.SetPosition(x_bar, y);
	initial_x_ = x_bar;
}


void ControlPanel::ProgressBar::SetValue(int value)
{
	value = value > 0 ? value : 0;
	float length = (float) value / max_value_ * PROG_BAR_WIDTH;
	if (length == 0.0f)
	{
		length = 0.1f;
	}

	bar_.Resize(length, PROG_BAR_HEIGHT);
	// It's magic, I ain't gonna explain shit.
	bar_.SetX(initial_x_ - length / 2.68);
}


void ControlPanel::BonusCount::Init(const sf::IntRect& subrect, int x, int y)
{
	icon.SetPosition(x, y);
	icon.SetImage(GET_IMG("entities/bonus"));
	icon.SetSubRect(subrect);
	count.SetPosition(x + BONUS_LENGTH, y);
	count.SetSize(TEXT_SIZE);
	count.SetColor(sf::Color::White);
	count.SetText("x 0");
}

