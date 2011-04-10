#ifndef GUI_BITMAPFONT_HPP
#define GUI_BITMAPFONT_HPP

#include <SFML/Graphics.hpp>

namespace gui
{

class BitmapFont
{
public:
	enum
	{
		FIRST_CHAR = 32, LAST_CHAR = 126
	};

	/**
	 * @param image: chemin image de la police bitmap
	 * @param width: nombre de caractères en largeur
	 * @param height: nombre de caractère en hauteur
	 */
	BitmapFont(const char* imagepath, int width, int height);

	/**
	 * Changer l'image de la police
	 */
	bool SetImage(const char* imagepath);

	/**
	 * Récupérer l'image en cours d'utilisation
	 */
	const sf::Image& GetImage() const;

	/**
	 * Obtenir le subrect d'un caractère
	 * @param character: caractère demandé
	 * @return subrect dans l'image de la police
	 */
	sf::IntRect GetCharRect(char character) const;

	/**
	 * Largeur d'un caractère
	 */
	int GetCharWidth() const;

	/**
	 * Hauteur d'un caractère
	 */
	int GetCharHeight() const;

private:
	sf::Image image_;
	int width_;
	int height_;
	int char_width_;
	int char_height_;
};

}

#endif // GUI_BITMAPFONT_HPP

