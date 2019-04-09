#include <string>

#include "ControlPanel.hpp"
#include "core/Resources.hpp"
#include "utils/StringUtils.hpp"
#include "utils/SFML_Helper.hpp"
#include "utils/I18n.hpp"

#define BONUS_LENGTH 25   // offset label bonus slot

#define PROG_BAR_WIDTH       110
#define PROG_BAR_HEIGHT      10
#define PROG_BAR_TEXT_LENGTH 60   // label length (pixels) on progress bars

#define TEXT_SIZE      12

#define LEVEL_BAR_X 420
#define LEVEL_BAR_Y 22

#define BAR_SHIP   sf::Color(0xc6, 0x00, 0x00)
#define BAR_SHIELD sf::Color(0x00, 0x80, 0xe0)
#define BAR_HEAT   sf::Color(0x44, 0xc0, 0x00)
#define BAR_OVERHEAT sf::Color(0xff, 0x88, 0x00)

ControlPanel& ControlPanel::getInstance()
{
    static ControlPanel self;
    return self;
}


ControlPanel::ControlPanel():
    m_background(Resources::getTexture("gui/score-board.png"))
{
    const sf::Font& font = Resources::getFont("Vera.ttf");

    // init progress bar
    pbars_[ProgressBar::HP].init(_t("panel.bar_hp"), font, BAR_SHIP);
    pbars_[ProgressBar::HP].setPosition(42, 7);

    pbars_[ProgressBar::SHIELD].init(_t("panel.bar_shield"), font, BAR_SHIELD);
    pbars_[ProgressBar::SHIELD].setPosition(42, 22);

    pbars_[ProgressBar::HEAT].init(_t("panel.bar_heat"), font, BAR_HEAT);
    pbars_[ProgressBar::HEAT].setPosition(42, 37);

    bar_mask_.setTexture(Resources::getTexture("gui/score-board-bar-mask.png"));
    bar_mask_.setPosition(101, 6);

    // init bonus counters
    bs_coolers_.init(PowerUp::ICECUBE, PowerUpSlot::COUNTER, font);
    bs_coolers_.setPosition(256, 8);

    bs_missiles_.init(PowerUp::MISSILE, PowerUpSlot::COUNTER, font);
    bs_missiles_.setPosition(256, 31);

    bs_attack_.init(PowerUp::DOUBLE_SHOT, PowerUpSlot::TIMER, font);
    bs_attack_.setPosition(334, 8);

    bs_speed_.init(PowerUp::SPEED, PowerUpSlot::TIMER, font);
    bs_speed_.setPosition(334, 31);

    // right container
    timer_.setPosition(480, 6);
    timer_.setFont(font);
    timer_.setCharacterSize(TEXT_SIZE);

    game_info_.setPosition(450, 33);
    game_info_.setFont(font);
    game_info_.setCharacterSize(TEXT_SIZE);

    str_points_.setPosition(530, 33);
    str_points_.setCharacterSize(TEXT_SIZE);
    str_points_.setFont(font);

    // story mode
    level_bar_.setTexture(Resources::getTexture("gui/level-bar.png"));
    level_bar_.setPosition(LEVEL_BAR_X, LEVEL_BAR_Y);
    level_cursor_.setTexture(Resources::getTexture("gui/level-cursor.png"));
    level_cursor_.setPosition(LEVEL_BAR_X, LEVEL_BAR_Y);
    level_duration_ = 0;
}


void ControlPanel::setMode(EntityManager::Mode mode)
{
    game_mode_ = mode;
    switch (mode)
    {
        case EntityManager::LEVELS_MODE:
            sfh::setX(level_cursor_, LEVEL_BAR_X);
            break;
        default:
            break;
    }
    setScore(0);
}


void ControlPanel::update(float frametime)
{
    bs_missiles_.update(frametime);
    bs_coolers_.update(frametime);
    bs_speed_.update(frametime);
    bs_attack_.update(frametime);
}


void ControlPanel::setGameInfo(const sf::String& text)
{
    game_info_.setString(text);
}


void ControlPanel::setScore(int score)
{
    str_points_.setString(I18n::templatize("panel.points", "{points}", score));
}


void ControlPanel::setHighscore(int highscore)
{
    game_info_.setString(I18n::templatize("panel.record", "{record}", highscore));
}


void ControlPanel::setElapsedTime(float seconds)
{
    static int previous = -1; // < 0, to force update at first call
    int rounded = (int) seconds;
    // Update every second
    if (rounded != previous)
    {
        std::wstring text = _t("panel.timer");
        std::wstring min = std::to_wstring(rounded / 60);
        std::wstring sec = std::to_wstring(rounded % 60);
        // Format on 2 digits
        wstr_self_replace(text, L"{min}", min.size() > 1 ? min : L"0" + min);
        wstr_self_replace(text, L"{sec}", sec.size() > 1 ? sec : L"0" + sec);
        timer_.setString(text);
        previous = rounded;
        if (game_mode_ == EntityManager::LEVELS_MODE)
        {
            int max_progress = sfh::width(level_bar_) - sfh::width(level_cursor_);
            int progress = max_progress * rounded / level_duration_;
            int x = LEVEL_BAR_X + (progress > max_progress ? max_progress : progress);
            sfh::setX(level_cursor_, x);
        }
    }
}


void ControlPanel::setLevelDuration(int seconds)
{
    level_duration_ = seconds > 0 ? seconds : 1;
}


