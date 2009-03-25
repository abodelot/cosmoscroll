#ifndef BONUS_HPP
#define BONUS_HPP

#include "Entity.hpp"


class Bonus: public Entity
{
public:
	enum Type
	{
		COOLER,   // ajoute 1 glaçon
		HEALTH,   // ajoute 1 point de vie
		SPEED,    // vitesse x2
		TRISHOT,  // tir x3
		BONUS_COUNT
	};

	Bonus(Type type, const sf::Vector2f& offset);

	// allouer un bonus aléatoirement
	static Bonus* MakeRandom(const sf::Vector2f& offset);

	void Hit(int damage);

	// inherited
	void OnCollide(Entity& entity);

	void Move(float frametime);

	/*
	 * Description textuelle du bonus
	 */
	const wchar_t* WhatItIs() const;

	inline Type GetType() const
	{
		return type_;
	}

private:
	Type type_;
};

#endif /* guard BONUS_HPP */

