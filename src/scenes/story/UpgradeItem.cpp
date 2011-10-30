#include "UpgradeItem.hpp"
#include "core/Game.hpp"
#include "utils/Resources.hpp"
#include "utils/I18n.hpp"
#include "utils/StringUtils.hpp"


UpgradeItem::UpgradeItem(gui::Menu* parent, ItemData::Type type):
	gui::Widget(parent, true)
{
	type_ = type;
	int x = 0, y = 0;
	switch (type)
	{
		case ItemData::LASER1:
			halo_.SetImage(Resources::GetImage("gui/halo-laser1.png"));
			x = 496;
			y = 249;
			break;
		case ItemData::LASER2:
			halo_.SetImage(Resources::GetImage("gui/halo-laser2.png"));
			x = 494;
			y = 263;
			break;
		case ItemData::ENGINE:
			halo_.SetImage(Resources::GetImage("gui/halo-engine.png"));
			x = 143;
			y = 224;
			break;
		case ItemData::ARMOR:
			halo_.SetImage(Resources::GetImage("gui/halo-armor.png"));
			x = 419;
			y = 176;
			break;
		case ItemData::SHIELD:
			halo_.SetImage(Resources::GetImage("gui/halo-shield.png"));
			x = 207;
			y = 244;
			break;
		case ItemData::HEATSINK:
			halo_.SetImage(Resources::GetImage("gui/halo-heatsink.png"));
			x = 361;
			y = 255;
			break;
		default:
			break;
	}
	label_.SetPosition(0, -20);
	label_.SetFont(Resources::GetFont("Ubuntu-R.ttf"));
	label_.SetSize(12);
	label_.SetColor(sf::Color::White);
	RefreshLabel();

	label_bg_.Resize(label_.GetRect().GetWidth() + 8, label_.GetRect().GetHeight() + 8);
	label_bg_.SetPosition(label_.GetPosition().x - 4, label_.GetPosition().y - 4);
	label_bg_.SetColor(sf::Color(0, 0, 0, 128));

	SetPosition(x, y);
	Resize(halo_.GetSize().x, halo_.GetSize().y);
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


void UpgradeItem::RefreshLabel()
{
	std::wstring content = _t(ItemData::TypeToString(type_));
	content += L"\n";
	content += _t("armory.item_level");
	wstr_self_replace(content, L"{level}", to_wstring(Game::GetInstance().GetPlayerSave().LevelOf(type_)));

	label_.SetText(content);
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
		default:
			break;
	}
}
