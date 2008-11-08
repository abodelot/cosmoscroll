#ifndef BONUS_HPP
#define BONUS_HPP

#include "Entity.hpp"


class Bonus: public Entity
{
public:
	enum Type
	{
		HEALTH, TRIGUN, COOLER, BONUS_COUNT
	};
	
	Bonus(Type type, const sf::Vector2f& offset);
	
	// allouer un bonus aléatoirement
	static Bonus* MakeRandom(const sf::Vector2f& offset);
	
	void Hit(int damage);
	
	void Collide(Entity& ent);
	
	void Move(float frametime);
	
	/*
	 * Description textuelle du bonus
	 */
	const char* WhatItIs() const;
	
	inline Type GetType() const
	{
		return type_;
	}
	
private:
	Type type_;
};

#endif /* guard BONUS_HPP */

