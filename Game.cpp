#include "Game.hpp"
#include "MediaManager.hpp"
#include "Asteroid.hpp"
#include "Ennemy.hpp"
#include "Blorb.hpp"
#include "Menu.hpp"
#include "Window.hpp"
#include "Misc.hpp"

#include <SFML/System.hpp>
#include <typeinfo>

#define KEY_PAUSE sf::Key::P
#define KEY_ESC sf::Key::Escape


Game::Game() :
    bullets_  (BulletManager::GetInstance()),
    particles_(ParticleSystem::GetInstance()),
    panel_    (ControlPanel::GetInstance()),
    levels_   (Level::GetInstance())
{
    // TODO: load settings here
    bool fullscreen = false;
    // mise en place de la fenêtre de rendu
    if (!fullscreen)
    {
        app_.Create(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT, WIN_BPP), WIN_TITLE);
    }
    else
    {
        app_.Create(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT, WIN_BPP), WIN_TITLE,
            sf::Style::Fullscreen);
    }
    app_.SetFramerateLimit(WIN_FPS);
    app_.ShowMouseCursor(false);
}


Game::~Game()
{
    RemoveEntities();
    app_.Close();
}


void Game::Run()
{
    // première scène = Intro
    Choice what = Intro();
    do
    {
        switch (what)
        {
            case PLAY:
                what = Play();
                break;
            case GAME_OVER:
                what = GameOver();
                break;
            default:
                break;
        }
    } while (what != EXIT_APP);
}


Game::Choice Game::Intro()
{
    Choice what = PLAY;
    float duration = 5;
    
    sf::Sprite background;
    background.SetImage(GET_IMG("background"));
	sf::String title;
	title.SetText("CosmoScroll");
	title.SetFont(GET_FONT());
	title.SetSize(60);
	title.SetColor(sf::Color::White);
	sf::String sfml;
	sfml.SetText("Powered by SFML");
	sfml.SetFont(GET_FONT());
	sfml.SetSize(24);
	sfml.SetColor(sf::Color::White);
	
	sf::FloatRect rect = title.GetRect();
	title.SetPosition((WIN_WIDTH - rect.GetWidth()) / 2,
	    (WIN_HEIGHT - rect.GetHeight()) / 2);
	
	rect = sfml.GetRect();
	sfml.SetPosition(WIN_WIDTH - rect.GetWidth(), WIN_HEIGHT - rect.GetHeight());
	
	sf::Sprite ship(GET_IMG("spaceship"));
	ship.SetPosition(-20, 100);
	
	sf::Event event;
	while (duration > 0)
	{
		while (app_.GetEvent(event)) 
		{
		    if (event.Type == sf::Event::Closed)
		    {
		        duration = 0;
		        what = EXIT_APP;
		    }
		    else if (event.Type == sf::Event::KeyPressed)
		    {
		        duration = 0;
                if (event.Key.Code == sf::Key::Escape)
				{
				    what = EXIT_APP;
				}
			}
		}
		float time = app_.GetFrameTime();
		duration -= time;
		// FIXME: déplacement magique !
		ship.Move(180 * time, 25 * time);
		title.Move(0, -30 * time);
		
		app_.Draw(background);
		app_.Draw(sfml);
		app_.Draw(title);
		app_.Draw(ship);
		app_.Display();
	}
	return what;
}


Game::Choice Game::Options()
{
    return EXIT_APP;
}


Game::Choice Game::Play()
{
    sf::Event event;
    bool running = true;
    timer_ = 0;
    Choice choice = EXIT_APP;
    
    Respawn();
    particles_.AddStars();
    if (levels_.SetLevel(1, level_desc_) == Level::SUCCESS)
    {
        std::cout << level_desc_ << std::endl ;
    }
    else
    {
        std::cerr << "Erreur au chargement du niveau" << std::endl;
        running = false;
    }
    
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
                    if (what == MENU_EXIT)
                    {
                        running = false;
                    }
                }
            }
        }
        // attention à la gestion des évènements, event doit être traité DANS
        // la boucle du GetEvent, alors que tout ce qui passe par GetInput se
        // fait après la boucle (ici, donc).
        
        Entity::ManagedIterator it;
        // <hack>
        // un ennemi en plus toutes les 10s + 7
        unsigned int max_foo = GetTimer(max_foo) /10 + 7;
        if (entities_.size() < max_foo)
        {
            AddFoo();
        }
        // </hack>
        
        running = Update_Entity_List();
        
        // action
        for (it = entities_.begin(); it != entities_.end(); ++it)
        {
            (*it).second.self->Action();
        }
        
        // moving
        float time = app_.GetFrameTime();
        timer_ += time;
        panel_.SetChrono(GetTimer(max_foo));
        
        for (it = entities_.begin(); it != entities_.end();)
        {
            if ((*it).second.self->IsDead())
            {
            //std::cerr << typeid(((*it).second.self)) << ", " << typeid(PlayerShip) << std::endl;
                if (typeid((*(*it).second.self)) == typeid(PlayerShip))
                {
                    running = false;
                    choice = GAME_OVER;
                    break;
                }
                Entity::ManagedIterator del_ = it;
                ++ it;
                delete (*del_).second.self; 
                entities_.erase(del_);
            }
            else
            {
                (*it).second.self->Move(time); 
                if (player_.ship != (*it).second.self   // FIXME: invalid read of size 8 - Freed by ~PlayerShip
                    && player_.ship->GetRect().Intersects((*it).second.self->GetRect()))
                {
                    player_.ship->Hit(1); // FIXME: dégâts magiques !
                    (*it).second.self->Hit(1);
                }
                ++it;
            }
        }
        
        bullets_.Update(time);
        particles_.Update(time);
        
        bullets_.Collide(entities_);
        
        // rendering
        bullets_.Show(app_);
        particles_.Show(app_);
        for (it = entities_.begin(); it != entities_.end(); ++it)
        {
            (*it).second.self->Show(app_);
        }
        panel_.Show(app_);
        app_.Display();
    }
    
    player_.best_time = timer_;
    // clean up
    RemoveEntities();
    particles_.Clear();
    bullets_.Clear();
    return choice;
}


