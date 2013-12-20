#ifndef PAUSEMENU_HPP
#define PAUSEMENU_HPP

#include "BaseMenu.hpp"

class PauseMenu: public BaseMenu
{
public:
	PauseMenu();

	void OnEvent(const sf::Event& event) override;

	void OnFocus() override;

	void Update(float frametime) override;

	void Show(sf::RenderTarget& target) const override;

private:
	void EventCallback(int id) override;
};

#endif // PAUSEMENU_HPP

