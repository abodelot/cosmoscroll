#include "ShipItemWidget.hpp"
#include "core/UserSettings.hpp"
#include "core/Resources.hpp"
#include "core/SoundSystem.hpp"
#include "core/Factory.hpp"
#include "core/Services.hpp"
#include "utils/SFML_Helper.hpp"

static constexpr int WIDTH = 340;
static constexpr int HEIGHT = 60;

ShipItemWidget::ShipItemWidget(gui::Menu* parent, Item::Type type, const sf::Font& font):
    gui::Widget(parent, true),
    m_type(type)
{
    m_background.setTexture(Resources::getTexture("gui/armory-item.png"));
    m_background.setTextureRect(sf::IntRect(0, 0, WIDTH, HEIGHT));

    // Item name
    m_txtName.setFont(font);
    m_txtName.setCharacterSize(16);
    m_txtName.setPosition(10, 5);
    m_txtName.setString(_t(Item::typeToString(m_type)));
    m_txtName.setOutlineColor(sf::Color(0, 0, 0, 128));
    m_txtName.setOutlineThickness(1.f);

    // Item current level
    m_txtLevel.setFont(font);
    m_txtLevel.setCharacterSize(10);
    m_txtLevel.setFillColor(sf::Color::White);
    m_txtLevel.setPosition(75, 24);

    // Item description
    m_txtDescription.setFont(font);
    m_txtDescription.setCharacterSize(10);
    m_txtDescription.setFillColor(sf::Color::White);
    m_txtDescription.setPosition(75, 40);

    m_txtUpgrade.setFont(font);
    m_txtUpgrade.setCharacterSize(12);

    m_txtPrice.setFont(font);
    m_txtPrice.setCharacterSize(12);

    refresh();

    resize(WIDTH, HEIGHT);
}


void ShipItemWidget::onStateChanged(gui::State::EState state)
{
    switch (state) {
        case gui::State::DEFAULT:
            m_background.setTextureRect(sf::IntRect(0, 0, WIDTH, HEIGHT));
            m_txtName.setFillColor(sf::Color::White);
            break;
        case gui::State::FOCUSED:
            m_background.setTextureRect(sf::IntRect(0, HEIGHT, WIDTH, HEIGHT));
            m_txtName.setFillColor(sf::Color(0, 255, 255));
            Services::getSoundSystem().playSound("menu-select.ogg");
            break;
        default:
            break;
    }
}


void ShipItemWidget::onKeyPressed(sf::Keyboard::Key code)
{
    if (code == sf::Keyboard::Return) {
        if (buyNextLevel()) {
            triggerCallback();
        }
    }
}


void ShipItemWidget::onMouseClicked(int, int)
{
    if (buyNextLevel()) {
        triggerCallback();
    }
}


void ShipItemWidget::refresh()
{
    Factory& factory = Services::getFactory();

    // Get current item level
    m_level = UserSettings::getItemLevel(m_type);
    m_txtLevel.setString(_t("armory.item_level", "{level}", m_level));
    const Item& item = factory.getItem(m_type, m_level);
    m_txtDescription.setString(item.getDescription());

    // Check for next item level
    int next_level = m_level + 1;
    if (factory.hasItem(m_type, next_level)) {
        const Item& next_item = factory.getItem(m_type, next_level);
        m_txtUpgrade.setString(_t("armory.upgrade_item", "{level}", next_level));
        m_txtPrice.setString(_t("item.price", "{price}", next_item.getPrice()));
        if (UserSettings::getCredits() >= next_item.getPrice()) {
            m_txtPrice.setFillColor(sf::Color::Green);
        } else {
            m_txtPrice.setFillColor(sf::Color(255, 200, 0));
        }
    } else {
        // No next item => last level reached
        m_txtUpgrade.setString(_t("armory.max_level"));
        m_txtUpgrade.setStyle(sf::Text::Italic);
        m_txtUpgrade.setFillColor(sf::Color(255, 255, 255, 128));
        m_txtPrice.setString("");
    }
    m_txtUpgrade.setPosition(270 - int(sfh::width(m_txtUpgrade)) / 2, 14);
    m_txtPrice.setPosition(270 - int(sfh::width(m_txtPrice)) / 2, 32);

}

bool ShipItemWidget::buyNextLevel() const
{
    Factory& factory = Services::getFactory();
    // Check for next item
    int next_level = m_level + 1;
    if (factory.hasItem(m_type, next_level)) {
        const Item& item = factory.getItem(m_type, next_level);
        if (UserSettings::getCredits() >= item.getPrice()) {
            UserSettings::updateCredits(-item.getPrice());
            UserSettings::setItemLevel(m_type, next_level);

            Services::getSoundSystem().playSound("cash-register.ogg");
            return true;
        }
    }

    Services::getSoundSystem().playSound("disabled.ogg");
    return false;
}

void ShipItemWidget::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_background, states);
    target.draw(m_txtName, states);
    target.draw(m_txtLevel, states);
    target.draw(m_txtDescription, states);
    target.draw(m_txtPrice, states);
    target.draw(m_txtUpgrade, states);

    // Draw step lights
    static sf::Sprite step(Resources::getTexture("gui/armory-step.png"));
    // Start with "light on" texture
    step.setTextureRect(sf::IntRect(0, 0, 16, 17));
    for (int i = 0; i < 3; ++i) {
        // Switch to "light off" texture
        if (i == m_level) {
            step.setTextureRect(sf::IntRect(0, 17, 16, 17));
        }

        step.setPosition(10 + i * 20, 32);
        target.draw(step, states);
    }
}
