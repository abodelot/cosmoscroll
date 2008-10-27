#ifndef GAME_HPP
#define GAME_HPP

#include "Level.hpp"
#include "Entity.hpp"
#include "BulletManager.hpp"
#include "PlayerShip.hpp"
#include "ParticleSystem.hpp"
#include "ControlPanel.hpp"

#include <SFML/Graphics.hpp>
#include <vector>

/*
 * Gestion du déroulement du jeu
 */
class Game
{
public:
    Game();
    
    ~Game();
    
    /*
     * Lancer une partie de CosmoScroll
     */
    void Run();

    /*
     * Ajout d'une nouvelle unité dans le jeu
     */
    void AddEntity(Entity* entity, int t = 0);
    
private:

    enum Choice
    {
        INTRO, OPTIONS, PLAY, GAME_OVER, EXIT_APP
    };
    
    Choice Intro();
    Choice Options();
    Choice Play();
    Choice GameOver();
    
    /*
     * Types de retour des menus
     */
    enum MenuAction
    {
        MENU_OPTIONS,
        MENU_RESUME,
        MENU_EXIT
    };
    
    /*
     * Mise en pause du jeu - Menu
     */
    MenuAction InGameMenu();

    /*
     * Méthode temporaire d'ajout d'ennemi
     */
    void AddFoo();
    
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
    Level& levels_;
    std::string level_desc_; // Description du niveau courant
    
    // toutes les unités sont allouées dynamiquement
    //std::vector<Entity*> entities_;
    Entity::ManagedContainer entities_;
    bool Update_Entity_List();
    
    
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
};

#endif /* guard GAME_HPP */

