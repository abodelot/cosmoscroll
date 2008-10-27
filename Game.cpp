#include "Game.hpp"
#include "MediaManager.hpp"

#include "Asteroid.hpp"
#include "Ennemy.hpp"

#include "Menu.hpp"
#include "Window.hpp"
#include "Misc.hpp"
#include "Level.hpp"

#include <SFML/System.hpp>
#include <typeinfo>

#define KEY_PAUSE sf::Key::P

Game& Game::GetInstance()
{
    static Game self;
    return self;
}


Game::Game() :
    bullets_  (BulletManager::GetInstance()),
    particles_(ParticleSystem::GetInstance()),
    panel_    (ControlPanel::GetInstance())
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
            case MAIN_MENU:
                what = MainMenu();
                break;
            case ARCADE_MODE:
            case STORY_MODE:
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
    Choice what = MAIN_MENU;
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


Game::Choice Game::MainMenu()
{
    puts("Main menu launched");
    Menu menu;
    menu.SetOffset(sf::Vector2f(42, 100));
    menu.AddItem("Mode Aventure", STORY_MODE);
    menu.AddItem("Mode Arcade", ARCADE_MODE);
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
        menu.Show(app_);
        app_.Display();
    }
    arcade_ = choice == ARCADE_MODE;
    puts("exiting main menu");
    return static_cast<Choice>(choice);
}

Game::Choice Game::Play()
{
    puts("game launched");
    sf::Event event;
    bool running = true;
    timer_ = 0.0f;
    Choice choice = EXIT_APP;
    
    Respawn();
    particles_.AddStars();
    if (!arcade_)
    {
        Level::GetInstance().Load(LEVEL_FILE);    
    }
    
    /*if (levels_.SetLevel(1, level_desc_) == Level::SUCCESS)
    {
        std::cout << level_desc_ << std::endl ;
    }
    else
    {
        std::cerr << "Erreur au chargement du niveau" << std::endl;
        running = false;
    }
    */
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
                switch (event.Key.Code)
                {
                    case sf::Key::Escape: // QUICK QUIT
                        running = false;
                        break;
                    case KEY_PAUSE:
                        if (InGameMenu() == MENU_EXIT)
                        {
                            running = false;
                        }
                        break;
                    default:
                        break;
                }
            }
        }
        // attention à la gestion des évènements, event doit être traité DANS
        // la boucle du GetEvent, alors que tout ce qui passe par GetInput se
        // fait après la boucle (ici, donc).
        
        if (running)
        {
            running = MoreBadGuys() || entities_.size() > 1;
            if (!running)
            {
                // il n'y a plus d'ennemis en liste d'attente, et plus
                // d'ennemis dans le vecteur = VICTOIRE EPIC WIN
                // TODO: mettre un flag changer l'enum pour indiquer la victoire
                choice = GAME_OVER;
            }
        }
        //Entity::ManagedIterator it;
        std::vector<Entity*>::iterator it;
        
