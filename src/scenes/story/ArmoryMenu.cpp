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
	current_type_ = ItemData::_UNSET;
	lab_info_ = new gui::Label(this, "");

	const sf::Font& font = Resources::getFont("Vera.ttf");
	// ship GUI
	new gui::Image(this, Resources::getTexture("gui/armory-ship-view.png"));
	for (int i = 0; i < ItemData::_COUNT; ++i)
	{
		items_[i] = new ShipPartWidget(this, (ItemData::Type) i, font);
	}

	m_credits = new CreditCounterWidget(this);

	// init dialog
	dialog_.background = new gui::Image(this, Resources::getTexture("gui/armory-dialog.png"));
	int x = dialog_.x = (APP_WIDTH - Dialog::WIDTH) / 2;
	int y = dialog_.y = (APP_HEIGHT - Dialog::HEIGHT) / 2;
	dialog_.background->setPosition(x, y);


	dialog_.lab_item = new gui::Label(this, "A\n");
	dialog_.lab_item->setPosition(x, y + 6);
	// left side
	gui::VBoxLayout layout_left(x + 30, y + 40);
	layout_left.SetSpacing(0, 1);


	dialog_.current_level = new gui::Label(this, "1");
	dialog_.current_level->setFont(font);
	dialog_.current_level->setStyle(sf::Text::Bold);
	dialog_.current_level->setColor(sf::Color::Green);
	dialog_.current_level->setCharacterSize(12);
	layout_left.Add(dialog_.current_level);

	dialog_.current_level_details = new gui::Label(this, "1\n2\n3");
	dialog_.current_level_details->setFont(font);
	dialog_.current_level_details->setCharacterSize(12);
	layout_left.Add(dialog_.current_level_details);

	dialog_.next_level = new gui::Label(this, "1");
	dialog_.next_level->setFont(font);
	dialog_.next_level->setStyle(sf::Text::Bold);
	dialog_.next_level->setColor(sf::Color::Green);
	dialog_.next_level->setCharacterSize(12);
	layout_left.Add(dialog_.next_level);

	dialog_.next_level_details = new gui::Label(this, "1\n2\n3s");
	dialog_.next_level_details->setFont(font);
	dialog_.next_level_details->setCharacterSize(12);
	layout_left.Add(dialog_.next_level_details);

	// right side: buttons
	gui::VBoxLayout layout_right(x + 210, y + 70);
	layout_right.SetSpacing(0, 10);

	dialog_.price = new gui::Label(this, "A\n");
	dialog_.price->setFont(font);
	dialog_.price->setCharacterSize(14);
	layout_right.Add(dialog_.price);

	dialog_.but_buy = new ConfigButton(this);
	dialog_.but_buy->setString(_t("armory.buy"));
	dialog_.but_buy->SetCallbackID(100);
	layout_right.Add(dialog_.but_buy);

	dialog_.but_back = new ConfigButton(this);
	dialog_.but_back->setString(_t("menu.cancel"));
	dialog_.but_back->SetCallbackID(101);
	layout_right.Add(dialog_.but_back);

	ShowDialog(false);

	gui::Button* but_back = new CosmoButton(this, _t("menu.back"));
	but_back->setPosition(210, 410);
	but_back->SetCallbackID(102);
}


void ArmoryMenu::OnFocus()
{
	m_credits->setCredits(UserSettings::getCredits());
	lab_info_->setString(_t("armory.info"));
	lab_info_->setPosition((APP_WIDTH - lab_info_->GetWidth()) / 2, 366);
}


void ArmoryMenu::EventCallback(int id)
{
	switch (id)
	{
		case ItemData::HULL:
		case ItemData::ENGINE:
		case ItemData::WEAPON:
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
				int item_level = UserSettings::getItemLevel(current_type_);
				wstr_self_replace(content, L"{item}", _t(ItemData::TypeToString(current_type_)));
				wstr_self_replace(content, L"{level}", std::to_wstring(item_level));
				lab_info_->setString(content);
				lab_info_->setPosition((APP_WIDTH - lab_info_->GetWidth()) / 2, lab_info_->getPosition().y);
				FocusFirstWidget();
			}
			break;
		case 101:
			ShowDialog(false);
			FocusFirstWidget();
			break;
		case 102:
			Game::getInstance().setNextScene(Game::SC_LevelMenu);
			break;
	}
}


bool ArmoryMenu::BuyItem()
{
	int level = UserSettings::getItemLevel(current_type_) + 1;
	const ItemData* data = ItemManager::GetInstance().GetItemData(current_type_, level);

	if (UserSettings::getCredits() >= data->getPrice())
	{
		UserSettings::updateCredits(-data->getPrice());
		UserSettings::setItemLevel(current_type_, level);

		items_[current_type_]->RefreshLabel(); // refresh item widget
		m_credits->setCredits(UserSettings::getCredits());

		SoundSystem::GetInstance().PlaySound(Resources::getSoundBuffer("cash-register.ogg"));
		return true;
	}
	SoundSystem::GetInstance().PlaySound(Resources::getSoundBuffer("disabled.ogg"));
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
	dialog_.price->SetVisible(visible);
	dialog_.but_buy->SetVisible(visible);
	dialog_.but_back->SetVisible(visible);

	for (int i = 0; i < ItemData::_COUNT; ++i)
	{
		items_[i]->SetVisible(!visible);
	}
}


void ArmoryMenu::LoadItem(ItemData::Type type)
{
	int level = UserSettings::getItemLevel(type);
	const ItemData* data = ItemManager::GetInstance().GetItemData(type, level);
	current_type_ = type;

	// dialog title
	dialog_.lab_item->setString(_t(data->TypeToString()));
	int x = (Dialog::WIDTH - dialog_.lab_item->GetWidth()) / 2;
	dialog_.lab_item->setPosition(dialog_.x + x, dialog_.lab_item->getPosition().y);

	// current item level
	dialog_.current_level->setString(I18n::templatize("armory.item_current_level", "{level}", level));

	// current item description
	dialog_.current_level_details->setString(data->BuildDescriptionString());

	// next item level
	++level;
	data = ItemManager::GetInstance().GetItemData(type, level);

	// last level reached
	if (data == NULL)
	{
		dialog_.next_level->setString(_t("armory.max_level"));
		dialog_.next_level_details->setString("");
		dialog_.price->setString("");
		dialog_.but_buy->SetVisible(false);
	}
	else
	{
		dialog_.next_level->setString(I18n::templatize("armory.item_next_level", "{level}", level));

		// next item description
		dialog_.next_level_details->setString(data->BuildDescriptionString());
		dialog_.price->setString(I18n::templatize("item.price", "{price}", data->getPrice()));
		if (UserSettings::getCredits() >= data->getPrice())
			dialog_.price->setColor(sf::Color::White);
		else
			dialog_.price->setColor(sf::Color(255, 128, 0));
	}
	FocusFirstWidget();
}
