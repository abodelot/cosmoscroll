#include "ControlPanel.hpp"
#include "utils/MediaManager.hpp"
#include "utils/StringUtils.hpp"
#include "utils/I18n.hpp"

#define BONUS_LENGTH 25   // offset label bonus slot

#define PROG_BAR_WIDTH       110
#define PROG_BAR_HEIGHT      10
#define PROG_BAR_TEXT_LENGTH 60   // longueur du texte

#define Y_LINE_1       10 // Y première ligne
#define Y_LINE_2       30 // Y deuxième ligne
#define TEXT_PADDING_Y 3  // décalage du texte en Y
#define TEXT_SIZE      12

#define LEVEL_BAR_X 425
#define LEVEL_BAR_Y 41

#define BAR_SHIP   sf::Color(0xc6, 0x00, 0x00)
#define BAR_SHIELD sf::Color(0x00, 0x80, 0xe0)
#define BAR_HEAT   sf::Color(0x44, 0xc0, 0x00)
#define BAR_OVERHEAT sf::Color(0xff, 0x88, 0x00)

ControlPanel& ControlPanel::GetInstance()
{
	static ControlPanel self;
	return self;
}


ControlPanel::ControlPanel()
{
	panel_.SetImage(GET_IMG("gui/score-board"));
	const sf::Font& font = MediaManager::GetFont("Ubuntu-R.ttf", TEXT_SIZE);

	// init progress bar
	pbars_[ProgressBar::HP].Init(_t("panel.bar_hp"), font, BAR_SHIP);
	pbars_[ProgressBar::HP].SetPosition(42, 7);

	pbars_[ProgressBar::SHIELD].Init(_t("panel.bar_shield"), font, BAR_SHIELD);
	pbars_[ProgressBar::SHIELD].SetPosition(42, 22);

	pbars_[ProgressBar::HEAT].Init(_t("panel.bar_heat"), font, BAR_HEAT);
	pbars_[ProgressBar::HEAT].SetPosition(42, 37);

	bar_mask_.SetImage(GET_IMG("gui/score-board-bar-mask"));
	bar_mask_.SetPosition(101, 6);

	// init bonus counters
	bs_coolers_.Init(Bonus::COOLER, BonusSlot::COUNTER);
	bs_coolers_.SetPosition(256, 8);

	bs_missiles_.Init(Bonus::MISSILE, BonusSlot::COUNTER);
	bs_missiles_.SetPosition(256, 31);

	bs_attack_.Init(Bonus::DOUBLE_SHOT, BonusSlot::TIMER);
	bs_attack_.SetPosition(334, 8);

	bs_speed_.Init(Bonus::SPEED, BonusSlot::TIMER);
	bs_speed_.SetPosition(334, 31);

	// right container
	timer_.SetPosition(430, 12);
	timer_.SetFont(font);
	timer_.SetSize(TEXT_SIZE);

	game_info_.SetPosition(530, 12);
	game_info_.SetFont(font);
	game_info_.SetSize(TEXT_SIZE);

	str_points_.SetPosition(530, 26);
	str_points_.SetSize(TEXT_SIZE);
	str_points_.SetFont(font);

	// story mode
	level_bar_.SetImage(GET_IMG("gui/level-bar"));
	level_bar_.SetPosition(LEVEL_BAR_X, LEVEL_BAR_Y);
	level_cursor_.SetImage(GET_IMG("gui/level-cursor"));
	level_cursor_.SetPosition(LEVEL_BAR_X, LEVEL_BAR_Y - 2);
	level_duration_ = 0;
}


void ControlPanel::Init(EntityManager::Mode mode)
{
	game_mode_ = mode;
	switch (mode)
	{
		case EntityManager::MODE_STORY:
			level_cursor_.SetX(LEVEL_BAR_X);
			break;
	}
	SetPoints(0);
}


void ControlPanel::Update(float frametime)
{
	bs_missiles_.Update(frametime);
	bs_coolers_.Update(frametime);
	bs_speed_.Update(frametime);
	bs_attack_.Update(frametime);
}


void ControlPanel::SetGameInfo(const sf::Unicode::Text& text)
{
	game_info_.SetText(text);
}


