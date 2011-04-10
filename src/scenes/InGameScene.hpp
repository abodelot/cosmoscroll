#ifndef INGAMESCENE_HPP
#define INGAMESCENE_HPP

#include "BaseScene.hpp"

class EntityManager;
class ControlPanel;

/**
 * Scène du déroulement du jeu
 */
class InGameScene: public BaseScene
{
public:
	InGameScene();

	// inherited
	void OnEvent(const sf::Event& event);

	// inherited
	void Update(float frametime);

	// inherited
	void Show(sf::RenderTarget& target) const;

	// inherited
	void OnFocus();

private:
	EntityManager& entities_;
	ControlPanel& panel_;
};

#endif // INGAMESCENE_HPP

