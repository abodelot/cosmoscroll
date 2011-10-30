#ifndef GUI_OPTIONLIST_HPP
#define GUI_OPTIONLIST_HPP

#include <vector>
#include <string>

#include "Widget.hpp"

namespace gui
{

/**
 * Une liste défilante d'options
 * callback si : choix modifié
 */
class OptionList: public Widget
{
public:
	OptionList(Menu* owner);

	/**
	 * Ajouter une option à la liste
	 */
	void AddOption(const sf::Unicode::Text& option);
	void AddOption(const sf::Unicode::Text& display, const std::string& value);

	/**
	 * Obtenir l'option sélectionnée
	 */
	std::string GetSelectedOption() const;
	int GetSelectedOptionIndex() const;

	/**
	 * Set the current displayed item
	 */
	void Select(int index);
	void SelectByValue(const std::string& value);

	// supprimer toutes les options
	void Clear();

	/**
	 * Indiquer l'alignement du texte des options
	 */
	void SetAlign(Align::EAlign align);

	// inherited
	void Update(float frametime);

	// inherited callbacks
	void OnKeyPressed(sf::Key::Code key);
	void OnMouseClicked(int x, int y);
	void OnMouseWheelMoved(int delta);

protected:
	// inherited
	void OnStateChanged(State::EState state);

private:
	// inherited
	void Render(sf::RenderTarget& target) const;

	void BuildBoxes();

	/**
	 * Calculer l'indentation d'une option selon l'alignement courant
	 */
	int ComputeIndentAlign(const sf::String& option) const;

	int PreviousIndex() const;
	int NextIndex() const;

	sf::Shape box_;
	sf::Shape inside_box_;
	sf::Shape left_arrow_;
	sf::Shape right_arrow_;
	typedef std::pair<sf::String, std::string> Item;
	std::vector<Item> options_;
	int current_opt_;
	size_t max_opt_width_;
	int dir_;
	float scale_;
	Align::EAlign align_;

	int text_size_;
};

}

#endif // GUI_OPTIONLIST_HPP
