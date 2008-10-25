#ifndef GAME_HPP
#define GAME_HPP

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
    void AddEntity(Entity* entity);
    
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
    
    BulletManager& bullets_;
    ParticleSystem& particles_;
    ControlPanel& panel_;
    
    // toutes les unités sont allouées dynamiquement
    std::vector<Entity*> entities_;
};

#endif /* guard GAME_HPP */

