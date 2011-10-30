#include "ArmoryMenu.hpp"
#include "core/Game.hpp"
#include "core/SoundSystem.hpp"
#include "utils/I18n.hpp"
#include "utils/StringUtils.hpp"
#include "scenes/ConfigButton.hpp"
#include "items/ItemManager.hpp"


ArmoryMenu::ArmoryMenu()
{
	SetTitle(_t("armory.title"));
	current_type_ = ItemData::_UNSET;
	lab_info_ = new gui::Label(this, "");

	// ship GUI
	new gui::Image(this, Resources::GetImage("gui/armory-ship-view.png"));
	for (int i = 0; i < ItemData::_COUNT; ++i)
	{
		items_[i] = new UpgradeItem(this, (ItemData::Type) i);
	}

	// init dialog
	dialog_.background = new gui::Image(this, Resources::GetImage("gui/armory-dialog.png"));
	int x = dialog_.x = (Game::WIDTH - Dialog::WIDTH) / 2;
	int y = dialog_.y = (Game::HEIGHT - Dialog::HEIGHT) / 2;
	dialog_.background->SetPosition(x, y);


	dialog_.lab_item = new gui::Label(this, "\n");
	dialog_.lab_item->SetPosition(x, y + 6);
	// left side
	gui::VBoxLayout layout_left(x + 20, y + 40);
	layout_left.SetSpacing(0, 5);
	const sf::Font& font = Resources::GetFont("Ubuntu-R.ttf");

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
	layout_right.SetSpacing(0, 10);

	dialog_.but_buy = new ConfigButton(this, _t("armory.buy"));
	dialog_.but_buy->SetCallbackID(100);
	layout_right.Add(dialog_.but_buy);

	dialog_.but_back = new ConfigButton(this, _t("menu.cancel"));
	dialog_.but_back->SetCallbackID(101);
	layout_right.Add(dialog_.but_back);

	ShowDialog(false);

	gui::Button* but_back = new CosmoButton(this, _t("menu.back"));
	but_back->SetPosition(210, 410);
	but_back->SetCallbackID(102);
}


void ArmoryMenu::OnFocus()
{
	CreditCounterBase::OnFocus();
	lab_info_->SetText(_t("armory.info"));
	lab_info_->SetPosition((Game::WIDTH - lab_info_->GetWidth()) / 2, 366);
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
			if (BuyItem())
			{
				ShowDialog(false);
				std::wstring content = _t("armory.item_upgraded");
				int item_level = Game::GetInstance().GetPlayerSave().LevelOf(current_type_);
				wstr_self_replace(content, L"{item}", _t(ItemData::TypeToString(current_type_)));
				wstr_self_replace(content, L"{level}", to_wstring(item_level));
				lab_info_->SetText(content);
				lab_info_->SetX((Game::WIDTH - lab_info_->GetWidth()) / 2);
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


bool ArmoryMenu::BuyItem()
{
	PlayerSave& playersave = Game::GetInstance().GetPlayerSave();
	int level = playersave.LevelOf(current_type_) + 1;
	const ItemData* data = ItemManager::GetInstance().GetItemData(current_type_, level);

	if (playersave.GetCredits() >= data->GetPrice())
	{
		playersave.UpdateCredits(-data->GetPrice());
		playersave.SetItemLevel(current_type_, level);

		items_[current_type_]->RefreshLabel(); // refresh item widget
		CreditCounterBase::OnFocus(); // refresh credit counter

		SoundSystem::GetInstance().PlaySound(Resources::GetSoundBuffer("cash-register.wav"));
		return true;
	}
	SoundSystem::GetInstance().PlaySound(Resources::GetSoundBuffer("disabled.ogg"));
	return false;
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
	int level = Game::GetInstance().GetPlayerSave().LevelOf(type);
	const ItemData* data = ItemManager::GetInstance().GetItemData(type, level);
	current_type_ = type;

	// dialog title
	std::wstring text = _t(data->TypeToString());
	dialog_.lab_item->SetText(text);
	int x = (Dialog::WIDTH - dialog_.lab_item->GetWidth()) / 2;
	dialog_.lab_item->SetX(dialog_.x + x);

	// current item level
	text = _t("armory.item_current_level");
	wstr_self_replace(text, L"{level}", to_wstring(level));
	dialog_.current_level->SetText(text);

	// current item description
	dialog_.current_level_details->SetText(data->BuildDescriptionString());

	// if last level already reached
	if (level == ItemData::MAX_ITEM_LEVEL)
	{
		text = _t("armory.max_level");
		dialog_.next_level->SetText(text);
		dialog_.next_level_details->SetText("");
		dialog_.but_buy->SetVisible(false);
	}
	else
	{
		// next item level
		++level;
		data = ItemManager::GetInstance().GetItemData(type, level);

		text = _t("armory.item_next_level");
		wstr_self_replace(text, L"{level}", to_wstring(level));
		dialog_.next_level->SetText(text);

		// next item description
		dialog_.next_level_details->SetText(data->BuildDescriptionString(true));
	}
}
