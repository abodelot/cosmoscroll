#ifndef INTROSCENE_HPP
#define INTROSCENE_HPP

#include "BaseScene.hpp"
#include "utils/sfml_helper.hpp"

class EntityManager;
class PlayerShip;

/**
 * Nice interactive splash screen at start =)
 */
class IntroScene: public BaseScene
{
public:
	IntroScene();

	// inherited
	void OnEvent(const sf::Event& event);

	// inherited
	void Update(float frametime);

	// inherited
	void Show(sf::RenderTarget& target) const;

private:
	float elapsed_;
	xsf::Sprite background_;
	xsf::Sprite title_;
	EntityManager& entity_mgr_;
	PlayerShip* ship_;
};

#endif // INTROSCENE_HPP

