#include "Hit.hpp"
#include "Bonus.hpp"
#include "PlayerShip.hpp"
#include "utils/Math.hpp"
#include "utils/StringUtils.hpp"
#include "core/ParticleSystem.hpp"


Hit::Hit(Entity* emitter, const sf::Vector2f& position, float angle,
	const sf::Texture* image, int speed, int damage) :
	Entity(position, 1, damage)
{
	setTexture(*image);
	SetTeam(emitter->GetTeam());
	SetCollideFlag(C_IGNORE_HITS | C_IGNORE_DAMAGE); // hit objects die by themselves on collide
	setRotation(-math::to_deg(angle));

	// calcul du vecteur vitesse à partir de l'angle et de la vitesse
	speed_.x = std::cos(angle) * speed + emitter->GetSpeedX();
	speed_.y = -std::sin(angle) * speed;

	// origin is located at sprite center to allow rotation
	setOrigin(getWidth() / 2, getHeight() / 2);
}


Hit* Hit::Clone() const
{
	return new Hit(*this);
}


void Hit::Update(float frametime)
{
	move(speed_.x * frametime, speed_.y * frametime);
}


void Hit::OnCollide(Entity& entity)
{
	// ignore friend entities, hit and bonuses
	if (entity.GetTeam() == GetTeam() || entity.GetCollideFlag() & C_IGNORE_HITS)
	{
		return;
	}
	Entity::OnCollide(entity);
	ParticleSystem::GetInstance().ImpactSfx(getPosition(), 10);
	if (GetTeam() == Entity::GOOD && entity.IsDead())
	{
		int points = entity.ConsumePoints();
		if (points != 0)
		{
			std::string s = "+" + to_string(points);
			EntityManager& e = EntityManager::GetInstance();
			e.GetPlayerShip()->UpdateScoreCounter(points);
			ParticleSystem::GetInstance().AddMessage(getPosition(), s, sf::Color(255, 128, 0));
		}
	}
	Kill();
}
