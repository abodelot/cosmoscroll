#include "ControlPanel.hpp"
#include "entities/Bonus.hpp"
#include "utils/MediaManager.hpp"
#include "utils/StringUtils.hpp"
#include "utils/I18n.hpp"

#define BONUS_LENGTH 25   // longueur des icones bonus

#define PROG_BAR_WIDTH       110
#define PROG_BAR_HEIGHT      10
#define PROG_BAR_BG          sf::Color(64, 64, 64) // couleur de fond
#define PROG_BAR_TEXT_LENGTH 60   // longueur du texte

#define Y_LINE_1       10 // Y première ligne
#define Y_LINE_2       30 // Y deuxième ligne
#define TEXT_PADDING_Y 3  // décalage du texte en Y
#define TEXT_SIZE      12

#define LEVEL_BAR_X 425
#define LEVEL_BAR_Y 41


#define BAR_SHIP   sf::Color(0xc6, 0x00, 0x00)
#define BAR_SHIELD sf::Color(0x06, 0xb1, 0xf1)
#define BAR_HEAT   sf::Color(0x88, 0xff, 0x00)


ControlPanel& ControlPanel::GetInstance()
{
	static ControlPanel self;
	return self;
}


ControlPanel::ControlPanel()
{
	panel_.SetImage(GET_IMG("gui/score-board"));

	const sf::Font& font = MediaManager::GetInstance().GetFixedFont();
	pbars_[ProgressBar::HP].Init(_t("panel.bar_hp"), font, BAR_SHIP);
	pbars_[ProgressBar::HP].SetPosition(42, 7);

	pbars_[ProgressBar::SHIELD].Init(_t("panel.bar_shield"), font, BAR_SHIELD);
	pbars_[ProgressBar::SHIELD].SetPosition(42, 22);

	pbars_[ProgressBar::HEAT].Init(_t("panel.bar_heat"), font, BAR_HEAT);
	pbars_[ProgressBar::HEAT].SetPosition(42, 37);

	sf::Vector2f pos = pbars_[ProgressBar::HEAT].bar_.GetPosition();
	info_.SetPosition(pos.x + 8, pos.y - TEXT_PADDING_Y);
	info_.SetFont(font);
	info_.SetSize(TEXT_SIZE);
	info_.SetColor(sf::Color::Red);

	// init bonus counters
	coolers_.Init(Bonus::GetSubRect(Bonus::COOLER), 328, 8);
	missiles_.Init(Bonus::GetSubRect(Bonus::MISSILE), 328, 31);

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

	bar_mask_.SetImage(GET_IMG("gui/score-board-bar-mask"));
	bar_mask_.SetPosition(101, 6);
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


void ControlPanel::Update(float frametime)
{
	missiles_.Update(frametime);
	coolers_.Update(frametime);
}


void ControlPanel::SetGameInfo(const sf::Unicode::Text& text)
{
	game_info_.SetText(text);
}


void ControlPanel::SetPoints(int points)
{
	char text[32];
	sprintf(text, "points: %d", points);
	str_points_.SetText(text);
}


void ControlPanel::SetOverheat(bool overheat)
{
	if (overheat)
	{
		info_.SetText(_t("panel.overheat"));
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


void ControlPanel::SetCoolers(int count)
{
	coolers_.SetValue(count);
}


void ControlPanel::SetMissiles(int count)
{
	missiles_.SetValue(count);
}


void ControlPanel::SetTimer(float seconds)
{
	static int previous = -1; // negative, to force update at first call
	int rounded = (int) seconds;
	if (rounded != previous)
	{
		std::wstring text = _t("panel.timer");
		wstr_self_replace(text, L"{min}", to_wstring(rounded / 60, 2));
		wstr_self_replace(text, L"{sec}", to_wstring(rounded % 60, 2));
		timer_.SetText(text);
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
	// background
	target.Draw(panel_);

	// draw bonus counters
	coolers_.Show(target);
	missiles_.Show(target);

	// progress bars
	for (int i = 0; i < ProgressBar::_PBAR_COUNT; ++i)
	{
		target.Draw(pbars_[i].label_);
		target.Draw(pbars_[i].bar_);
	}
	target.Draw(bar_mask_);
	target.Draw(game_info_);
	target.Draw(info_);
	target.Draw(timer_);


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
	//background_ = sf::Shape::Rectangle(0, 0, PROG_BAR_WIDTH, PROG_BAR_HEIGHT, PROG_BAR_BG, 1.f);
	initial_x_ = 0;
}


void ControlPanel::ProgressBar::Init(const sf::Unicode::Text& text, const sf::Font& font, const sf::Color& color)
{
	label_.SetFont(font);
	label_.SetText(text);
	label_.SetSize(TEXT_SIZE);
	label_.SetColor(sf::Color::White);
	bar_.Resize(0, PROG_BAR_HEIGHT);
	bar_.SetColor(color);
}


void ControlPanel::ProgressBar::SetPosition(int x, int y)
{
	label_.SetPosition(x, y - TEXT_PADDING_Y);
	int x_bar = x + PROG_BAR_TEXT_LENGTH;
	//background_.SetPosition(x_bar, y);
	bar_.SetPosition(x_bar, y);
	initial_x_ = x_bar;
}


void ControlPanel::ProgressBar::SetValue(int value)
{
	value = value > 0 ? value : 0;
	float length = (float) value / max_value_ * (PROG_BAR_WIDTH - 1);
	if (length == 0.0f)
	{
		length = 0.1f;
	}

	bar_.Resize(length, PROG_BAR_HEIGHT);
	// It's magic, I ain't gonna explain shit.
	//bar_.SetX(initial_x_ - length / 2.68);
}


void ControlPanel::BonusCounter::Init(const sf::IntRect& subrect, int x, int y)
{
	icon_.SetPosition(x, y);
	icon_.SetImage(GET_IMG("entities/bonus"));
	icon_.SetSubRect(subrect);

	count_.SetPosition(x + BONUS_LENGTH, y);
	count_.SetSize(TEXT_SIZE);
	count_.SetColor(sf::Color::White);
	count_.SetText("x 0");
	count_.SetFont(MediaManager::GetInstance().GetFixedFont());

	// glow is 64x64, centered on bonus sprite
	glow_.SetPosition(x - 24, y - 24);
	glow_.SetImage(GET_IMG("gui/bonus-glow"));
	glow_.SetColor(sf::Color(255, 255, 255, 0));
	timer_ = -1.f;
	glowing_ = STOP;
}


void ControlPanel::BonusCounter::SetValue(int count)
{
	count_.SetText("x " + to_string(count));
	timer_ = 0.f;
	glowing_ = UP;
}


void ControlPanel::BonusCounter::Update(float frametime)
{
	if (glowing_ != STOP)
	{
		const float DELAY = 1.f;
		timer_ += frametime;
		int alpha = 255 * timer_ / DELAY;
		if (glowing_ == UP && timer_ >= DELAY)
		{
			glowing_ = DOWN;
			timer_ = 0.f;
			alpha = 255;
		}
		else if (glowing_ == DOWN)
		{
			alpha = 255 - alpha;
			if (timer_ >= DELAY)
			{
				glowing_ = STOP;
				glow_.SetColor(sf::Color(255, 255, 255, 0));
				return;
			}

		}
		glow_.SetColor(sf::Color(255, 255, 255, alpha));
	}
}


void ControlPanel::BonusCounter::Show(sf::RenderTarget& target) const
{
	target.Draw(glow_);
	target.Draw(icon_);
	target.Draw(count_);
}

