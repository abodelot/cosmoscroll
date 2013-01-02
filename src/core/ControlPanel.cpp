#include "ControlPanel.hpp"
#include "core/Resources.hpp"
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
	panel_.setTexture(Resources::getTexture("gui/score-board.png"));
	const sf::Font& font = Resources::getFont("Ubuntu-R.ttf");

	// init progress bar
	pbars_[ProgressBar::HP].Init(_t("panel.bar_hp"), font, BAR_SHIP);
	pbars_[ProgressBar::HP].setPosition(42, 7);

	pbars_[ProgressBar::SHIELD].Init(_t("panel.bar_shield"), font, BAR_SHIELD);
	pbars_[ProgressBar::SHIELD].setPosition(42, 22);

	pbars_[ProgressBar::HEAT].Init(_t("panel.bar_heat"), font, BAR_HEAT);
	pbars_[ProgressBar::HEAT].setPosition(42, 37);

	bar_mask_.setTexture(Resources::getTexture("gui/score-board-bar-mask.png"));
	bar_mask_.setPosition(101, 6);

	// init bonus counters
	bs_coolers_.Init(PowerUp::COOLER, PowerUpSlot::COUNTER);
	bs_coolers_.setPosition(256, 8);

	bs_missiles_.Init(PowerUp::MISSILE, PowerUpSlot::COUNTER);
	bs_missiles_.setPosition(256, 31);

	bs_attack_.Init(PowerUp::DOUBLE_SHOT, PowerUpSlot::TIMER);
	bs_attack_.setPosition(334, 8);

	bs_speed_.Init(PowerUp::SPEED, PowerUpSlot::TIMER);
	bs_speed_.setPosition(334, 31);

	// right container
	timer_.setPosition(430, 12);
	timer_.setFont(font);
	timer_.setCharacterSize(TEXT_SIZE);

	game_info_.setPosition(530, 12);
	game_info_.setFont(font);
	game_info_.setCharacterSize(TEXT_SIZE);

	str_points_.setPosition(530, 26);
	str_points_.setCharacterSize(TEXT_SIZE);
	str_points_.setFont(font);

	// story mode
	level_bar_.setTexture(Resources::getTexture("gui/level-bar.png"));
	level_bar_.setPosition(LEVEL_BAR_X, LEVEL_BAR_Y);
	level_cursor_.setTexture(Resources::getTexture("gui/level-cursor.png"));
	level_cursor_.setPosition(LEVEL_BAR_X, LEVEL_BAR_Y - 2);
	level_duration_ = 0;
}


