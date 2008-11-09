#ifndef EVILBOSS_HPP
#define EVILBOSS_HPP

#include "Weapon.hpp"
#include "Game.hpp"
#include "Ennemy.hpp"

/*
 * Il est très méchant, et il en veut!
 */
class EvilBoss: public Ennemy
{
public:
    EvilBoss(const sf::Vector2f& offset, Entity* target);
    
    void Move(float frametime);
	void Collide(Entity& ent);
	void Action();
private:
	bool left_;
	Weapon weaponA_;
	Weapon weaponB_;
	Game& game_;
	float frametime_;	//frametime du dernier Move. Utilisié par Action
};

#endif /* guard EVILBOSS_HPP */

