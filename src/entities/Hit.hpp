#ifndef HIT_HPP
#define HIT_HPP

#include "Entity.hpp"


class Hit: public Entity
{
public:
	Hit(Entity::Team team, const sf::Vector2f& offset, float angle,
		const sf::Image* image, int speed, int damage);

	Hit* Clone() const;

	void Update(float frametime);

	void OnCollide(Entity& entity);

	void TakeDamage(int);

private:
	float angle_;
	int speed_;
};

#endif // HIT_HPP
