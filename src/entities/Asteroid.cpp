#include "Asteroid.hpp"
#include "EntityManager.hpp"
#include "core/SoundSystem.hpp"
#include "core/Resources.hpp"
#include "utils/Math.hpp"

#define BASE_SPEED         80
#define ROTATION_SPEED_MIN 10
#define ROTATION_SPEED_MAX 80


Asteroid::Asteroid(Size size, float angle):
	m_size(size),
	m_rotation_speed(math::rand(ROTATION_SPEED_MIN, ROTATION_SPEED_MAX))
{
	setHP(size * 2 + 1);
	setTexture(Resources::getTexture("entities/asteroids.png"));
	setRandomImage();

	// Compute speed vector from angle and velocity
	angle = math::to_radians(angle);
	m_speed.x = std::cos(angle) * BASE_SPEED;
	m_speed.y = -std::sin(angle) * BASE_SPEED;

	// Origin must be located at sprite center to enable centered rotation
	setOrigin(getWidth() / 2, getHeight() / 2);
}


void Asteroid::onUpdate(float frametime)
{
	sf::Vector2f pos = getPosition();
	pos.x += m_speed.x * frametime;
	pos.y += m_speed.y * frametime;
	setPosition(pos);
	rotate(m_rotation_speed * frametime);
}


void Asteroid::onDestroy()
{
	sf::Vector2f pos = getPosition();
	switch (m_size)
	{
		case BIG:
			// Create 3 medium asteroids
			for (int i = 0; i < 3; ++i)
			{
				Asteroid* asteroid = new Asteroid(MEDIUM, math::rand(0, 360));
				asteroid->setPosition(pos);
				EntityManager::getInstance().addEntity(asteroid);
			}
			break;
		case MEDIUM:
			// Create 3 small asteroids
			for (int i = 0; i < 3; ++i)
			{
				Asteroid* asteroid = new Asteroid(SMALL, math::rand(0, 360));
				asteroid->setPosition(pos);
				EntityManager::getInstance().addEntity(asteroid);
			}
			break;
		default:
			break;
	}
	SoundSystem::GetInstance().PlaySound("asteroid-break.ogg");
	EntityManager::getInstance().createImpactParticles(getPosition(), 10);
}


void Asteroid::setRandomImage()
{

	int x = math::rand(0, 5); // nb sprites = 6
	switch (m_size)
	{
		case BIG:
			setTextureRect(sf::IntRect(0 + x * 64, 0, 64, 64)); // 64px
			break;
		case MEDIUM:
			setTextureRect(sf::IntRect(0 + x * 32, 64, 32, 32)); // 32px
			break;
		case SMALL:
			setTextureRect(sf::IntRect(192 + x * 16, 64, 16, 16)); // 16px
			break;
	}
}


