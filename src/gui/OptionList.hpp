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
template <class T>
class OptionList: public Widget
{
public:
	OptionList(Menu* owner);

	/**
	 * Ajouter une option à la liste
	 */
	void Add(const sf::String& display, const T& value);

	size_t GetSize() const;

	const T& GetValueAt(int index) const;

	/**
	 * Get option index currently selected (first is 0)
	 */
	int GetSelectedIndex() const;

	/**
	 * Get selected option value
	 * shortcut for GetOptionAt(GetSelectedOptionIndex())
	 */
	const T& GetSelectedValue() const;

	/**
	 * Set the current displayed item
	 */
	void Select(int index);
	void SelectByValue(const T& value);

	// supprimer toutes les options
	void Clear();

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

	int PreviousIndex() const;
	int NextIndex() const;

	typedef std::pair<sf::Text, T> Item;
	typedef std::vector<Item> ItemVector;

	ItemVector options_;
	int current_opt_;
	size_t max_opt_width_;
	int text_size_;
	sf::RectangleShape box_;
	sf::RectangleShape inside_box_;
	sf::ConvexShape left_arrow_;
	sf::ConvexShape right_arrow_;
};

}

#include "OptionList.inl"

#endif // GUI_OPTIONLIST_HPP
