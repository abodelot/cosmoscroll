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
    panel_    (ControlPanel::GetInstance()),
    level_    (Level::GetInstance())
    
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
            case IN_GAME_MENU:
                what = InGameMenu();
                break;
            case GAME_OVER:
                what = GameOver();
                break;
            case CONTINUE:
                what = Continue();
                break;
            case INTERTITRE:
                what = Intertitre();
                break;
            default:
                break;
        }
    } while (what != EXIT_APP);
}

// les méthodes-écrans

Game::Choice Game::Intro()
{
    puts("-> Game::Intro");
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
    puts("-> Game::Options");
    puts("not implemented yet (quit)");
    return EXIT_APP;
}


Game::Choice Game::MainMenu()
{
    puts("-> Game::MainMenu");
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
    // si on lance une nouvelle partie, on nettoie les restes des éventuelles
    // parties précédentes et l'on respawn un joueur neuf
    if (choice == ARCADE_MODE || choice == STORY_MODE)
    {
        // clean up
        RemoveEntities();
        particles_.Clear();
        bullets_.Clear();
        
        // init
        timer_ = 0.0f;
        particles_.AddStars();
        Respawn();
        if (choice == STORY_MODE)
        {
            cur_lvl_ = 1;
            if (level_.Set(cur_lvl_, level_desc_) == Level::SUCCESS)
            {
                find_replace(level_desc_, "\\n", "\n");
                std::cout << level_desc_ << std::endl ;
            }
            else
            {
                std::cerr << "Erreur au chargement du niveau" << std::endl;
                running = false;
            }  
            arcade_ = false;
        }
        else
        {
            arcade_ = true;
        }
    }
    return static_cast<Choice>(choice);
}


Game::Choice Game::Play()
{
    puts("-> Game::Play");
    sf::Event event;
    bool running = true;
    Choice what = EXIT_APP;
    
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
                        what = InGameMenu();
                        running = false;
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
                
                // On demande niveau suivant, si pas de niveau suivant, fin.


                if (cur_lvl_ > level_.GetLastID())
                {
                    what = GAME_OVER;
                }
                else
                {
                    what = INTERTITRE;
                   
                }
            }
        }
        std::vector<Entity*>::iterator it;

        
        // action
        for (it = entities_.begin(); it != entities_.end(); ++it)
        {
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
                    what = GAME_OVER;
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
                    player_.ship->Collide(**it);
                    (**it).Collide(*player_.ship);
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
            (**it).Show(app_);
        }
        panel_.Show(app_);
        app_.Display();
    }
    
    player_.best_time = timer_;
    return what;
}


Game::Choice Game::InGameMenu()
{
    puts("-> Game::InGameMenu");
    sf::Event event;
    sf::String title("P A U S E D");
    title.SetPosition(255, 160);
    title.SetSize(30.0);

    Menu menu;
    menu.SetOffset(sf::Vector2f(300, 200));
    Choice resume = arcade_ ? ARCADE_MODE : STORY_MODE;
    menu.AddItem("Resume Game", resume);
    menu.AddItem("Back to main menu", MAIN_MENU);
    menu.AddItem("Options", OPTIONS);
    menu.AddItem("Exit Game", EXIT_APP);
    
    bool paused = true;
    int what;
    while (paused)
    {
        while (app_.GetEvent(event))
        {
            if (event.Type == sf::Event::Closed)
            {
                what = EXIT_APP;
                paused = false;
            }
            if (event.Type == sf::Event::KeyPressed)
            {
                if (event.Key.Code == KEY_PAUSE)
                {
                    what = resume;
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
        std::vector<Entity*>::iterator it;
        for (it = entities_.begin(); it != entities_.end(); ++it)
        {
            (**it).Show(app_);
        }
        panel_.Show(app_);
        
        app_.Draw(title);
        menu.Show(app_);
        app_.Display();
    }

    return static_cast<Choice>(what);

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

Game::Choice Game::Intertitre()
{
    sf::String title;
    sf::String subtitle;
    int min = (int) player_.best_time / 60;
    int sec = (int) player_.best_time % 60;
    subtitle.SetText(str_sprintf("Terminé en %d min et %d sec",
            min, sec));
    
    title.SetText(str_sprintf("Fin du niveau %d", cur_lvl_));

    title.SetFont(GET_FONT());
    title.SetColor(sf::Color::White);
    title.SetPosition(42, 42);
    subtitle.SetFont(GET_FONT());
    subtitle.SetColor(sf::Color::White);
    subtitle.SetPosition(32, 72);
    
    Menu menu;
    menu.SetOffset(sf::Vector2f(42, 112));
    menu.AddItem("Continuer", CONTINUE);    
    // menu.AddItem("Réessayer", RETRY);
    
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
        app_.Draw(subtitle);
        menu.Show(app_);
        app_.Display();
    }
    return static_cast<Choice>(choice);
}

Game::Choice Game::Continue()
{
    sf::String title;
    sf::String subtitle;
    
    level_.Set(++cur_lvl_, level_desc_);
    // hacky re-init
    timer_ = 0.0f;
    bullets_.Clear();
    
    find_replace(level_desc_, "\\n", "\n");
    title.SetText(str_sprintf("Niveau %d", cur_lvl_));
    subtitle.SetText(level_desc_);
    
    title.SetFont(GET_FONT());
    title.SetColor(sf::Color::White);
    title.SetPosition(42, 42);
    subtitle.SetFont(GET_FONT());
    subtitle.SetColor(sf::Color::White);
    subtitle.SetPosition(32, 72);
    
    Menu menu;
    menu.SetOffset(sf::Vector2f(42, 558));
    menu.AddItem("Jouer", STORY_MODE);    

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
        app_.Draw(subtitle);
        app_.Draw(title);
        menu.Show(app_);
        app_.Display();
    }
    return static_cast<Choice>(choice);
}

// retourne true s'il y a encore des ennemis à envoyer plus tard
// sinon false (niveau fini, bravo :D)
bool Game::MoreBadGuys()
{
    // si mode arcade
    if (arcade_)
    {
        // un ennemi en plus toutes les 10s + 7
        unsigned int max_bad_guys = static_cast<unsigned int>(timer_ / 10 + 7);
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


void Game::AddEntity(Entity* entity)
{
    entities_.push_back(entity);
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


void Game::Respawn()
{
    sf::Vector2f offset;
    offset.x = 0;
    offset.y = WIN_HEIGHT / 2.0;
    player_.ship = new PlayerShip(offset, app_.GetInput());
    AddEntity(player_.ship);

}


PlayerShip* Game::GetPlayer() const
{
    return player_.ship;
}

