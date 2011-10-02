#include <typeinfo>

#include "Hit.hpp"
#include "Bonus.hpp"
#include "PlayerShip.hpp"
#include "utils/Math.hpp"
#include "utils/StringUtils.hpp"
#include "core/ParticleSystem.hpp"


Hit::Hit(Entity::Team team, const sf::Vector2f& position, float angle,
	const sf::Image* image, int speed, int damage) :
	Entity(position, 1, damage)
{
	SetImage(*image);
	SetTeam(team);
	SetRotation(math::rad_to_deg(angle));
	// calcul du vecteur vitesse à partir de l'angle et de la vitesse
	speed_.x = std::cos(angle) * speed;
	speed_.y = -std::sin(angle) * speed;

	// origin is located at sprite center to allow rotation
	const sf::IntRect& rect = GetSubRect();
	SetCenter(rect.GetWidth() / 2, rect.GetHeight() / 2);
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


void Hit::GetCollideRect(sf::FloatRect& rect) const
{
	// origin is centered
	int width = GetSize().x;
	int height = GetSize().y;
	rect.Left = GetPosition().x - width / 2;
	rect.Top = GetPosition().y - height / 2;
	rect.Right = rect.Left + width;
	rect.Bottom = rect.Top + height;
}


void Hit::OnCollide(Entity& entity)
{
	if (!IsDead())
	{
		// ignore friend entities, hit and bonuses
		if (entity.GetTeam() == GetTeam()
			|| typeid (entity) == typeid (Hit)
			|| typeid (entity) == typeid (Bonus))
		{
			return;
		}
		Entity::OnCollide(entity);
		ParticleSystem::GetInstance().AddImpact(GetPosition(), 10);
		if (GetTeam() == Entity::GOOD && entity.IsDead())
		{
			int points = entity.ConsumePoints();
			if (points != 0)
			{
				std::string s = "+" + to_string(points);
				EntityManager& e = EntityManager::GetInstance();
				e.GetPlayerShip()->UpdateScoreCounter(points);
				ParticleSystem::GetInstance().AddMessage(GetPosition(), s, sf::Color(255, 128, 0));
			}
		}
		Kill();
	}
}


void Hit::TakeDamage(int)
{
}
