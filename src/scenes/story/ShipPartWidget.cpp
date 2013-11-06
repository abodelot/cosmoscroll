#include "ShipPartWidget.hpp"
#include "core/Game.hpp"
#include "core/Resources.hpp"
#include "utils/I18n.hpp"
#include "utils/StringUtils.hpp"


ShipPartWidget::ShipPartWidget(gui::Menu* parent, ItemData::Type type):
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
	m_text_name.setFont(Resources::getFont("Ubuntu-R.ttf"));
	m_text_name.setStyle(sf::Text::Bold);
	m_text_name.setCharacterSize(12);
	m_text_name.setColor(sf::Color::White);

	m_text_level.setFont(Resources::getFont("Ubuntu-R.ttf"));
	m_text_level.setCharacterSize(12);
	m_text_level.setColor(sf::Color::Yellow);

	RefreshLabel();
	x = (GetWidth() - m_text_name.getWidth()) / 2;
	m_text_name.setPosition(x, -30);
	m_text_level.setPosition(x, -14);

	// label background
	label_bg_.setSize(sf::Vector2f(m_text_name.getWidth() + 8, m_text_name.getHeight() + m_text_level.getHeight() + 10));
	label_bg_.setFillColor(sf::Color(0, 0, 0, 128));
	label_bg_.setPosition(m_text_name.getPosition() - sf::Vector2f(4, 4));
}


void ShipPartWidget::OnKeyPressed(sf::Keyboard::Key code)
{
	if (code == sf::Keyboard::Return)
	{
		CallTheCallback();
	}
}


void ShipPartWidget::OnMouseClicked(int, int)
{
	CallTheCallback();
}


void ShipPartWidget::RefreshLabel()
{
	m_text_name.setString(_t(ItemData::TypeToString(type_)));
	m_text_level.setString(I18n::templatize("armory.item_level", "{level}", Game::getInstance().getPlayerSave().LevelOf(type_)));
}


void ShipPartWidget::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	switch (GetState())
	{
		case gui::State::HOVERED:
		case gui::State::FOCUSED:
			target.draw(halo_, states);
			target.draw(label_bg_, states);
			target.draw(m_text_name, states);
			target.draw(m_text_level, states);
			break;
		default:
			break;
	}
}
