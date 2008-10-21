#include "Intro.hpp"
#include "MediaManager.hpp"
#include "Window.hpp"


Intro::Intro(sf::RenderWindow& app) :
	Screen(app)
{   
    background_.SetImage(GET_IMG("background"));
}

Screen::Choice Intro::Run() 
{
    Screen::Choice choice = Screen::GAME;
    
	sf::String title;
	title.SetText("CosmoScroll");
	title.SetFont(GET_FONT());
	title.SetSize(60);
	title.SetColor(sf::Color::White);
	sf::FloatRect rect = title.GetRect();
	title.SetPosition((WIN_WIDTH - rect.GetWidth()) / 2, 42);
	
	sf::Sprite ship(GET_IMG("spaceship"));
	ship.SetPosition(-20, 100);
	
	sf::Event event;
	bool running = true;
	while (running)
	{
		while (app_.GetEvent(event)) 
		{
		    if (event.Type == sf::Event::Closed)
		    {
		        running = false;
		        choice = Screen::EXIT_APP;
		    }
		    else if (event.Type == sf::Event::KeyPressed)
		    {
			    if (event.Key.Code == sf::Key::Return)
				    running = false;
				else if (event.Key.Code == sf::Key::Escape)
				{
				    running = false;
				    choice = Screen::EXIT_APP;
				}
			}
		}
		float time = app_.GetFrameTime();
		ship.Move(180 * time, 25 * time);
		if (ship.GetPosition().x > WIN_WIDTH)
		{
		    running = false;
		}
		app_.Draw(background_);
		app_.Draw(ship);
		app_.Draw(title);
		app_.Display();
	}
	return choice;
}
