#include "Window.hpp"
#include "Intro.hpp"
#include "GameOver.hpp"
#include "Game.hpp"


int main()
{
    // mise en place fenêtre de rendu
    sf::RenderWindow app(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT, WIN_BPP), WIN_TITLE);//,sf::Style::Fullscreen);
    app.SetFramerateLimit(WIN_BPP);
    app.ShowMouseCursor(false);
    
    // sélecteur pour gérer plusieurs scènes
    Screen* selector[Screen::NB_SCREENS];
    selector[Screen::INTRO] = new Intro(app);
    selector[Screen::GAME_OVER] = new GameOver(app);
    selector[Screen::GAME] = new Game(app);
    
    // première scène = intro
    Screen::Choice choice = Screen::INTRO;
    do
    {
        choice = selector[choice]->Run();
    } while (choice != Screen::EXIT_APP);
    
    for (int i = 0; i < Screen::NB_SCREENS; ++i)
    {
        delete selector[i];
    }
    app.Close();
    
    return 0;
}

