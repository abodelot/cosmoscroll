#include "UpgradeItem.hpp"
#include "utils/MediaManager.hpp"
#include "utils/I18n.hpp"


UpgradeItem::UpgradeItem(gui::Menu* parent, Type type):
	gui::Widget(parent, true)
{
	int x = 0, y = 0;
	switch (type)
	{
		case UP_LASER1:
			halo_.SetImage(GET_IMG("gui/halo-laser1"));
			label_.SetText(_t("armory.laser1"));
			x = 496;
			y = 249;
			break;
		case UP_LASER2:
			halo_.SetImage(GET_IMG("gui/halo-laser2"));
			label_.SetText(_t("armory.laser2"));
			x = 494;
			y = 263;
			break;
		case UP_ENGINE:
			halo_.SetImage(GET_IMG("gui/halo-engine"));
			label_.SetText(_t("armory.engine"));
			x = 143;
			y = 224;
			break;
		case UP_ARMOR:
			halo_.SetImage(GET_IMG("gui/halo-armor"));
			label_.SetText(_t("armory.armor"));
			x = 419;
			y = 176;
			break;
		case UP_SHIELD:
			halo_.SetImage(GET_IMG("gui/halo-shield"));
			label_.SetText(_t("armory.shield"));
			x = 207;
			y = 244;
			break;
		case UP_HEATSINK:
			halo_.SetImage(GET_IMG("gui/halo-heatsink"));
			label_.SetText(_t("armory.heatsink"));
			x = 361;
			y = 255;
			break;
		default:
			break;
	}
	label_.SetPosition(0, -16);
	label_.SetSize(14);
	label_.SetColor(sf::Color::Yellow);
	SetRect(x, y, x + halo_.GetSize().x, y + halo_.GetSize().y);
}

void UpgradeItem::Render(sf::RenderTarget& target) const
{

	switch (GetState())
	{
		case gui::State::HOVERED:
			target.Draw(halo_);
			break;
		case gui::State::FOCUSED:
			target.Draw(halo_);
			target.Draw(label_);
			break;
	}

}
