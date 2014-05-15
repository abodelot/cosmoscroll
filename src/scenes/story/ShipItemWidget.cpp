#include "ShipItemWidget.hpp"
#include "core/UserSettings.hpp"
#include "core/Resources.hpp"
#include "core/SoundSystem.hpp"
#include "items/ItemManager.hpp"
#include "utils/I18n.hpp"
#include "utils/SFML_Helper.hpp"


ShipItemWidget::ShipItemWidget(gui::Menu* parent, ItemData::Type type, const sf::Font& font):
	gui::Widget(parent, true),
	m_type(type)
{
	m_background.setTexture(Resources::getTexture("gui/armory-item.png"));
	m_background.setTextureRect(sf::IntRect(0, 0, 340, 60));

	sf::Vector2f size = sfh::size(m_background);

	// Item name
	m_txt_name.setFont(font);
	//m_name.setStyle(sf::Text::Bold);
	m_txt_name.setCharacterSize(16);
	m_txt_name.setPosition(10, 5);
	m_txt_name.setString(_t(ItemData::TypeToString(m_type)));

	// Item current level
	m_txt_level.setFont(font);
	m_txt_level.setCharacterSize(10);
	m_txt_level.setColor(sf::Color::White);
	m_txt_level.setPosition(75, 24);

	// Item description
	m_txt_description.setFont(font);
	m_txt_description.setCharacterSize(10);
	m_txt_description.setColor(sf::Color::White);
	m_txt_description.setPosition(75, 40);

	m_txt_upgrade.setFont(font);
	m_txt_upgrade.setCharacterSize(12);

	m_txt_price.setFont(font);
	m_txt_price.setCharacterSize(12);

	refresh();

	Resize(size.x, size.y);
}


void ShipItemWidget::OnStateChanged(gui::State::EState state)
{
	switch (state)
	{
	case gui::State::DEFAULT:
		m_background.setTextureRect(sf::IntRect(0, 0, 340, 60));
		m_txt_name.setColor(sf::Color::White);
		break;
	case gui::State::FOCUSED:
		m_background.setTextureRect(sf::IntRect(0, 60, 340, 60));
		m_txt_name.setColor(sf::Color(0, 255, 255));
		SoundSystem::GetInstance().PlaySound(Resources::getSoundBuffer("menu-select.ogg"));
		break;
	default:
		break;
	}
}

void ShipItemWidget::OnKeyPressed(sf::Keyboard::Key code)
{
	if (code == sf::Keyboard::Return)
	{
		if (buyNextLevel())
			CallTheCallback();
	}
}


void ShipItemWidget::OnMouseClicked(int, int)
{
	if (buyNextLevel())
		CallTheCallback();
}


void ShipItemWidget::refresh()
{
	// Get current item level
	m_level = UserSettings::getItemLevel(m_type);
	m_txt_level.setString(I18n::templatize("armory.item_level", "{level}", m_level));
	const ItemData* data = ItemManager::GetInstance().GetItemData(m_type, m_level);
	m_txt_description.setString(data->BuildDescriptionString());

	// Get next item level
	int next_level = m_level + 1;
	data = ItemManager::GetInstance().GetItemData(m_type, next_level);

	// No next item => last level reached
	if (data == NULL)
	{
		m_txt_upgrade.setString(_t("armory.max_level"));
		m_txt_upgrade.setStyle(sf::Text::Italic);
		m_txt_upgrade.setColor(sf::Color(255, 255, 255, 128));
		m_txt_price.setString("");
	}
	else
	{
		m_txt_upgrade.setString(I18n::templatize("armory.upgrade_item", "{level}", next_level));
		m_txt_price.setString(I18n::templatize("item.price", "{price}", data->getPrice()));
		if (UserSettings::getCredits() >= data->getPrice())
		{
			m_txt_price.setColor(sf::Color::Green);
		}
		else
		{
			m_txt_price.setColor(sf::Color(255, 128, 0));

		}
	}
	m_txt_upgrade.setPosition(270 - int(sfh::width(m_txt_upgrade)) / 2, 14);
	m_txt_price.setPosition(270 - int(sfh::width(m_txt_price)) / 2, 32);

}


bool ShipItemWidget::buyNextLevel() const
{
	int next_level = m_level + 1;
	const ItemData* data = ItemManager::GetInstance().GetItemData(m_type, next_level);

	if (data && UserSettings::getCredits() >= data->getPrice())
	{
		UserSettings::updateCredits(-data->getPrice());
		UserSettings::setItemLevel(m_type, next_level);

		SoundSystem::GetInstance().PlaySound(Resources::getSoundBuffer("cash-register.ogg"));
		return true;
	}
	SoundSystem::GetInstance().PlaySound(Resources::getSoundBuffer("disabled.ogg"));
	return false;
}


void ShipItemWidget::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_background, states);
	target.draw(m_txt_name, states);
	target.draw(m_txt_level, states);
	target.draw(m_txt_description, states);
	target.draw(m_txt_price, states);
	target.draw(m_txt_upgrade, states);

	// Draw step lights
	static sf::Sprite step(Resources::getTexture("gui/armory-step.png"));
	// Start with "light on" texture
	step.setTextureRect(sf::IntRect(0, 0, 16, 17));
	for (int i = 0; i < 3; ++i)
	{
		// Switch to "light off" texture
		if (i == m_level)
			step.setTextureRect(sf::IntRect(0, 17, 16, 17));

		step.setPosition(10 + i * 20, 32);
		target.draw(step, states);
	}
}
