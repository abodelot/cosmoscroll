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
    m_form.SetLabelAlignment(gui::Align::RIGHT);

    m_vsync = new gui::CheckBox(this);
    m_vsync->SetCallbackID(1);
    m_vsync->Check(Game::getInstance().isVerticalSync());
    m_form.AddRow(_t("settings.vsync"), m_vsync);

    m_resolutions = new gui::OptionList<sf::Vector2u>(this);
    m_resolutions->SetCallbackID(2);
    addResolution(APP_WIDTH, APP_HEIGHT);
    addResolution(APP_WIDTH * 2, APP_HEIGHT * 2);
    m_resolutions->SelectByValue(Game::getInstance().getWindow().getSize());
    m_form.AddRow(_t("settings.resolution"), m_resolutions);

    m_languages = new gui::OptionList<std::string>(this);
    m_languages->Add(L"English",  "en");
    m_languages->Add(L"Français", "fr");
    m_languages->Add(L"Deutsch",  "de");
    m_languages->SelectByValue(I18n::getInstance().getCurrentLanguage());
    m_languages->SetCallbackID(3);
    m_form.AddRow(_t("settings.language"), m_languages);

    m_back = new CosmoButton(this, _t("back"));
    m_back->SetCallbackID(4);
    m_back->setPosition(210, 340);
}


void SettingsMenu::EventCallback(int id)
{
    switch (id)
    {
        case 1:
            Game::getInstance().setVerticalSync(m_vsync->Checked());
            break;
        case 2:
            Game::getInstance().setResolution(m_resolutions->GetSelectedValue());
            break;
        case 3:
            I18n::getInstance().loadFromCode(m_languages->GetSelectedValue());
            // delete other scenes
            Game::getInstance().unloadScreens();
            ControlPanel::getInstance().refreshTextTranslations();
            // re-load i18ned texts
            setTitle(_t("settings.title"));
            m_form.GetLabelAt(0)->setString(_t("settings.vsync"));
            m_form.GetLabelAt(1)->setString(_t("settings.resolution"));
            m_form.GetLabelAt(2)->setString(_t("settings.language"));
            m_form.AlignRows();
            m_back->setString(_t("back"));
            break;
        case 4:
            Game::getInstance().setCurrentScreen(Game::SC_OptionMenu);
            break;
    }
}


void SettingsMenu::addResolution(size_t width, size_t height)
{
    std::ostringstream oss;
    oss << width << "x" << height;
    m_resolutions->Add(oss.str(), sf::Vector2u(width, height));
}
