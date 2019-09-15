#ifndef HUD_HPP
#define HUD_HPP

#include <SFML/Graphics.hpp>
#include "entities/EntityManager.hpp"
#include "entities/PowerUp.hpp"

/**
 * HUD: panel displaying various data about player status, current level, ...
 */
class HUD: public sf::Drawable
{
public:
    // height in pixels, width is Game::WIDTH
    enum { HEIGHT = 56 };

    HUD();

    void initialize();

    void setVisible(bool visible);

    void update(float frametime);

    void setLevelNumber(int number);

    void setScore(int score);

    /**
     * @param seconds: elapsed time
     */
    void setElapsedTime(float seconds);

    /**
     * Set player horizontal progression
     * @param pixel: x position
     */
    void setPlayerPosition(int pixels);

    /**
     * Set current level total length
     */
    void setLevelLength(int pixels);

    // setters progress bars --------------------------------------------------

    /**
     * Set current player HP and max HP
     */
    void setHP(int value);
    void setMaxHP(int max);

    /**
     * Set current shield points and max shield points
     */
    void setShield(int value);
    void setMaxShield(int max);

    /**
     * Set current heat and max heat
     */
    void setHeat(float value);
    void setMaxHeat(float max);

    /**
     * Enable/disable overheat effect on heat progress bar
     */
    void setOverheat(bool overheat);

    // setters bonus slots ----------------------------------------------------

    // Set icecubess count
    void setIcecubes(int count);

    // Set missiles count
    void setMissiles(int count);

    void activeSpeedPowerUp(int seconds);

    void activeAttackPowerUp(int seconds, PowerUp::Type bonus_type);

    void refreshTextTranslations();

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    class ProgressBar: public sf::Drawable, public sf::Transformable {
    public:
        enum {
            SHIELD, HP, HEAT, _PBAR_COUNT
        };

        ProgressBar();

        void init(const sf::String& text, const sf::Color& color);

        // Set progress bar value
        void setValue(int value);
        void setMaxValue(int maxValue);

        void setBarColor(const sf::Color& color);

    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        sf::Text m_label;
        sf::RectangleShape m_background;
        sf::RectangleShape m_bar;
        sf::Text m_value;
        int m_maxValue;
    };

    class PowerUpIcon: public sf::Drawable, public sf::Transformable {
    public:
        enum Type { COUNTER, TIMER };

        PowerUpIcon(Type type);

        void init(PowerUp::Type powerUpType);

        void setIcon(PowerUp::Type powerUpType);

        void setValue(int value);

        void update(float frametime);

    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        Type m_type;
        sf::Sprite m_background;
        sf::Sprite m_icon;
        sf::Text m_label;
        sf::Sprite m_glow;
        float m_timer;
        enum GlowingState { FADE_IN, FADE_OUT, HIDDEN } m_glowState;
    };

    bool m_visible;

    ProgressBar m_infoBars[ProgressBar::_PBAR_COUNT];
    PowerUpIcon m_powerUpIceCubes;
    PowerUpIcon m_powerUpMissiles;
    PowerUpIcon m_powerUpAttack;
    PowerUpIcon m_powerUpSpeed;

    sf::Text m_clock;

    // Level info
    sf::Text m_levelNumber;
    int m_levelLength;
    sf::Sprite m_levelCursor;
    sf::Sprite m_levelBar;

    sf::Text m_score;
};

#endif
