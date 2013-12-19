#ifndef ENDGAMESCENE_HPP
#define ENDGAMESCENE_HPP

#include "BaseScene.hpp"
#include "utils/sfml_helper.hpp"

class EntityManager;

class EndGameScene: public BaseScene
{
public:
	EndGameScene();

	void OnEvent(const sf::Event& event) override;

	void Update(float frametime) override;

	void Show(sf::RenderTarget& target) const override;

	void OnFocus() override;

private:
	float timer_;
	xsf::Text info_;
	EntityManager& entities_;
};

#endif // ENDGAMESCENE_HPP

