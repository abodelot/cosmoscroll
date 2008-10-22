#include "Game.hpp"
#include "MediaManager.hpp"
#include "Window.hpp"
#include "Asteroid.hpp"
#include "Blorb.hpp"
#include "Blorb2.hpp"
#include "Menu.hpp"

#include <SFML/System.hpp>

#define KEY_PAUSE sf::Key::P
#define KEY_ESC sf::Key::Escape


Game::Game(sf::RenderWindow& app) :
    Screen       (app),
    bullets_     (BulletManager::GetInstance()),
    particle_sys_(ParticleSystem::GetInstance()),
    panel_       (ControlPanel::GetInstance())
{ 
}


Game::~Game()
{
    RemoveEntities();
}


Screen::Choice Game::Run()
{
    sf::Event event;
    bool running = true;
    Screen::Choice choice = Screen::EXIT_APP;
    
    Respawn();
    particle_sys_.AddStars();
    
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
                if (event.Key.Code == KEY_ESC)
                {
                    running = false;
                }
                if (event.Key.Code == KEY_PAUSE)
                {
                    MenuAction what = InGameMenu();
		    if (what == EXIT)
		    {
			running = false;
		    }
                }
            }
        }
        // attention à la gestion des évènements, event doit être traité DANS
        // la boucle du GetEvent, alors que tout ce qui passe par GetInput se
        // fait après la boucle (ici, donc).
        
        std::vector<Entity*>::iterator it;
        // <hack>
        if (entities_.size() < 12)
        {
            AddFoo();
        }
        // </hack>
        
        // action
        for (it = entities_.begin(); it != entities_.end(); ++it)
        {
            (**it).Action();
        }
        
        // moving
        float time = app_.GetFrameTime();
        
        for (it = entities_.begin(); it != entities_.end();)
        {
            if ((**it).IsDead())
            {
                if (typeid(**it) == typeid(PlayerShip))
                {
                    running = false;
                    choice = Screen::GAME_OVER;
                    break;
                }
                delete *it;
                it = entities_.erase(it);
            }
            else
            {
                (**it).Move(time);
                if (player_ != *it
                    && player_->GetRect().Intersects((**it).GetRect()))
                {
                    player_->Hit(1); // FIXME: magique
                    (**it).Hit(1);
                }
                ++it;
            }
        }
        
        bullets_.Update(time);
        particle_sys_.Update(time);
        
        bullets_.Collide(entities_);
        
        // rendering
        bullets_.Show(app_);
        particle_sys_.Show(app_);
        for (it = entities_.begin(); it != entities_.end(); ++it)
        {
            (**it).Show(app_);
        }
        panel_.Show(app_);
        app_.Display();
    }
    
    // clean up
    RemoveEntities();
    particle_sys_.Clear();
    bullets_.Clear();
    return choice;
}


void Game::AddEntity(Entity* entity)
{
    entities_.push_back(entity);
}


void Game::AddFoo()
{
    // <hack>
    sf::Vector2f offset;
    offset.x = WIN_WIDTH;
    offset.y = sf::Randomizer::Random(0, WIN_HEIGHT);
    int ploufplouf = sf::Randomizer::Random(0, 2);
    if (ploufplouf == 0)
    {
    // cast en Entity** car on ne peut pas subsituer un super pointeur enfant
    // à un parent. (un pointeur oui, une référence oui, mais pas un super
    // pointeur :( )
        entities_.push_back(new Blorb(offset, (Entity**) &player_));
    }
    else if (ploufplouf == 1)
    {
        entities_.push_back(new Blorb2(offset, (Entity**) &player_));
    }
    else if (ploufplouf == 2)
    {
        entities_.push_back(new Asteroid(offset, Asteroid::BIG, *this));
    }
    // </hack>
}


void Game::Respawn()
{
    sf::Vector2f offset;
    offset.x = 0;
    offset.y = WIN_HEIGHT / 2.0;
    player_ = new PlayerShip(offset, app_.GetInput());
    entities_.push_back(player_);
}


void Game::RemoveEntities()
{
    const size_t length = entities_.size();
    for (size_t i = 0; i < length; ++i)
    {
        delete entities_[i];
    }
    entities_.clear();
}

// Autre souci: comment intercepter proprement les événements de fermeture de l'appli?
// Retourne: Valeur enumérée Action:
Game::MenuAction Game::InGameMenu()
{
    sf::Event event;
    sf::String title("P A U S E D");
    title.SetPosition(255, 160);
    title.SetSize(30.0);

    Menu menu;
    menu.SetOffset(sf::Vector2f(300, 200));
    menu.AddItem("Resume Game", RESUME);
    menu.AddItem("Options", OPTIONS);
    menu.AddItem("Exit Game", EXIT);
    
    bool paused = true;
    int what;
    while (paused)
    {
        while (app_.GetEvent(event))
        {
            if (event.Type == sf::Event::Closed)
            {
                what = EXIT;
                paused = false;
            }
            if (event.Type == sf::Event::KeyPressed)
            {
                if (event.Key.Code == KEY_ESC)
                {
                    what = EXIT;
                    paused = false;
                }
                else if (menu.ActionChosen(event.Key, what))
                {
                    paused = false;
                }
            }
        }
        
        // seules les particules sont mises à jour
        particle_sys_.Update(app_.GetFrameTime());
        
        // rendering
        bullets_.Show(app_);
        particle_sys_.Show(app_);
        std::vector<Entity*>::const_iterator it;
        for (it = entities_.begin(); it != entities_.end(); ++it)
        {
            (**it).Show(app_);
        }
        panel_.Show(app_);
        
        app_.Draw(title);
        menu.Show(app_);
        app_.Display();
    }
    return static_cast<MenuAction>(what);
}

void Game::Options()
{
}