void ControlPanel::SetPoints(int points)
{
	str_points_.SetText(wstr_replace(_t("panel.points"), L"{points}", to_wstring(points)));
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


void ControlPanel::SetOverheat(bool overheat)
{
	if (overheat)
	{
		pbars_[ProgressBar::HEAT].bar_.SetColor(BAR_OVERHEAT);
		pbars_[ProgressBar::HEAT].label_.SetColor(BAR_OVERHEAT);
	}
	else
	{
		pbars_[ProgressBar::HEAT].bar_.SetColor(BAR_HEAT);
		pbars_[ProgressBar::HEAT].label_.SetColor(sf::Color::White);
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
	bs_coolers_.SetValue(count);
}


void ControlPanel::SetMissiles(int count)
{
	bs_missiles_.SetValue(count);
}


void ControlPanel::ActiveSpeedBonus(int seconds)
{
	bs_speed_.SetValue(seconds);
}


void ControlPanel::ActiveAttackBonus(int seconds, Bonus::Type bonus_type)
{
	bs_attack_.Init(bonus_type, BonusSlot::TIMER);
	bs_attack_.SetValue(seconds);
}


void ControlPanel::Render(sf::RenderTarget& target) const
{
	// background
	target.Draw(panel_);

	// draw progress bars
	for (int i = 0; i < ProgressBar::_PBAR_COUNT; ++i)
	{
		target.Draw(pbars_[i].label_);
		target.Draw(pbars_[i].bar_);
		target.Draw(pbars_[i].value_);
	}
	target.Draw(bar_mask_);

	// draw bonus slots
	bs_coolers_.Show(target);
	bs_missiles_.Show(target);
	bs_attack_.Show(target);
	bs_speed_.Show(target);

	target.Draw(game_info_);
	target.Draw(timer_);
	target.Draw(str_points_);

	switch (game_mode_)
	{
		case EntityManager::MODE_STORY:
			target.Draw(level_bar_);
			target.Draw(level_cursor_);
			break;
	}
}

// ProgessBar -----------------------------------------------------------------

ControlPanel::ProgressBar::ProgressBar()
{
}


void ControlPanel::ProgressBar::Init(const sf::Unicode::Text& text, const sf::Font& font, const sf::Color& color)
{
	label_.SetFont(font);
	label_.SetText(text);
	label_.SetSize(TEXT_SIZE);
	label_.SetColor(sf::Color::White);
	bar_ = sf::Shape::Rectangle(0, 0, 0, PROG_BAR_HEIGHT, sf::Color::White);
	bar_.SetColor(color);
	value_.SetFont(font);
	value_.SetSize(TEXT_SIZE);
}


void ControlPanel::ProgressBar::SetPosition(int x, int y)
{
	label_.SetPosition(x, y - TEXT_PADDING_Y);
	int x_bar = x + PROG_BAR_TEXT_LENGTH;
	bar_.SetPosition(x_bar, y);
	value_.SetPosition(x_bar + 40, y - TEXT_PADDING_Y);
}


void ControlPanel::ProgressBar::SetValue(int value)
{
	// resize progress bar
	value = value > 0 ? value : 0;
	float length = (float) value / max_value_ * (PROG_BAR_WIDTH - 1);
	if (length == 0.0f)
	{
		length = 0.1f;
	}
	bar_.SetPointPosition(1, length, 0);
	bar_.SetPointPosition(2, length, PROG_BAR_HEIGHT);

	// display {value}/{max_value}
	value_.SetText(to_string(value) + "/" + to_string(max_value_));
	// center text on progress bar
	value_.SetX(bar_.GetPosition().x + (int) (PROG_BAR_WIDTH - value_.GetRect().GetWidth()) / 2);
}

// BonusSlot ------------------------------------------------------------------

void ControlPanel::BonusSlot::Init(Bonus::Type bonus_type, Type type)
{
	icon_.SetImage(GET_IMG("entities/bonus"));
	icon_.SetSubRect(Bonus::GetSubRect(bonus_type));

	label_.SetSize(TEXT_SIZE);
	label_.SetColor(sf::Color::White);
	label_.SetText(type == COUNTER ? "x 0" : "-");
	label_.SetFont(MediaManager::GetFont("Ubuntu-R.ttf", 12));

	glow_.SetImage(GET_IMG("gui/bonus-glow"));
	glow_.SetColor(sf::Color(255, 255, 255, 0));
	timer_ = -1.f;
	glowing_ = STOP;
	type_ = type;
}


void ControlPanel::BonusSlot::SetPosition(int x, int y)
{
	icon_.SetPosition(x, y);
	label_.SetPosition(x + BONUS_LENGTH, y);
	// glow is 64x64, centered on bonus sprite
	glow_.SetPosition(x - 24, y - 24);
}


void ControlPanel::BonusSlot::SetValue(int count)
{
	switch (type_)
	{
		case COUNTER:
			label_.SetText("x " + to_string(count));
			timer_ = 0.f;
			glowing_ = UP;
			break;
		case TIMER:
			timer_ = count;
			label_.SetText(to_string(count) + "s");
			glowing_ = UP;
			glow_.SetColor(sf::Color::White);
			break;
	}
}


void ControlPanel::BonusSlot::Update(float frametime)
{
	switch (type_)
	{
		case COUNTER:
			if (glowing_ != STOP)
			{
				const float DELAY = 1.5f;
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
			break;
		case TIMER:
			if (glowing_ == UP)
			{
				int old_timer = (int) (timer_ + 0.5f);
				timer_ -= frametime;
				int new_timer = (int) (timer_ + 0.5f);
				if (new_timer != old_timer)
				{
					label_.SetText(to_string(new_timer) + "s");
				}
				else if (timer_ <= 0.f)
				{
					glow_.SetColor(sf::Color(255, 255, 255, 0));
					glowing_ = STOP;
					label_.SetText("-");
				}
			}
			break;
	}
}


void ControlPanel::BonusSlot::Show(sf::RenderTarget& target) const
{
	target.Draw(glow_);
	target.Draw(icon_);
	target.Draw(label_);
}

