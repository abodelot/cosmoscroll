#ifndef GUI_OPTIONLIST_HPP
#define GUI_OPTIONLIST_HPP

#include <vector>
#include <string>

#include "Widget.hpp"
#include "utils/SFML_Helper.hpp"

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
	void AddOption(const sf::String& option);
	void AddOption(const sf::String& display, const std::string& value);

	int GetNbItems() const;

	std::string GetOptionAt(int index) const;

	/**
	 * Get option index currently selected (first is 0)
	 */
	int GetSelectedOptionIndex() const;

	/**
	 * Get selected option value
	 * shortcut for GetOptionAt(GetSelectedOptionIndex())
	 */
	std::string GetSelectedOption() const;

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
	void OnKeyPressed(sf::Keyboard::Key key);
	void OnMouseClicked(int x, int y);
	void OnMouseWheelMoved(int delta);

protected:
	// inherited
	void OnStateChanged(State::EState state);

private:
	// inherited
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void BuildBoxes();

	/**
	 * Calculer l'indentation d'une option selon l'alignement courant
	 */
	int ComputeIndentAlign(const xsf::Text& option) const;

	int PreviousIndex() const;
	int NextIndex() const;

	sf::RectangleShape box_;
	sf::RectangleShape inside_box_;
	sf::ConvexShape left_arrow_;
	sf::ConvexShape right_arrow_;
	typedef std::pair<xsf::Text, std::string> Item;
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
