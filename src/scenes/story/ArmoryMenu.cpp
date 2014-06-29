#include "ArmoryMenu.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include "core/UserSettings.hpp"
#include "core/Resources.hpp"
#include "core/SoundSystem.hpp"
#include "utils/I18n.hpp"
#include "scenes/ConfigButton.hpp"
#include "items/ItemManager.hpp"


ArmoryMenu::ArmoryMenu()
{
	SetTitle(_t("armory.title"));

	// Upgrade item
	gui::VBoxLayout layout(100, 90);
	layout.SetSpacing(0, 0);
	const sf::Font& font = Resources::getFont("Vera.ttf");
	for (int i = 0; i < Item::_COUNT; ++i)
	{
		m_items[i] = new ShipItemWidget(this, (Item::Type) i, font);
		m_items[i]->SetCallbackID(1);
		layout.Add(m_items[i]);
	}

	// Credit counter
	m_credits = new CreditCounterWidget(this);

	// Back button
	gui::Button* but_back = new CosmoButton(this, _t("menu.back"));
	but_back->setPosition(210, 410);
	but_back->SetCallbackID(2);
}


void ArmoryMenu::OnFocus()
{
	// Refresh UI
	m_credits->setCredits(UserSettings::getCredits());
	for (int i = 0; i < Item::_COUNT; ++i)
	{
		m_items[i]->refresh();
	}
}


void ArmoryMenu::EventCallback(int id)
{
	switch (id)
	{
		case 1:
			OnFocus();
			break;
		case 2:
			Game::getInstance().setNextScene(Game::SC_LevelMenu);
			break;
	}
}
