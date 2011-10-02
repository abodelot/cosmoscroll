#include "UpgradeItem.hpp"
#include "core/Game.hpp"
#include "utils/MediaManager.hpp"
#include "utils/I18n.hpp"
#include "utils/StringUtils.hpp"


UpgradeItem::UpgradeItem(gui::Menu* parent, ItemData::Type type):
	gui::Widget(parent, true)
{
	int x = 0, y = 0;
	switch (type)
	{
		case ItemData::LASER1:
			halo_.SetImage(GET_IMG("gui/halo-laser1"));
			x = 496;
			y = 249;
			break;
		case ItemData::LASER2:
			halo_.SetImage(GET_IMG("gui/halo-laser2"));
			x = 494;
			y = 263;
			break;
		case ItemData::ENGINE:
			halo_.SetImage(GET_IMG("gui/halo-engine"));
			x = 143;
			y = 224;
			break;
		case ItemData::ARMOR:
			halo_.SetImage(GET_IMG("gui/halo-armor"));
			x = 419;
			y = 176;
			break;
		case ItemData::SHIELD:
			halo_.SetImage(GET_IMG("gui/halo-shield"));
			x = 207;
			y = 244;
			break;
		case ItemData::HEATSINK:
			halo_.SetImage(GET_IMG("gui/halo-heatsink"));
			x = 361;
			y = 255;
			break;
		default:
			break;
	}
	std::wstring content = _t(ItemData::TypeToString(type));
	content += L"\n";
	content += _t("armory.item_level");
	wstr_self_replace(content, L"{level}", to_wstring(Game::GetInstance().GetPlayerSave().LevelOf(type)));

	label_.SetText(content);

	label_.SetPosition(0, -20);
	label_.SetFont(MediaManager::GetFont("Ubuntu-R.ttf"));
	label_.SetSize(12);
	label_.SetColor(sf::Color::White);

	label_bg_.Resize(label_.GetRect().GetWidth() + 8, label_.GetRect().GetHeight() + 8);
	label_bg_.SetPosition(label_.GetPosition().x - 4, label_.GetPosition().y - 4);
	label_bg_.SetColor(sf::Color(0, 0, 0, 128));

	SetRect(x, y, x + halo_.GetSize().x, y + halo_.GetSize().y);
	SetCallbackID(type);
}


void UpgradeItem::OnKeyPressed(sf::Key::Code code)
{
	if (code == sf::Key::Return)
	{
		CallTheCallback();
	}
}


void UpgradeItem::OnMouseClicked(int, int)
{
	CallTheCallback();
}


void UpgradeItem::Render(sf::RenderTarget& target) const
{

	switch (GetState())
	{
		case gui::State::HOVERED:
		case gui::State::FOCUSED:
			target.Draw(halo_);
			target.Draw(label_bg_);
			target.Draw(label_);
			break;
	}

}
