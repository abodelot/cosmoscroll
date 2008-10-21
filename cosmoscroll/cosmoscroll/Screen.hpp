#ifndef H_SCREEN
#define H_SCREEN

#include <SFML/Graphics.hpp>

/**
 * Classe abstraite pour gérer les différents écrans du jeu
 */
class Screen
{
public:
    /**
     * Liste des choix d'écran possibles
     */
    enum Choice
    {
        GAME, INTRO, GAME_OVER, NB_SCREENS, EXIT_APP
    };
    
    /**
     * @param[in] app: fenêtre de rendu
     */
    Screen(sf::RenderWindow& app): app_(app) {}
    
    /**
     * Exécution de l'écran
     * @return prochaine scène à afficher
     */
    virtual Choice Run() = 0;
    
    virtual ~Screen() {}
    
protected:
    sf::RenderWindow& app_;
};

#endif /* guard H_SCREEN */