void ControlPanel::setOverheat(bool overheat)
{
    if (overheat)
    {
        pbars_[ProgressBar::HEAT].bar_.setFillColor(BAR_OVERHEAT);
        pbars_[ProgressBar::HEAT].label_.setFillColor(BAR_OVERHEAT);
    }
    else
    {
        pbars_[ProgressBar::HEAT].bar_.setFillColor(BAR_HEAT);
        pbars_[ProgressBar::HEAT].label_.setFillColor(sf::Color::White);
    }
}


void ControlPanel::setHP(int value)
{
    pbars_[ProgressBar::HP].setValue(value);
}


void ControlPanel::setMaxHP(int max)
{
    pbars_[ProgressBar::HP].max_value_ = max;
}


void ControlPanel::setShield(int value)
{
    pbars_[ProgressBar::SHIELD].setValue(value);
}


void ControlPanel::setMaxShield(int max)
{
    pbars_[ProgressBar::SHIELD].max_value_ = max;
}


void ControlPanel::setHeat(float value)
{
    pbars_[ProgressBar::HEAT].setValue(value);
}


void ControlPanel::setMaxHeat(float max)
{
    pbars_[ProgressBar::HEAT].max_value_ = max;
}


void ControlPanel::setIcecubes(int count)
{
    bs_coolers_.setValue(count);
}


void ControlPanel::setMissiles(int count)
{
    bs_missiles_.setValue(count);
}


void ControlPanel::activeSpeedPowerUp(int seconds)
{
    bs_speed_.setValue(seconds);
}


void ControlPanel::activeAttackPowerUp(int seconds, PowerUp::Type bonus_type)
{
    bs_attack_.icon_.setTextureRect(PowerUp::getTextureRect(bonus_type));
    bs_attack_.setValue(seconds);
}


void ControlPanel::refreshTextTranslations()
{
    pbars_[ProgressBar::HP].label_.setString(_t("panel.bar_hp"));
    pbars_[ProgressBar::SHIELD].label_.setString(_t("panel.bar_shield"));
    pbars_[ProgressBar::HEAT].label_.setString(_t("panel.bar_heat"));
}


void ControlPanel::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    // Draw background
    target.draw(m_background, states);

    // Draw progress bars
    for (int i = 0; i < ProgressBar::_PBAR_COUNT; ++i)
    {
        target.draw(pbars_[i].label_, states);
        target.draw(pbars_[i].bar_, states);
        target.draw(pbars_[i].value_, states);
    }
    target.draw(bar_mask_, states);

    // Draw bonus slots
    target.draw(bs_coolers_, states);
    target.draw(bs_missiles_, states);
    target.draw(bs_attack_, states);
    target.draw(bs_speed_, states);

    target.draw(game_info_, states);
    target.draw(timer_, states);
    target.draw(str_points_, states);

    if (game_mode_ == EntityManager::LEVELS_MODE)
    {
        target.draw(level_bar_, states);
        target.draw(level_cursor_, states);
    }
}

// ProgessBar -----------------------------------------------------------------

ControlPanel::ProgressBar::ProgressBar()
{
}


void ControlPanel::ProgressBar::init(const sf::String& text, const sf::Font& font, const sf::Color& color)
{
    label_.setFont(font);
    label_.setString(text);
    label_.setCharacterSize(TEXT_SIZE);
    label_.setFillColor(sf::Color::White);
    bar_.setSize(sf::Vector2f(0, PROG_BAR_HEIGHT));
    bar_.setFillColor(color);
    value_.setFont(font);
    value_.setCharacterSize(TEXT_SIZE);
}


void ControlPanel::ProgressBar::setPosition(int x, int y)
{
    label_.setPosition(x, y - 2);
    int x_bar = x + PROG_BAR_TEXT_LENGTH;
    bar_.setPosition(x_bar, y);
    value_.setPosition(x_bar + 40, y - 2);
}


void ControlPanel::ProgressBar::setValue(int value)
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
    value_.setString(std::to_string(value) + "/" + std::to_string(max_value_));
    // center text on progress bar
    sfh::setX(value_, bar_.getPosition().x + (int) (PROG_BAR_WIDTH - sfh::width(value_)) / 2);
}

// PowerUpSlot ------------------------------------------------------------------

void ControlPanel::PowerUpSlot::init(PowerUp::Type bonus_type, Type type, const sf::Font& font)
{
    icon_.setTexture(Resources::getTexture("entities/power-ups.png"));
    icon_.setTextureRect(PowerUp::getTextureRect(bonus_type));

    label_.setCharacterSize(TEXT_SIZE);
    label_.setFillColor(sf::Color::White);
    label_.setString(type == COUNTER ? "x 0" : "-");
    label_.setFont(font);

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


void ControlPanel::PowerUpSlot::setValue(int count)
{
    switch (type_)
    {
        case COUNTER:
            label_.setString("x " + std::to_string(count));
            timer_ = 0.f;
            glowing_ = UP;
            break;
        case TIMER:
            timer_ = count;
            label_.setString(std::to_string(count) + "s");
            glowing_ = UP;
            glow_.setColor(sf::Color::White);
            break;
    }
}


void ControlPanel::PowerUpSlot::update(float frametime)
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
                    label_.setString(std::to_string(new_timer) + "s");
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


void ControlPanel::PowerUpSlot::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(glow_, states);
    target.draw(icon_, states);
    target.draw(label_, states);
}

