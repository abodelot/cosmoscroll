#include "Asteroid.hpp"
#include "EntityManager.hpp"
#include "core/ParticleSystem.hpp"
#include "core/SoundSystem.hpp"
#include "core/Resources.hpp"
#include "utils/Math.hpp"

#define BASE_SPEED         80
#define ROTATION_SPEED_MIN 40
#define ROTATION_SPEED_MAX 160


Asteroid::Asteroid(Size size, float angle):
	m_size(size),
	m_rotation_speed(math::random(ROTATION_SPEED_MIN, ROTATION_SPEED_MAX))
{
	setHP(size * 2 + 1);
	setRandomImage();

	// Compute speed vector from angle and velocity
	angle = math::to_rad(angle);
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
			// Create 2 medium asteroids
			for (int i = 0; i < 2; ++i)
			{
				Asteroid* asteroid = new Asteroid(MEDIUM, math::random(0, 360));
				asteroid->setPosition(pos);
				EntityManager::getInstance().addEntity(asteroid);
			}
			break;
		case MEDIUM:
			// Create 4 small asteroids
			for (int i = 0; i < 4; ++i)
			{
				Asteroid* asteroid = new Asteroid(SMALL, math::random(0, 360));
				asteroid->setPosition(pos);
				EntityManager::getInstance().addEntity(asteroid);
			}
			break;
		default:
			break;
	}
	SoundSystem::GetInstance().PlaySound("asteroid-break.ogg");
	ParticleSystem::GetInstance().ImpactSfx(getPosition(), 10);
}


void Asteroid::setRandomImage()
{
	int x = 0;
	switch (m_size)
	{
		case SMALL:
			setTexture(Resources::getTexture("entities/asteroid-small.png"));
			x = math::random(0, 3) * 16; // 4 sprites
			setTextureRect(sf::IntRect(x, 0, 16, 16));
			break;
		case MEDIUM:
			setTexture(Resources::getTexture("entities/asteroid-medium.png"));
			x = math::random(0, 2) * 32; // 3 sprites
			setTextureRect(sf::IntRect(x, 0, 32, 32));
			break;
		case BIG:
			setTexture(Resources::getTexture("entities/asteroid-big.png"));
			x = math::random(0, 2) * 48; // 3 sprites
			setTextureRect(sf::IntRect(x, 0, 48, 48));
			break;
	}
}


