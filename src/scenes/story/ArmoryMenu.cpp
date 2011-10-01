#include "ArmoryMenu.hpp"
#include "core/Game.hpp"
#include "utils/I18n.hpp"
#include "utils/StringUtils.hpp"
#include "scenes/ConfigButton.hpp"


ArmoryMenu::ArmoryMenu()
{
	SetTitle(_t("armory.title"));

	// ship GUI
	new gui::Image(this, GET_IMG("gui/armory-ship-view"));
	for (int i = 0; i < ItemData::_COUNT; ++i)
	{
		items_[i] = new UpgradeItem(this, (ItemData::Type) i);
	}

	// init dialog
	dialog_.background = new gui::Image(this, GET_IMG("gui/armory-dialog"));
	int x = dialog_.x = (Game::WIDTH - Dialog::WIDTH) / 2;
	int y = dialog_.y = (Game::HEIGHT - Dialog::HEIGHT) / 2;
	dialog_.background->SetPosition(x, y);


	dialog_.lab_item = new gui::Label(this, "\n");
	dialog_.lab_item->SetPosition(x, y + 6);
	// left side
	gui::VBoxLayout layout_left(x + 20, y + 40);
	layout_left.SetSpacing(5);
	const sf::Font& font = MediaManager::GetFont("Ubuntu-R.ttf");

	dialog_.current_level = new gui::Label(this, "\n");
	dialog_.current_level->SetColor(sf::Color::Green);
	dialog_.current_level->SetSize(18);
	layout_left.Add(dialog_.current_level);

	dialog_.current_level_details = new gui::Label(this, "\n");
	dialog_.current_level_details->SetFont(font);
	dialog_.current_level_details->SetSize(12);
	layout_left.Add(dialog_.current_level_details);

	dialog_.next_level = new gui::Label(this, "\n");
	dialog_.next_level->SetColor(sf::Color::Green);
	dialog_.next_level->SetSize(18);
	layout_left.Add(dialog_.next_level);

	dialog_.next_level_details = new gui::Label(this, "\n");
	dialog_.next_level_details->SetFont(font);
	dialog_.next_level_details->SetSize(12);
	layout_left.Add(dialog_.next_level_details);
	// right side: buttons
	gui::VBoxLayout layout_right(x + 220, y + 90);
	layout_right.SetSpacing(10);

	dialog_.but_buy = new ConfigButton(this, _t("armory.buy"));
	dialog_.but_buy->SetCallbackID(100);
	layout_right.Add(dialog_.but_buy);

	dialog_.but_back = new ConfigButton(this, _t("menu.cancel"));
	dialog_.but_back->SetCallbackID(101);
	layout_right.Add(dialog_.but_back);

	ShowDialog(false);

	gui::Button* but_back = new CosmoButton(this, _t("menu.back"), 210, 410);
	but_back->SetCallbackID(102);
}


void ArmoryMenu::EventCallback(int id)
{
	switch (id)
	{
		case ItemData::ARMOR:
		case ItemData::ENGINE:
		case ItemData::LASER1:
		case ItemData::LASER2:
		case ItemData::SHIELD:
		case ItemData::HEATSINK:
			ShowDialog(true);
			LoadItem((ItemData::Type) id);
			break;
		case 100:
			if (BuyItem((ItemData::Type) id))
			{
				ShowDialog(false);
			}
			break;
		case 101:
			ShowDialog(false);
			break;
		case 102:
			Game::GetInstance().SetNextScene(Game::SC_LevelMenu);
			break;
	}
}


bool ArmoryMenu::BuyItem(ItemData::Type type)
{
	/*switch (type)
	{
		case ItemData::UP_HEATSINK:
			ItemManager::GetInstance().GetHeatSink(dialog_.next_item_level);
			break;
		default:
			break;
	}*/
	return true;
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

	for (int i = 0; i < ItemData::_COUNT; ++i)
	{
		items_[i]->SetVisible(!visible);
	}
}


void ArmoryMenu::LoadItem(ItemData::Type type)
{
	std::wstring text = _t(UpgradeItem::TypeToString(type));
	dialog_.lab_item->SetText(text);
	int x = (Dialog::WIDTH - dialog_.lab_item->GetRect().GetWidth()) / 2;
	dialog_.lab_item->SetX(dialog_.x + x);

	// current item

	int level = Game::GetInstance().GetPlayerSave().LevelOf(type);
	text = _t("armory.item_current_level");
	wstr_self_replace(text, L"{level}", to_wstring(level));
	dialog_.current_level->SetText(text);

	text = _t(std::string(UpgradeItem::TypeToString(type)) + "_info");
	dialog_.current_level_details->SetText(text);

	// if last level already reached
	if (Game::GetInstance().GetPlayerSave().LevelOf(type) == ItemData::MAX_ITEM_LEVEL)
	{
		text = _t("armory.max_level");
		dialog_.next_level->SetText(text);
		dialog_.next_level_details->SetText("");
		dialog_.but_buy->SetVisible(false);
	}
	else
	{
		text = _t("armory.item_next_level");
		wstr_self_replace(text, L"{level}", to_wstring(level + 1));
		dialog_.next_level->SetText(text);

		text = _t("armory.price") + L"\n";
		text += _t(std::string(UpgradeItem::TypeToString(type)) + "_info");
		dialog_.next_level_details->SetText(text);
	}
}

