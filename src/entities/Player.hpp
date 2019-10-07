#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Damageable.hpp"
#include "Animator.hpp"
#include "PowerUp.hpp"
#include "core/Input.hpp"
#include "core/ControlPanel.hpp"
#include "core/ParticleEmitter.hpp"
#include "entities/Weapon.hpp"

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

    inline bool isCheater() const { return m_konami_code_activated; }

    // callbacks ---------------------------------------------------------------

    void onInit();

    void onUpdate(float frametime);

    void onCollision(PowerUp& powerup);

    void onDestroy();

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
        TIMED_BONUS_COUNT
    };

    /**
     * Unapply a powerup when duration is elapsed
     */
    void DisableTimedPowerUp(TimedPowerUp tbonus);

    /**
     * Apply some upgrades when konami code sequence is activated
     */
    void applyKonamiCode();

    /**
     * Set shield points
     */
    void setShield(int shield);

    /**
     * Warn the player if heat has reached a given threshold by playing an audio signal
     */
    void overheatAudioHint() const;

    ControlPanel& m_panel;

    // Code Konami sequence
    static const int KONAMI_CODE_LENGTH = 10;
    Action::ID       m_konami_code[KONAMI_CODE_LENGTH];
    int              m_current_konami_index;
    bool             m_konami_code_activated;

    float bonus_[TIMED_BONUS_COUNT]; // timers des bonus
    bool   m_overheat;
    float  m_heat, m_max_heat;
    int    m_shield, m_max_shield;
    int    m_max_hp;
    float  m_speed;
    int    m_missiles;
    int    m_icecubes;
    Weapon m_weapon;
    Weapon m_missile_launcher;

    Animator         m_animator;
    const Animation& m_animation_up;
    const Animation& m_animation_down;
    const Animation& m_animation_normal;

    int              m_score;

    class ShieldEmitter: public ParticleEmitter
    {
    public:
        void createParticles(size_t count);

        void onParticleUpdated(ParticleSystem::Particle& particle, float frametime) const override;
    };

    ShieldEmitter   m_shieldEmitter;
    ParticleEmitter m_engineEmitter;
    ParticleEmitter m_smokeEmitter;
    ParticleEmitter m_snowflakesEmitter;
    ParticleEmitter m_powerupEmitter;
};

#endif // PLAYER_HPP

