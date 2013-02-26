#include "UpgradeItem.hpp"
#include "core/Game.hpp"
#include "core/Resources.hpp"
#include "utils/I18n.hpp"
#include "utils/StringUtils.hpp"


UpgradeItem::UpgradeItem(gui::Menu* parent, ItemData::Type type):
	gui::Widget(parent, true)
{
	type_ = type;
	int x = 0, y = 0;
	switch (type)
	{
		case ItemData::WEAPON:
			halo_.setTexture(Resources::getTexture("gui/halo-laser1.png"));
			x = 496;
			y = 252;
			break;
		case ItemData::ENGINE:
			halo_.setTexture(Resources::getTexture("gui/halo-engine.png"));
			x = 143;
			y = 224;
			break;
		case ItemData::ARMOR:
			halo_.setTexture(Resources::getTexture("gui/halo-armor.png"));
			x = 419;
			y = 176;
			break;
		case ItemData::SHIELD:
			halo_.setTexture(Resources::getTexture("gui/halo-shield.png"));
			x = 207;
			y = 244;
			break;
		case ItemData::HEATSINK:
			halo_.setTexture(Resources::getTexture("gui/halo-heatsink.png"));
			x = 361;
			y = 255;
			break;
		default:
			break;
	}
	setPosition(x, y);
	Resize(halo_.getWidth(), halo_.getHeight());
	SetCallbackID(type);

	// label centered on halo sprite
	label_.setFont(Resources::getFont("Ubuntu-R.ttf"));
	label_.setCharacterSize(12);
	label_.setColor(sf::Color::White);
	RefreshLabel();
	x = (GetWidth() - label_.getWidth()) / 2;
	label_.setPosition(x, -20);

	// label background
	label_bg_.setSize(sf::Vector2f(label_.getWidth() + 8, label_.getHeight() + 10));
	label_bg_.setFillColor(sf::Color(0, 0, 0, 128));
	label_bg_.setPosition(label_.getPosition().x - 4, label_.getPosition().y - 4);
}


void UpgradeItem::OnKeyPressed(sf::Keyboard::Key code)
{
	if (code == sf::Keyboard::Return)
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
	wstr_self_replace(content, L"{level}", std::to_wstring(Game::GetInstance().GetPlayerSave().LevelOf(type_)));

	label_.setString(content);
}


void UpgradeItem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	switch (GetState())
	{
		case gui::State::HOVERED:
		case gui::State::FOCUSED:
			target.draw(halo_, states);
			target.draw(label_bg_, states);
			target.draw(label_, states);
			break;
		default:
			break;
	}
}
