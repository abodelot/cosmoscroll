#include "ArmoryMenu.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include "core/UserSettings.hpp"
#include "core/Resources.hpp"
#include "core/SoundSystem.hpp"
#include "utils/I18n.hpp"
#include "scenes/ConfigButton.hpp"


ArmoryMenu::ArmoryMenu()
{
    setTitle(_t("armory.title"));

    // Upgrade item
    gui::VBoxLayout layout(100, 90);
    layout.setSpacing(0, 0);
    const sf::Font& font = Resources::getFont("Vera.ttf");
    for (int i = 0; i < Item::_COUNT; ++i) {
        m_items[i] = new ShipItemWidget(this, (Item::Type) i, font);
        m_items[i]->setCallback([this]() {
            onFocus();
        });
        layout.add(m_items[i]);
    }

    // Credit counter
    m_credits = new CreditCounterWidget(this);

    // Back button
    gui::Button* but_back = new CosmoButton(this, _t("back"));
    but_back->setPosition(210, 410);
    but_back->setCallback([]() {
        Game::getInstance().setCurrentScreen(Game::SC_LevelMenu);
    });
}

void ArmoryMenu::onFocus()
{
    // Refresh UI
    m_credits->setCredits(UserSettings::getCredits());
    for (int i = 0; i < Item::_COUNT; ++i) {
        m_items[i]->refresh();
    }
}
