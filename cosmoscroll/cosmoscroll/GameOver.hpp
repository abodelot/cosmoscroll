#ifndef H_GAME_OVER
#define H_GAME_OVER

#include "Screen.hpp"

/*
 * Scène affichée en cas de défaite
 */
class GameOver: public Screen
{
public:
    GameOver(sf::RenderWindow& app);
    
    Screen::Choice Run();
private:
};

#endif /* guard H_GAME_OVER */

