#ifndef GAME_HPP
#define GAME_HPP

#include "Entity.hpp"
#include "BulletManager.hpp"
#include "PlayerShip.hpp"
#include "ParticleSystem.hpp"
#include "ControlPanel.hpp"
#include "Level.hpp"

#include <SFML/Graphics.hpp>
#include <vector>

/*
 * Gestion du déroulement du jeu
 */
class Game
{
public:
    static Game& GetInstance();

    /*
     * Lancer une partie de CosmoScroll
     */
    void Run();
    
    /*
     * Ajout d'une nouvelle unité dans le jeu
     */
    void AddEntity(Entity* entity);
    
    PlayerShip* GetPlayer() const;
    
private:
    Game();
    ~Game();
    
    enum Choice
    {
        INTRO, OPTIONS, IN_GAME_MENU,
        MAIN_MENU, STORY_MODE, ARCADE_MODE, GAME_OVER, EXIT_APP, INTERTITRE, CONTINUE
    };
    
    // scène d'intro
    Choice Intro();
    // menu principal
    Choice MainMenu();
    // les options de configurations
    Choice Options();
    // jouer (arcade ou story)
    Choice Play();
    // menu en cours de jeu (pause)
    Choice InGameMenu();
    // partie terminée
    Choice GameOver();
    // fin d'un niveau þNon-fin du jeu]
    Choice Intertitre();
    // lancement niveau suivant
    Choice Continue();


    bool MoreBadGuys();
    
    /*
     * Création du vaisseau du joueur
     */
    void Respawn();
    
    /*
     * Suppression de toutes les unités en jeu
     */
    void RemoveEntities();
    
    // ne pas mélanger la gestion du joueur avec PlayerShip
    struct Player
    {
        float best_time;
        PlayerShip* ship;
    };
    
    sf::RenderWindow app_;
    Player player_;
    float timer_;
    BulletManager& bullets_;
    ParticleSystem& particles_;
    ControlPanel& panel_;
    Level& level_;
    std::string level_desc_; // Description du niveau courant
    
    bool arcade_;
    unsigned short cur_lvl_;
    
    // toutes les unités sont allouées dynamiquement
    std::vector<Entity*> entities_;
    //Entity::ManagedContainer entities_;
    //bool Update_Entity_List();
    
    /*
    inline unsigned int GetTimer(unsigned int& i)
    {  
        i = static_cast<unsigned int>(timer_);
        return i;
    }
    
    inline float GetTimer(float& f)
    {
        f = timer_;
        return f;
    }
    */
};

#endif /* guard GAME_HPP */

