#include "Asteroid.hpp"
#include "EntityManager.hpp"
#include "core/ParticleSystem.hpp"
#include "core/SoundSystem.hpp"
#include "core/Resources.hpp"
#include "utils/Math.hpp"

// division en morceaux de taille moindre
#define BIG_SPLIT_INTO     2
#define MEDIUM_SPLIT_INTO  4

#define BASE_SPEED         80
#define ROTATION_SPEED_MIN 40
#define ROTATION_SPEED_MAX 160


Asteroid::Asteroid(const sf::Vector2f& position, Size size, float angle) :
	Entity(position, size + 1)
{
	rotation_speed_ = math::random(ROTATION_SPEED_MIN, ROTATION_SPEED_MAX);
	size_ = size;
	SetRandomImage();

	// calcul du vecteur vitesse à partir de l'angle et de la vitesse
	angle = math::to_rad(angle);
	speed_.x = std::cos(angle) * BASE_SPEED;
	speed_.y = -std::sin(angle) * BASE_SPEED;

	// origin is located at sprite center to enable centered rotation
	const sf::IntRect& rect = getTextureRect();
	setOrigin(rect.width / 2, rect.height / 2);
	// hack: décalage de la position car l'origine du sprite est le centre et non le coin top-left
	sf::Sprite::move(sf::Sprite::getOrigin());
}


Asteroid* Asteroid::Clone() const
{
	Asteroid* asteroid = new Asteroid(*this);
	asteroid->SetRandomImage();
	return asteroid;
}


void Asteroid::Update(float frametime)
{
	sf::Vector2f pos = getPosition();
	pos.x += speed_.x * frametime;
	pos.y += speed_.y * frametime;
	setPosition(pos);
	rotate(rotation_speed_ * frametime);
}


void Asteroid::OnDestroy()
{
	sf::Vector2f pos = getPosition();

	static EntityManager& manager = EntityManager::GetInstance();
	switch (size_)
	{
		case BIG:
			for (int i = 0; i < BIG_SPLIT_INTO; ++i)
			{
				manager.AddEntity(new Asteroid(pos, MEDIUM, math::random(0, 360)));
			}
			break;
		case MEDIUM:
			for (int i = 0; i < MEDIUM_SPLIT_INTO; ++i)
			{
				manager.AddEntity(new Asteroid(pos, SMALL, math::random(0, 360)));
			}
			break;
		default:
			break;
	}
	SoundSystem::GetInstance().PlaySound("asteroid-break.ogg");
	ParticleSystem::GetInstance().ImpactSfx(getPosition(), 10);
}


void Asteroid::SetRandomImage()
{
	int x = 0;
	switch (size_)
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