Game::Choice Game::GameOver()
{
    sf::String title;
    int min = (int) player_.best_time / 60;
    int sec = (int) player_.best_time % 60;
    title.SetText(epic_sprintf("Tu as tenu seulement %d min et %d sec", min, sec));
    title.SetFont(GET_FONT());
    title.SetColor(sf::Color::White);
    title.SetPosition(42, 42);
    
    Menu menu;
    menu.SetOffset(sf::Vector2f(42, 100));
    menu.AddItem("Rejouer", PLAY);    
    menu.AddItem("Quitter", EXIT_APP);
    
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
    return static_cast<Choice>(choice);
}


void Game::AddEntity(Entity* entity, int t)
{

    Entity::TimedEntity tmp_te_;
    tmp_te_.t=t;
    tmp_te_.self=entity;
    
    entities_.insert(std::pair<Entity::Status, Entity::TimedEntity>(Entity::BASE, tmp_te_));
}


void Game::AddFoo()
{
    // <hack>
    sf::Vector2f offset;
    offset.x = WIN_WIDTH;
    offset.y = sf::Randomizer::Random(0, WIN_HEIGHT);
    int ploufplouf = sf::Randomizer::Random(0, 10);
    if (ploufplouf > 6)
    {
    // cast en Entity** car on ne peut pas subsituer un super pointeur enfant
    // à un parent. (un pointeur oui, une référence oui, mais pas un super
    // pointeur :( )
        AddEntity(new Ennemy(offset, player_.ship));
    }
    else if (ploufplouf > 2)
    {
        AddEntity(new Asteroid(offset, Asteroid::BIG, *this));
    }
    else
    {
        AddEntity(new Blorb(offset, player_.ship));
    }
    // </hack>
}


void Game::Respawn()
{
    sf::Vector2f offset;
    offset.x = 0;
    offset.y = WIN_HEIGHT / 2.0;
    player_.ship = new PlayerShip(offset, app_.GetInput());
    AddEntity(player_.ship);
}


void Game::RemoveEntities()
{
    Entity::ManagedIterator it;
    Entity::ManagedIterator del_;
    for (it = entities_.begin(); it != entities_.end();)
    {
        del_ = it;
        ++ it;
        delete (*del_).second.self;
    }
    entities_.clear();
    
    
}


Game::MenuAction Game::InGameMenu()
{
    sf::Event event;
    sf::String title("P A U S E D");
    title.SetPosition(255, 160);
    title.SetSize(30.0);

    Menu menu;
    menu.SetOffset(sf::Vector2f(300, 200));
    menu.AddItem("Resume Game", MENU_RESUME);
    menu.AddItem("Options", MENU_OPTIONS);
    menu.AddItem("Exit Game", MENU_EXIT);
    
    bool paused = true;
    int what;
    while (paused)
    {
        while (app_.GetEvent(event))
        {
            if (event.Type == sf::Event::Closed)
            {
                what = MENU_EXIT;
                paused = false;
            }
            if (event.Type == sf::Event::KeyPressed)
            {
                if (event.Key.Code == KEY_ESC)
                {
                    what = MENU_EXIT;
                    paused = false;
                }
                else if (menu.ActionChosen(event.Key, what))
                {
                    paused = false;
                }
            }
        }
        
        // seules les particules sont mises à jour
        particles_.Update(app_.GetFrameTime());
        
        // rendering
        bullets_.Show(app_);
        particles_.Show(app_);
        Entity::ManagedConstIterator it;
        for (it = entities_.begin(); it != entities_.end(); ++it)
        {
            (*it).second.self->Show(app_);
        }
        panel_.Show(app_);
        
        app_.Draw(title);
        menu.Show(app_);
        app_.Display();
    }
    return static_cast<MenuAction>(what);
}

bool Game::Update_Entity_List()
{
    Level::Error err = levels_.GetNextGroup(entities_);
    
    
    return true; //err == Level::END;
}


