#include "HUD.hpp"
#include "core/Resources.hpp"
#include "utils/I18n.hpp"
#include "utils/StringUtils.hpp"
#include "utils/SFML_Helper.hpp"

#include <string>

#define PROG_BAR_WIDTH       100
#define PROG_BAR_HEIGHT      10.f
#define PROG_BAR_TEXT_LENGTH 50   // label length (pixels) on progress bars

#define TEXT_SIZE      12

#define LEVEL_BAR_X 430
#define LEVEL_BAR_Y 465

#define BAR_SHIP   sf::Color(0xc6, 0x00, 0x00)
#define BAR_SHIELD sf::Color(0x00, 0x80, 0xe0)
#define BAR_HEAT   sf::Color(0x44, 0xc0, 0x00)
#define BAR_OVERHEAT sf::Color(0xff, 0x88, 0x00)


void defaultTextStyle(sf::Text& text)
{
    static const sf::Font& font = Resources::getFont("Vera.ttf");
    text.setFont(font);
    text.setCharacterSize(TEXT_SIZE);
    text.setOutlineColor(sf::Color(0, 0, 0, 128));
    text.setOutlineThickness(1.f);
}


HUD::HUD():
    m_visible(true),
    m_powerUpIceCubes(PowerUpIcon::COUNTER),
    m_powerUpMissiles(PowerUpIcon::COUNTER),
    m_powerUpAttack(PowerUpIcon::TIMER),
    m_powerUpSpeed(PowerUpIcon::TIMER),
    m_levelLength(0)
{
}


void HUD::initialize()
{
    // Init progress bars
    m_infoBars[ProgressBar::HP].init(_t("panel.bar_hp"), BAR_SHIP);
    m_infoBars[ProgressBar::HP].setPosition(10, 5);
    m_infoBars[ProgressBar::SHIELD].init(_t("panel.bar_shield"), BAR_SHIELD);
    m_infoBars[ProgressBar::SHIELD].setPosition(180, 5);
    m_infoBars[ProgressBar::HEAT].init(_t("panel.bar_heat"), BAR_HEAT);
    m_infoBars[ProgressBar::HEAT].setPosition(360, 5);

    m_clock.setPosition(550, 4);
    defaultTextStyle(m_clock);


    m_score.setPosition(570, 31);
    defaultTextStyle(m_score);

    // Init power-up icons
    m_powerUpIceCubes.init(PowerUp::ICECUBE);
    m_powerUpIceCubes.setPosition(20, 455);
    m_powerUpMissiles.init(PowerUp::MISSILE);
    m_powerUpMissiles.setPosition(100, 455);
    m_powerUpAttack.init(PowerUp::DOUBLE_SHOT);
    m_powerUpAttack.setPosition(180, 455);
    m_powerUpSpeed.init(PowerUp::SPEED);
    m_powerUpSpeed.setPosition(260, 455);


    m_levelBar.setTexture(Resources::getTexture("gui/level-bar.png"));
    m_levelBar.setPosition(LEVEL_BAR_X, LEVEL_BAR_Y);
    m_levelCursor.setTexture(Resources::getTexture("gui/level-cursor.png"));
    m_levelCursor.setPosition(LEVEL_BAR_X, LEVEL_BAR_Y);
    m_levelLength = 0;

    m_levelNumber.setPosition(LEVEL_BAR_X + 85, 463);
    defaultTextStyle(m_levelNumber);
}


void HUD::setVisible(bool visible)
{
    m_visible = visible;
}


void HUD::update(float frametime)
{
    m_powerUpIceCubes.update(frametime);
    m_powerUpMissiles.update(frametime);
    m_powerUpAttack.update(frametime);
    m_powerUpSpeed.update(frametime);
}


void HUD::setLevelNumber(int number)
{
    m_levelNumber.setString(_t("panel.level", "{level}", number));
}


void HUD::setScore(int score)
{
    m_score.setString(_t("panel.points", "{points}", score));
}


void HUD::setElapsedTime(float seconds)
{
    static int previous = -1; // < 0, to force update at first call
    int rounded = (int) seconds;
    // Update every second
    if (rounded != previous) {
        sf::String text = _t("panel.timer");
        std::string min = std::to_string(rounded / 60);
        std::string sec = std::to_string(rounded % 60);
        // Format on 2 digits
        text.replace("{min}", min.size() > 1 ? min : "0" + min);
        text.replace("{sec}", sec.size() > 1 ? sec : "0" + sec);
        m_clock.setString(text);
        previous = rounded;
    }
}


