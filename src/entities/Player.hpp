#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Damageable.hpp"
#include "Animator.hpp"
#include "PowerUp.hpp"
#include "core/Input.hpp"
#include "particles/ParticleEmitter.hpp"
#include "entities/Weapon.hpp"

class HUD;

/**
 * Player's spaceship
 */
class Player: public Damageable
{
public:
    Player();
    ~Player();

    void onActionDown(Action::ID action);

    void onActionUp(Action::ID action);

    void onEvent(const sf::Event& event);

    int getScore() const;

    void updateScore(int diff);

    void takeDamage(int damage) override;

    inline bool isCheater() const { return m_konamiCodeActivated; }

    // callbacks ---------------------------------------------------------------

    void onInit() override;

    void onUpdate(float frametime) override;

    void onCollision(PowerUp& powerup) override;

    void onTileCollision() override;

    void onDestroy() override;

    void setMaxHeat(int heat);

    void setMaxHP(int hp);

    void setSpeed(float speed);

    void setMaxShield(int shield);

    Weapon& getLaser();

private:
    enum TimedPowerUp
    {
        T_DOUBLESHOT,
        T_TRISHOT,
        T_SPEED,
        TIMED_POWERUP_COUNT
    };

    /**
     * Unapply a powerup when duration is elapsed
     */
    void DisableTimedPowerUp(TimedPowerUp tbonus);

    /**
     * Apply some upgrades when konami code sequence is activated
     */
    void toggleKonamiCode();

    /**
     * Set shield points
     */
    void setShield(int shield);

    /**
     * Warn the player if heat has reached a given threshold by playing an audio signal
     */
    void overheatAudioHint() const;

    // Code Konami sequence
    static const int KONAMI_CODE_LENGTH = 10;
    Action::ID m_konamiCode[KONAMI_CODE_LENGTH];
    int        m_currentKonamiIndex;
    bool       m_konamiCodeActivated;

    float  m_powerUps[TIMED_POWERUP_COUNT]; // timers, in seconds
    bool   m_overheat;
    float  m_heat, m_max_heat;
    int    m_shield, m_max_shield;
    int    m_max_hp;
    float  m_speed;
    int    m_missiles;
    int    m_icecubes;
    Weapon m_weapon;
    Weapon m_missileLauncher;

    Animator m_animator;
    int      m_score;

    class ShieldEmitter: public ParticleEmitter {
    public:
        using ParticleEmitter::ParticleEmitter;

        void createParticles(size_t count);

        void onParticleUpdated(Particle& particle, float frametime) const override;
    };

    ShieldEmitter   m_shieldEmitter;
    ParticleEmitter m_engineEmitter;
    ParticleEmitter m_smokeEmitter;
    ParticleEmitter m_snowflakesEmitter;
    ParticleEmitter m_powerupEmitter;

    HUD& m_hud;
};

#endif // PLAYER_HPP
