#ifndef GAME_HPP
#define GAME_HPP

#include "Screen.hpp"
#include "Entity.hpp"
#include "BulletManager.hpp"
#include "PlayerShip.hpp"
#include "ParticleSystem.hpp"
#include "ControlPanel.hpp"

#include <SFML/Graphics.hpp>
#include <vector>

/*
 * Écran du jeu
 */
class Game: public Screen
{
public:
    Game(sf::RenderWindow& app);
    
    ~Game();
    
    /*
     * Lancer une partie de CosmoScroll
     */
    Screen::Choice Run();

    /*
     * Ajout d'une nouvelle unité dans le jeu
     */
    void AddEntity(Entity* entity);
    
private:

    /*
     * Types de retour des menus
     */
    enum MenuAction
    {
        OPTIONS,
        RESUME,
        EXIT
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
    
    void Options();

    BulletManager& bullets_;
    ParticleSystem& particle_sys_;
    
    // toutes les unités sont allouées dynamiquement
    std::vector<Entity*> entities_;
    PlayerShip* player_;
    ControlPanel& panel_;
};

#endif /* guard GAME_HPP */

