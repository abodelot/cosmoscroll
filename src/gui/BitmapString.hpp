#ifndef GUI_BITMAPSTRING_HPP
#define GUI_BITMAPSTRING_HPP

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

#include "BitmapFont.hpp"

namespace gui
{

class BitmapString: public sf::Drawable, public sf::Transformable
{
public:
	BitmapString();

	/**
	 * @param font: police bitmap
	 */
	BitmapString(const BitmapFont& font);

	/**
	 * Définir le texte de la chaîne
	 */
	void setString(const char* text);
	void setString(const std::string& text);

	/**
	 * Récupérer le texte de la chaîne
	 */
	const char* GetText() const;

	/**
	 * Ajouter un caractère en fin de chaîne
	 * @param character: caractère à ajouter
	 */
	void AppendChar(char character);

	/**
	 * Insérer un caractère
	 * @param character: caractère à insérer
	 * @param position: indice du caractère
	 * >= 0 : gauche vers droite, < 0 : droite vers gauche (python like)
	 */
	void InsertChar(char character, int position);

	/**
	 * Supprimer un caractère de la chaîne
	 * @param position: indice du caractère
	 * >= 0 : gauche vers droite, < 0 : droite vers gauche (python like)
	 */
	void RemoveChar(int position);

	/**
	 * Obtenir un caractère de la chaîne
	 * @param position: indice du caractère
	 * >= 0 : gauche vers droite, < 0 : droite vers gauche (python like)
	 * @return caractère de la chaîne
	 */
	char GetCharAt(int position) const;

	/**
	 * Changer la police bitmap
	 */
	void setFont(const BitmapFont& font);

	/**
	 * Récupérer la police bitmap en cours d'utilisation
	 */
	const BitmapFont& getFont() const;

	/**
	 * Vider la chaîne
	 */
	void Clear();

	/**
	 * Obtenir le nombre de caractères
	 */
	int Length() const;

	/**
	 * Définir la couleur des bitmaps
	 */
	void setColor(const sf::Color& color);

private:
	// inherited
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	/**
	 * Transforme les positions négatives en positions positives exploitables
	 */
	int GetRealPosition(int position) const;

	/**
	 * Recalculer la position des bitmaps
	 * @param from: première position à recalculer
	 */
	void ComputePosition(int from);

	typedef std::vector<sf::Sprite> SpriteVec;
	SpriteVec bitmaps_;

	int char_width_;
	int last_x_;
	int last_y_;
	std::string chars_;
	const BitmapFont* font_;
};

}

#endif // GUI_BITMAPSTRING_HPP

