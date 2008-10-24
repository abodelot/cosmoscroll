#include "GameOver.hpp"
#include "MediaManager.hpp"
#include "Menu.hpp"


GameOver::GameOver(sf::RenderWindow& app) :
	Screen(app)
{
}


Screen::Choice GameOver::Run()
{
    sf::String title;
    title.SetText("T'as perdu, t'es trop naze");
    title.SetFont(GET_FONT());
    title.SetColor(sf::Color::White);
    title.SetPosition(42, 42);
    
    Menu menu;
    menu.SetOffset(sf::Vector2f(42, 100));
    menu.AddItem("Rejouer", Screen::GAME);
    menu.AddItem("Quitter", Screen::EXIT_APP);
    
    bool running = true;
    int choice;
    
    sf::Event event;
    while (running)
    {
        while (app_.GetEvent(event))
        {
            if (event.Type == sf::Event::Closed)
            {
                running = false;
            }
            else if (event.Type == sf::Event::KeyPressed)
            {
                if (menu.ActionChosen(event.Key, choice))
                {
                    running = false;
                }
            }
        }
        app_.Draw(title);
        menu.Show(app_);
        app_.Display();
    }
    return static_cast<Screen::Choice>(choice);
}

