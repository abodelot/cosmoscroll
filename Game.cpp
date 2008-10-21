#include "Game.hpp"
#include "MediaManager.hpp"
#include "Window.hpp"
#include "Asteroid.hpp"
#include "Blorb.hpp"
#include "Blorb2.hpp"

#include <SFML/System.hpp>


#define KEY_PAUSE sf::Key::P
#define KEY_DOWN sf::Key::Up
#define KEY_UP sf::Key::Down
#define KEY_VALID sf::Key::Return
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
#define NB_ITEMS 3
    sf::Event event;
    sf::Image capture = app_.Capture(); // Problem: Image is pitch black.
    sf::Sprite back(capture);
    sf::String text("P A U S E D"); text.SetPosition(255,160); text.SetSize(30.0);

    sf::String* items[3];
    items[0] = new sf::String("Resume Game"); items[0]->SetPosition(300,200);
    items[1] = new sf::String("Exit Game"); items[1]->SetPosition(300,228);
    items[2] = new sf::String("Options"); items[2]->SetPosition(300,256);
    short i, selected_item = 0;
    while (1)
    {
	app_.Draw(back); app_.Draw(text);
        for (i = 0; i < NB_ITEMS; ++i)
        {
            items[i]->SetStyle( (i == selected_item)?  sf::String::Bold : sf::String::Regular);
            app_.Draw(*items[i]);
        }
        app_.Display();
        while (app_.GetEvent(event))
        {
            if (event.Type == sf::Event::KeyPressed)
            {
                if (event.Key.Code == KEY_ESC)
                {
                    return EXIT;
                }
                if (event.Key.Code == KEY_PAUSE)
                {
                    return RESUME;
                }
                if (event.Key.Code == KEY_UP)
                {
                    selected_item = (selected_item + 1) % 3;
                }
                if (event.Key.Code == KEY_DOWN)
                {
                    selected_item = ((selected_item)? selected_item - 1 : selected_item = NB_ITEMS - 1);
                }
                if (event.Key.Code == KEY_VALID)
                {
                    if (selected_item == 0) return RESUME;
		    if (selected_item == 1) return EXIT;
                    if (selected_item == 2) Options();
                }
            }
        }
    }
}

void Game::Options()
{
}