void HUD::setPlayerPosition(int pixels)
{
    int maxProgress = sfh::width(m_levelBar) - sfh::width(m_levelCursor);
    int progress = maxProgress * pixels / m_levelLength;
    int x = LEVEL_BAR_X + (progress > maxProgress ? maxProgress : progress);
    sfh::setX(m_levelCursor, x);
}


void HUD::setLevelLength(int pixels)
{
    m_levelLength = pixels;
}


void HUD::setOverheat(bool overheat)
{
    m_infoBars[ProgressBar::HEAT].setBarColor(overheat ? BAR_OVERHEAT : BAR_HEAT);
}


void HUD::setHP(int value)
{
    m_infoBars[ProgressBar::HP].setValue(value);
}


void HUD::setMaxHP(int max)
{
    m_infoBars[ProgressBar::HP].setMaxValue(max);
}


void HUD::setShield(int value)
{
    m_infoBars[ProgressBar::SHIELD].setValue(value);
}


void HUD::setMaxShield(int max)
{
    m_infoBars[ProgressBar::SHIELD].setMaxValue(max);
}


void HUD::setHeat(float value)
{
    m_infoBars[ProgressBar::HEAT].setValue(value);
}


void HUD::setMaxHeat(float max)
{
    m_infoBars[ProgressBar::HEAT].setMaxValue(max);
}


void HUD::setIcecubes(int count)
{
    m_powerUpIceCubes.setValue(count);
}


void HUD::setMissiles(int count)
{
    m_powerUpMissiles.setValue(count);
}


void HUD::activeSpeedPowerUp(int seconds)
{
    m_powerUpSpeed.setValue(seconds);
}


void HUD::activeAttackPowerUp(int seconds, PowerUp::Type powerUpType)
{
    m_powerUpAttack.setIcon(powerUpType);
    m_powerUpAttack.setValue(seconds);
}


void HUD::refreshTextTranslations()
{
    m_infoBars[ProgressBar::HP].init(_t("panel.bar_hp"), BAR_SHIP);
    m_infoBars[ProgressBar::SHIELD].init(_t("panel.bar_shield"), BAR_SHIELD);
    m_infoBars[ProgressBar::HEAT].init(_t("panel.bar_heat"), BAR_HEAT);
}


void HUD::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (!m_visible) return;

    // Draw progress bars
    for (int i = 0; i < ProgressBar::_PBAR_COUNT; ++i) {
        target.draw(m_infoBars[i], states);
    }
    target.draw(m_clock, states);

    // Draw bonus slots
    target.draw(m_powerUpIceCubes, states);
    target.draw(m_powerUpMissiles, states);
    target.draw(m_powerUpAttack, states);
    target.draw(m_powerUpSpeed, states);

    target.draw(m_score, states);

    target.draw(m_levelBar, states);
    target.draw(m_levelNumber, states);
    target.draw(m_levelCursor, states);
}

// ProgessBar -----------------------------------------------------------------

HUD::ProgressBar::ProgressBar():
    m_maxValue(0)
{
    m_bar.setPosition(PROG_BAR_TEXT_LENGTH, 0);
    m_bar.setSize({PROG_BAR_WIDTH, PROG_BAR_HEIGHT});

    m_background.setPosition(PROG_BAR_TEXT_LENGTH - 2, -2);
    m_background.setSize({PROG_BAR_WIDTH + 4, PROG_BAR_HEIGHT + 4});
    m_background.setFillColor(sf::Color(0x33, 0x33, 0x33));

    m_value.setPosition(PROG_BAR_TEXT_LENGTH + 40, -2);

    m_label.setPosition(0, -2);
}


void HUD::ProgressBar::init(const sf::String& text, const sf::Color& color)
{
    defaultTextStyle(m_label);
    defaultTextStyle(m_value);

    m_label.setString(text);
    m_bar.setFillColor(color);
}


