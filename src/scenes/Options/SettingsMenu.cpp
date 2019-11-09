#include "SettingsMenu.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include "core/ControlPanel.hpp"
#include "utils/I18n.hpp"


SettingsMenu::SettingsMenu()
{
    setTitle(_t("settings.title"));

    m_form.SetOffset(90, 140);
    m_form.SetSpacing(10, 25);

    // Checkbox for vsync
    m_vsync = new gui::CheckBox(this);
    m_vsync->setCallback([this]() {
        Game::getInstance().setVerticalSync(m_vsync->Checked());
    });
    m_vsync->Check(Game::getInstance().isVerticalSync());
    m_form.addRow(_t("settings.vsync"), m_vsync);

    // OptionList for resolutions
    m_resolutions = new gui::OptionList<sf::Vector2u>(this);
    m_resolutions->setCallback([this]() {
        Game::getInstance().setResolution(m_resolutions->GetSelectedValue());
    });
    addResolution(APP_WIDTH, APP_HEIGHT);
    addResolution(APP_WIDTH * 2, APP_HEIGHT * 2);
    m_resolutions->SelectByValue(Game::getInstance().getWindow().getSize());
    m_form.addRow(_t("settings.resolution"), m_resolutions);

    // OptionList for language
    m_languages = new gui::OptionList<std::string>(this);
    m_languages->Add(L"English",  "en");
    m_languages->Add(L"Français", "fr");
    m_languages->Add(L"Deutsch",  "de");
    m_languages->SelectByValue(I18n::getInstance().getCurrentLanguage());
    m_languages->setCallback([this]() {
        I18n::getInstance().loadFromCode(m_languages->GetSelectedValue());
        // delete other scenes
        Game::getInstance().unloadScreens();
        ControlPanel::getInstance().refreshTextTranslations();
        // re-load i18ned texts
        setTitle(_t("settings.title"));
        m_form.getLabelAt(0)->setString(_t("settings.vsync"));
        m_form.getLabelAt(1)->setString(_t("settings.resolution"));
        m_form.getLabelAt(2)->setString(_t("settings.language"));
        m_form.recomputeLabelWidth();
        m_back->setString(_t("back"));
    });
    m_form.addRow(_t("settings.language"), m_languages);

    // Back button
    m_back = new CosmoButton(this, _t("back"));
    m_back->setCallback([this]() {
        Game::getInstance().setCurrentScreen(Game::SC_OptionMenu);
    });
    m_back->setPosition(210, 340);
}


void SettingsMenu::addResolution(size_t width, size_t height)
{
    m_resolutions->Add(
        std::to_string(width) + "x" + std::to_string(height),
        sf::Vector2u(width, height)
    );
}
