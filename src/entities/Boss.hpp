#ifndef BOSS_HPP_
#define BOSS_HPP_

#include "Entity.hpp"
#include "items/Weapon.hpp"


class Boss : public Entity
{
    public:
        Boss( sf::Vector2f& pos,int HP = 300,bool split = true);

        Boss* Clone() const;

            // override
        void Update(float frametime);

        // override
        void TakeDamage(int damage);

        // override
        void SetTarget(Entity* target);

        // override
        void OnDestroy();

        //for moving the small bosses in different directions
        void invert_speed_y();
    private :
        int speed_x_,speed_y_;
        Entity* target_;
        bool split_mode_;
        Weapon eye_left_;
        Weapon eye_right_;
        Weapon canon_;
};


#endif // BOSSDIV_HPP_
