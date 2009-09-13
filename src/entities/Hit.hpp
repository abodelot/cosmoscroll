#ifndef HIT_HPP
#define HIT_HPP

#include "Entity.hpp"


class Hit: public Entity
{
public:
	Hit(int emitter_id, const sf::Vector2f& offset, float angle,
		const sf::Image* image, int speed, int damage);

	void Update(float frametime);

	void OnCollide(Entity& entity);

	void TakeDamage(int);

private:
	float angle_;
	int speed_;
	int emitter_id_;
};

#endif // HIT_HPP
