#ifndef PLAYSCREEN_HPP
#define PLAYSCREEN_HPP

#include "Screen.hpp"

class EntityManager;
class ControlPanel;

/**
 * Playing state
 */
class PlayScreen: public Screen
{
public:
	PlayScreen();

	void onEvent(const sf::Event& event) override;

	void onFocus() override;

	void update(float frametime) override;

	void draw(sf::RenderTarget& target) const override;

private:
	void setPanelOnTop(bool top);

	EntityManager& m_entities;
	ControlPanel&  m_panel;
};

#endif // PLAYSCREEN_HPP

