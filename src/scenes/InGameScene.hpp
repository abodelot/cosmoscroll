#ifndef INGAMESCENE_HPP
#define INGAMESCENE_HPP

#include "BaseScene.hpp"

class EntityManager;
class ControlPanel;

/**
 * Playing state
 */
class InGameScene: public BaseScene
{
public:
	InGameScene();

	void OnEvent(const sf::Event& event) override;

	void OnFocus() override;

	void Update(float frametime) override;

	void Show(sf::RenderTarget& target) const override;

private:
	void setPanelOnTop(bool top);

	EntityManager& entities_;
	ControlPanel& panel_;
};

#endif // INGAMESCENE_HPP

