#include "ArmoryMenu.hpp"
#include "core/Game.hpp"
#include "utils/I18n.hpp"
#include "utils/StringUtils.hpp"
#include "scenes/ConfigButton.hpp"


ArmoryMenu::ArmoryMenu()
{
	SetTitle(_t("armory.title"));
	gui::Button* but_back = new CosmoButton(this, _t("menu.back"), 210, 410);
	but_back->SetCallbackID(102);

	new gui::Image(this, GET_IMG("gui/armory-ship-view"));
	for (int i = 0; i < UpgradeItem::_UP_COUNT; ++i)
	{
		new UpgradeItem(this, (UpgradeItem::Type) i);
	}
	// init dialog
	dialog_.background = new gui::Image(this, GET_IMG("gui/armory-dialog"));
	int x = 120;
	int y = 180;
	dialog_.background->SetPosition(x, y);


	dialog_.lab_item = new gui::Label(this, "\n");
	dialog_.lab_item->SetPosition(x + 30, y + 10);
	// left side
	gui::VBoxLayout layout_left(x + 10, y + 50);
	layout_left.SetSpacing(5);
	const sf::Font& font = MediaManager::GetFont("Ubuntu-R.ttf");

	dialog_.current_level = new gui::Label(this, "\n");
	dialog_.current_level->SetFont(font);
	dialog_.current_level->SetColor(sf::Color::Green);
	dialog_.current_level->SetSize(16);
	layout_left.Add(dialog_.current_level);

	dialog_.current_level_details = new gui::Label(this, "\n");
	dialog_.current_level_details->SetFont(font);
	dialog_.current_level_details->SetSize(12);
	layout_left.Add(dialog_.current_level_details);

	dialog_.next_level = new gui::Label(this, "\n");
	dialog_.next_level->SetColor(sf::Color::Green);
	dialog_.next_level->SetFont(font);
	dialog_.next_level->SetSize(16);
	layout_left.Add(dialog_.next_level);

	dialog_.next_level_details = new gui::Label(this, "\n");
	dialog_.next_level_details->SetFont(font);
	dialog_.next_level_details->SetSize(12);
	layout_left.Add(dialog_.next_level_details);

	gui::VBoxLayout layout_right(x + 200, y + 50);
	layout_right.SetSpacing(10);

	dialog_.but_buy = new ConfigButton(this, _t("armory.buy"));
	dialog_.but_buy->SetCallbackID(100);
	layout_right.Add(dialog_.but_buy);

	dialog_.but_back = new ConfigButton(this, _t("menu.cancel"));
	dialog_.but_back->SetCallbackID(101);
	layout_right.Add(dialog_.but_back);

	ShowDialog(false);
}


void ArmoryMenu::EventCallback(int id)
{
	switch (id)
	{
		case UpgradeItem::UP_ARMOR:
		case UpgradeItem::UP_ENGINE:
		case UpgradeItem::UP_LASER1:
		case UpgradeItem::UP_LASER2:
		case UpgradeItem::UP_SHIELD:
		case UpgradeItem::UP_HEATSINK:
			ShowDialog(true);
			LoadItem((UpgradeItem::Type) id);
			break;
		case 100:
		case 101:
			ShowDialog(false);
			break;
		case 102:
			Game::GetInstance().SetNextScene(Game::SC_LevelMenu);
			break;
	}
}


void ArmoryMenu::ShowDialog(bool visible)
{
	dialog_.background->SetVisible(visible);
	dialog_.lab_item->SetVisible(visible);
	dialog_.current_level->SetVisible(visible);
	dialog_.current_level_details->SetVisible(visible);
	dialog_.next_level->SetVisible(visible);
	dialog_.next_level_details->SetVisible(visible);
	dialog_.but_buy->SetVisible(visible);
	dialog_.but_back->SetVisible(visible);
}


void ArmoryMenu::LoadItem(UpgradeItem::Type type)
{
	std::wstring text = _t(UpgradeItem::TypeToString(type));
	dialog_.lab_item->SetText(text);

	int level = Game::GetInstance().GetPlayerSave().LevelOf(type);
	text = _t("armory.item_current_level");
	wstr_self_replace(text, L"{level}", to_wstring(level));
	dialog_.current_level->SetText(text);

	text = _t("armory.item_next_level");
	wstr_self_replace(text, L"{level}", to_wstring(level + 1));
	dialog_.next_level->SetText(text);

	text = _t(std::string(UpgradeItem::TypeToString(type)) + "_info");
	dialog_.current_level_details->SetText(text);

	text = _t("armory.price") + L"\n" + text;
	dialog_.next_level_details->SetText(text);
}