//        running = Update_Entity_List();
        
        // action
        for (it = entities_.begin(); it != entities_.end(); ++it)
        {
            //(*it).second.self->Action();
            (**it).Action();
        }
        
        // moving
        float time = app_.GetFrameTime();
        timer_ += time;
        panel_.SetChrono(timer_);
        
        
        for (it = entities_.begin(); it != entities_.end();)
        {
            if ((**it).IsDead())
            {
                if (typeid(**it) == typeid(PlayerShip))
                {
                    running = false;
                    choice = GAME_OVER;
                    break;
                }
                delete *it;
                it = entities_.erase(it);
            }
            else
            {
                (**it).Move(time);
                if (player_.ship != *it
                    && player_.ship->GetRect().Intersects((**it).GetRect()))
                {
                    player_.ship->Hit(1); // FIXME: dégâts magiques !
                    (**it).Hit(1);
                }
                ++it;
            }
        }

        /*for (it = entities_.begin(); it != entities_.end();)
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
        }*/
        
        bullets_.Update(time);
        particles_.Update(time);
        
        bullets_.Collide(entities_);
        
        // rendering
        bullets_.Show(app_);
        particles_.Show(app_);
        for (it = entities_.begin(); it != entities_.end(); ++it)
        {
            //(*it).second.self->Show(app_);
            (**it).Show(app_);
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


// retourne true s'il y a encore des ennemis à envoyer plus tard
// sinon false (niveau fini, bravo :D)
bool Game::MoreBadGuys()
{
    // si mode arcade
    if (arcade_)
    {
        // un ennemi en plus toutes les 10s + 7
        unsigned int max_bad_guys = timer_ / 10 + 7;
        if (entities_.size() < max_bad_guys)
        {
            // ajout d'une ennemi au hasard
            // <hack>
            sf::Vector2f offset;
            offset.x = WIN_WIDTH;
            offset.y = sf::Randomizer::Random(0, WIN_HEIGHT);
            int random = sf::Randomizer::Random(0, 10);
            if (random > 6)
            {
                AddEntity(Ennemy::Make(Ennemy::INTERCEPTOR, offset, player_.ship));
            }
            else if (random > 4)
            {
                AddEntity(Ennemy::Make(Ennemy::BLORB, offset, player_.ship));
            }
            else if (random > 2)
            {
                AddEntity(Ennemy::Make(Ennemy::DRONE, offset, player_.ship));
            }
            else
            {
                AddEntity(new Asteroid(offset, Asteroid::BIG));
            };
            // </hack>
        }
        // endless badguys! arcade mode will kill you til you die from it.
        return true;
    }
    
    Level& level = Level::GetInstance();
    // sinon mode histoire
    Entity* p = level.GiveNext(timer_);
    while (p != NULL)
    {
        AddEntity(p);
        p = level.GiveNext(timer_);
    }
    // si le niveau n'est pas fini, alors il y a encore des ennemis
    return level.RemainingEntities() > 0;
}


Game::Choice Game::GameOver()
{
    sf::String title;
    if (arcade_)
    {
        int min = (int) player_.best_time / 60;
        int sec = (int) player_.best_time % 60;
        title.SetText(str_sprintf("Tu as tenu seulement %d min et %d sec",
            min, sec));
    }
    else
    {
        title.SetText("Fin de niveau");
    }
    title.SetFont(GET_FONT());
    title.SetColor(sf::Color::White);
    title.SetPosition(42, 42);
    
    Menu menu;
    menu.SetOffset(sf::Vector2f(42, 100));
    menu.AddItem("Menu principal", MAIN_MENU);    
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


void Game::AddEntity(Entity* entity)
{

//    Entity::TimedEntity tmp_te_;
//    tmp_te_.t=t;
//    tmp_te_.self=entity;
//    entities_.insert(std::pair<Entity::Status, Entity::TimedEntity>(Entity::BASE, tmp_te_));
    entities_.push_back(entity);
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
    const size_t length = entities_.size();
    for (size_t i = 0; i < length; ++i)
    {
        delete entities_[i];
    }
    entities_.clear();
    
    /*Entity::ManagedIterator it;
    Entity::ManagedIterator del_;
    for (it = entities_.begin(); it != entities_.end();)
    {
        del_ = it;
        ++ it;
        delete (*del_).second.self;
    }
    entities_.clear();
    */
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
                if (event.Key.Code == KEY_PAUSE)
                {
                    what = MENU_RESUME;
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
        //Entity::ManagedConstIterator it;
        std::vector<Entity*>::iterator it;
        for (it = entities_.begin(); it != entities_.end(); ++it)
        {
            //(*it).second.self->Show(app_);
            (**it).Show(app_);
        }
        panel_.Show(app_);
        
        app_.Draw(title);
        menu.Show(app_);
        app_.Display();
    }
    return static_cast<MenuAction>(what);
}


PlayerShip* Game::GetPlayer() const
{
    return player_.ship;
}

/*
bool Game::Update_Entity_List()
{
    Level::Error err = levels_.GetNextGroup(entities_);
    return true; //err == Level::END;
}
*/