void HUD::ProgressBar::setValue(int value)
{
    // resize progress bar
    value = value > 0 ? value : 0;
    float length = (float) value / m_maxValue * (PROG_BAR_WIDTH - 1);
    if (length == 0.0f) {
        length = 0.1f;
    }
    m_bar.setSize(sf::Vector2f(length, PROG_BAR_HEIGHT));

    // display {value}/{max_value}
    m_value.setString(std::to_string(value) + "/" + std::to_string(m_maxValue));
    // center text on progress bar
    sfh::setX(m_value, m_bar.getPosition().x + (int) (PROG_BAR_WIDTH - sfh::width(m_value)) / 2);
}


void HUD::ProgressBar::setMaxValue(int maxValue)
{
    m_maxValue = maxValue;
}


void HUD::ProgressBar::setBarColor(const sf::Color& color)
{
    m_bar.setFillColor(color);
}


void HUD::ProgressBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(m_background, states);
    target.draw(m_label, states);
    target.draw(m_bar, states);
    target.draw(m_value, states);
}


// PowerUpIcon ------------------------------------------------------------------

static constexpr float HUD_POWERUP_WIDTH = 60;
static constexpr float HUD_POWERUP_HEIGHT = 26;

HUD::PowerUpIcon::PowerUpIcon(Type type):
    m_type(type),
    m_timer(-1.f),
    m_glowState(HIDDEN)
{
    m_background.setPosition(0, 0);
    m_icon.setPosition(8, 3);
    // glow is 64x64, centered on bonus sprite
    m_glow.setPosition(-20, -20);
    m_label.setPosition(32, 4);
}


void HUD::PowerUpIcon::init(PowerUp::Type powerUpType)
{
    m_background.setTexture(Resources::getTexture("gui/hud-power-up-icon.png"));
    m_icon.setTexture(Resources::getTexture("entities/power-ups.png"));
    m_icon.setTextureRect(PowerUp::getTextureRect(powerUpType));
    m_label.setString(m_type == COUNTER ? "0" : "-");
    defaultTextStyle(m_label);

    m_glow.setTexture(Resources::getTexture("gui/bonus-glow.png"));
    m_glow.setColor(sf::Color(255, 255, 255, 0));
}


void HUD::PowerUpIcon::setIcon(PowerUp::Type powerUpType)
{
    m_icon.setTextureRect(PowerUp::getTextureRect(powerUpType));
}


void HUD::PowerUpIcon::setValue(int count)
{
    switch (m_type) {
    case COUNTER:
        m_label.setString(std::to_string(count));
        m_timer = 0.f;
        m_glowState = FADE_IN;
        break;
    case TIMER:
        m_timer = count; // duration in seconds
        m_label.setString(std::to_string(count) + "s");
        m_glowState = FADE_IN;
        m_glow.setColor(sf::Color::White);
        break;
    }
}


void HUD::PowerUpIcon::update(float frametime)
{
    switch (m_type) {
    case COUNTER:
        if (m_glowState != HIDDEN) {
            const float DELAY = 1.5f;
            m_timer += frametime;
            int alpha = 255 * m_timer / DELAY;
            if (m_glowState == FADE_IN && m_timer >= DELAY) {
                m_glowState = FADE_OUT;
                m_timer = 0.f;
                alpha = 255;
            } else if (m_glowState == FADE_OUT) {
                alpha = 255 - alpha;
                if (m_timer >= DELAY) {
                    m_glowState = HIDDEN;
                    m_glow.setColor(sf::Color(255, 255, 255, 0));
                    return;
                }

            }
            m_glow.setColor(sf::Color(255, 255, 255, alpha));
        }
        break;
    case TIMER:
        if (m_glowState == FADE_IN) {
            int oldTime = (int) (m_timer + 0.5f);
            m_timer -= frametime;
            int newTime = (int) (m_timer + 0.5f);
            if (newTime != oldTime) {
                m_label.setString(std::to_string(newTime) + "s");
            } else if (m_timer <= 0.f) {
                m_glow.setColor(sf::Color(255, 255, 255, 0));
                m_glowState = HIDDEN;
                m_label.setString("-");
            }
        }
        break;
    }
}


void HUD::PowerUpIcon::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_background, states);
    target.draw(m_glow, states);
    target.draw(m_icon, states);
    target.draw(m_label, states);
}
