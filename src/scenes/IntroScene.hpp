#ifndef INTROSCENE_HPP
#define INTROSCENE_HPP

#include "BaseScene.hpp"
#include "utils/SFML_Helper.hpp"

class EntityManager;
class Player;

/**
 * Nice interactive splash screen at start =)
 */
class IntroScene: public BaseScene
{
public:
	IntroScene();

	void OnEvent(const sf::Event& event) override;

	void Update(float frametime) override;

	void Show(sf::RenderTarget& target) const override;

private:
	float elapsed_;
	xsf::Sprite background_;
	xsf::Sprite title_;
	EntityManager& entity_mgr_;
	Player* ship_;
};

#endif // INTROSCENE_HPP

