#include <typeinfo>

#include "Hit.hpp"
#include "Bonus.hpp"
#include "../utils/Math.hpp"
#include "../core/ParticleSystem.hpp"


Hit::Hit(Entity::Team team, const sf::Vector2f& offset, float angle,
	const sf::Image* image, int speed, int damage) :
	Entity(offset, 1, damage)
{
	SetImage(*image);
	SetTeam(team);
	SetRotation(math::rad_to_deg(angle));

	speed_ = speed;
	angle_ = angle;
}


Hit* Hit::Clone() const
{
	return new Hit(*this);
}


void Hit::Update(float frametime)
{
	sf::Vector2f pos = GetPosition();
	math::translate(pos, angle_, speed_ * frametime);
	SetPosition(pos);
	KillIfOut();
}


void Hit::OnCollide(Entity& entity)
{
	// traverse les tirs et les bonus
	if (entity.GetTeam() == GetTeam()
		|| typeid (entity) == typeid (Hit)
		|| typeid (entity) == typeid (Bonus))
	{
		return;
	}
	Entity::OnCollide(entity);
	ParticleSystem::GetInstance().AddImpact(GetPosition(), 10);
	Kill();
}


void Hit::TakeDamage(int)
{
}