void ControlPanel::Init(EntityManager::Mode mode)
{
	game_mode_ = mode;
	switch (mode)
	{
		case EntityManager::MODE_STORY:
			level_cursor_.setX(LEVEL_BAR_X);
			break;
		default:
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


void ControlPanel::SetGameInfo(const sf::String& text)
{
	game_info_.setString(text);
}


void ControlPanel::SetPoints(int points)
{
	str_points_.setString(wstr_replace(_t("panel.points"), L"{points}", to_wstring(points)));
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
		timer_.setString(text);
		previous = rounded;
		if (game_mode_ == EntityManager::MODE_STORY)
		{
			int max_progress = level_bar_.getWidth() - level_cursor_.getWidth();
			int progress = max_progress * rounded / level_duration_;
			int x = LEVEL_BAR_X + (progress > max_progress ? max_progress : progress);
			level_cursor_.setX(x);
		}
	}
}


bool ControlPanel::IsOnTop() const
{
	return (int) getPosition().y == 0;
}


void ControlPanel::SetLevelDuration(int seconds)
{
	level_duration_ = seconds > 0 ? seconds : 1;
}


void ControlPanel::SetOverheat(bool overheat)
{
	if (overheat)
	{
		pbars_[ProgressBar::HEAT].bar_.setFillColor(BAR_OVERHEAT);
		pbars_[ProgressBar::HEAT].label_.setColor(BAR_OVERHEAT);
	}
	else
	{
		pbars_[ProgressBar::HEAT].bar_.setFillColor(BAR_HEAT);
		pbars_[ProgressBar::HEAT].label_.setColor(sf::Color::White);
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


void ControlPanel::ActiveSpeedPowerUp(int seconds)
{
	bs_speed_.SetValue(seconds);
}


void ControlPanel::ActiveAttackPowerUp(int seconds, PowerUp::Type bonus_type)
{
	bs_attack_.Init(bonus_type, PowerUpSlot::TIMER);
	bs_attack_.SetValue(seconds);
}


void ControlPanel::RefreshTextTranslations()
{
	pbars_[ProgressBar::HP].label_.setString(_t("panel.bar_hp"));
	pbars_[ProgressBar::SHIELD].label_.setString(_t("panel.bar_shield"));
	pbars_[ProgressBar::HEAT].label_.setString(_t("panel.bar_heat"));
}


void ControlPanel::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	// background
	target.draw(panel_);

	// draw progress bars
	for (int i = 0; i < ProgressBar::_PBAR_COUNT; ++i)
	{
		target.draw(pbars_[i].label_);
		target.draw(pbars_[i].bar_);
		target.draw(pbars_[i].value_);
	}
	target.draw(bar_mask_);

	// draw bonus slots
	bs_coolers_.Show(target);
	bs_missiles_.Show(target);
	bs_attack_.Show(target);
	bs_speed_.Show(target);

	target.draw(game_info_);
	target.draw(timer_);
	target.draw(str_points_);

	if (game_mode_ == EntityManager::MODE_STORY)
	{
		target.draw(level_bar_);
		target.draw(level_cursor_);
	}
}

// ProgessBar -----------------------------------------------------------------

ControlPanel::ProgressBar::ProgressBar()
{
}


void ControlPanel::ProgressBar::Init(const sf::String& text, const sf::Font& font, const sf::Color& color)
{
	label_.setFont(font);
	label_.setString(text);
	label_.setCharacterSize(TEXT_SIZE);
	label_.setColor(sf::Color::White);
	bar_.setSize(sf::Vector2f(0, PROG_BAR_HEIGHT));
	bar_.setFillColor(color);
	value_.setFont(font);
	value_.setCharacterSize(TEXT_SIZE);
}


void ControlPanel::ProgressBar::setPosition(int x, int y)
{
	label_.setPosition(x, y - TEXT_PADDING_Y);
	int x_bar = x + PROG_BAR_TEXT_LENGTH;
	bar_.setPosition(x_bar, y);
	value_.setPosition(x_bar + 40, y - TEXT_PADDING_Y);
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
	bar_.setSize(sf::Vector2f(length, PROG_BAR_HEIGHT));

	// display {value}/{max_value}
	value_.setString(to_string(value) + "/" + to_string(max_value_));
	// center text on progress bar
	value_.setX(bar_.getPosition().x + (int) (PROG_BAR_WIDTH - value_.getWidth()) / 2);
}

// PowerUpSlot ------------------------------------------------------------------

void ControlPanel::PowerUpSlot::Init(PowerUp::Type bonus_type, Type type)
{
	icon_.setTexture(Resources::getTexture("entities/bonus.png"));
	icon_.setTextureRect(PowerUp::getTextureRect(bonus_type));

	label_.setCharacterSize(TEXT_SIZE);
	label_.setColor(sf::Color::White);
	label_.setString(type == COUNTER ? "x 0" : "-");
	label_.setFont(Resources::getFont("Ubuntu-R.ttf"));

	glow_.setTexture(Resources::getTexture("gui/bonus-glow.png"));
	glow_.setColor(sf::Color(255, 255, 255, 0));
	timer_ = -1.f;
	glowing_ = STOP;
	type_ = type;
}


void ControlPanel::PowerUpSlot::setPosition(int x, int y)
{
	icon_.setPosition(x, y);
	label_.setPosition(x + BONUS_LENGTH, y);
	// glow is 64x64, centered on bonus sprite
	glow_.setPosition(x - 24, y - 24);
}


void ControlPanel::PowerUpSlot::SetValue(int count)
{
	switch (type_)
	{
		case COUNTER:
			label_.setString("x " + to_string(count));
			timer_ = 0.f;
			glowing_ = UP;
			break;
		case TIMER:
			timer_ = count;
			label_.setString(to_string(count) + "s");
			glowing_ = UP;
			glow_.setColor(sf::Color::White);
			break;
	}
}


void ControlPanel::PowerUpSlot::Update(float frametime)
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
						glow_.setColor(sf::Color(255, 255, 255, 0));
						return;
					}

				}
				glow_.setColor(sf::Color(255, 255, 255, alpha));
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
					label_.setString(to_string(new_timer) + "s");
				}
				else if (timer_ <= 0.f)
				{
					glow_.setColor(sf::Color(255, 255, 255, 0));
					glowing_ = STOP;
					label_.setString("-");
				}
			}
			break;
	}
}


void ControlPanel::PowerUpSlot::Show(sf::RenderTarget& target) const
{
	target.draw(glow_);
	target.draw(icon_);
	target.draw(label_);
}

