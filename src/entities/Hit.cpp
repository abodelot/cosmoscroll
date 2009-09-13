#include <typeinfo>

#include "Hit.hpp"
#include "Bonus.hpp"
#include "../utils/Math.hpp"


Hit::Hit(int emitter_id, const sf::Vector2f& offset, float angle,
	const sf::Image* image, int speed, int damage) :
	Entity(*image, offset, 1, damage)
{
	speed_ = speed;
	SetRotation(RAD_TO_DEG(angle));
	angle_ = angle;
	emitter_id_ = emitter_id;
}


void Hit::Update(float frametime)
{
	sf::Vector2f pos = GetPosition();
	TRANSLATE(pos, angle_, speed_ * frametime);
	SetPosition(pos);
	KillIfOut();
}


void Hit::OnCollide(Entity& entity)
{
	// traverse les tirs et les bonus
	if (entity.GetID() == emitter_id_
		|| typeid (entity) == typeid (Hit)
		|| typeid (entity) == typeid (Bonus))
	{
		return;
	}
	Entity::OnCollide(entity);
	Kill();
}


void Hit::TakeDamage(int)
{
}
