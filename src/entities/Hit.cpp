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

	// calcul du vecteur vitesse à partir de l'angle et de la vitesse
	speed_.x = std::cos(angle) * speed;
	speed_.y = -std::sin(angle) * speed;
}


Hit* Hit::Clone() const
{
	return new Hit(*this);
}


void Hit::Update(float frametime)
{
	sf::Vector2f pos = GetPosition();
	pos.x = pos.x + speed_.x * frametime;
	pos.y = pos.y + speed_.y * frametime;
	SetPosition(pos);
	KillIfOut();
}


void Hit::OnCollide(Entity& entity)
{
	if (!IsDead())
	{
		// ignore les alliés, les tirs et les bonus
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
}


void Hit::TakeDamage(int)
{
}
