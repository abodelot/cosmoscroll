#ifndef H_INTRO
#define H_INTRO

#include "Screen.hpp"

/*
 * Scène animée d'introduction au lancement du jeu
 */
class Intro: public Screen
{
public:
	Intro(sf::RenderWindow& app);
	
	Screen::Choice Run();

private:
	sf::Sprite background_;
};

#endif /* guard H_INTRO */
