#include "ArmoryMenu.hpp"
#include "core/Game.hpp"
#include "utils/I18n.hpp"
#include "UpgradeItem.hpp"

ArmoryMenu::ArmoryMenu()
{
	SetTitle(_t("armory.title"));
	gui::Button* but_back = new CosmoButton(this, _t("menu.back_main_menu"), 210, 410);
	but_back->SetCallbackID(0);

	new gui::Image(this, GET_IMG("gui/armory-ship-view"));
	for (int i = 0; i < UpgradeItem::_UP_COUNT; ++i)
	{
		new UpgradeItem(this, (UpgradeItem::Type) i);
	}
}


void ArmoryMenu::EventCallback(int id)
{
	if (id == 0)
	{
		Game::GetInstance().SetNextScene(Game::SC_LevelMenu);
	}
}



