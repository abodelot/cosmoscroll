#ifndef GUI_MENU_HPP
#define GUI_MENU_HPP

#include <vector>
#include <SFML/Graphics.hpp>


#include "WidgetStyle.hpp"

namespace gui {

class Widget;

/**
 * Gestionnaire et conteneur d'éléments graphiques
 * Classe à dériver pour implémenter ses propres menus
 */
class Menu {
public:
    Menu(const sf::RenderWindow& window, WidgetStyle& style);

    virtual ~Menu();

    void onEvent(const sf::Event& event);

    void update(float frametime);

    void show(sf::RenderTarget& target) const;

    void addWidget(Widget* widget);

    const Widget* getFocusedWidget() const;

    /**
     * Récupérer le thème des widgets
     */
    WidgetStyle& getWidgetStyle();

protected:
    bool focusWidget(const Widget* widget);

    virtual void onWidgetHovered() {};
    virtual void onWidgetFocused() {};

    /**
     * Sélectionner un widget dans le menu
     * @param index: position du widget
     * @return true si le widget a pris le focus, sinon false
     */
    bool focusWidget(int index);

    /**
     * Sélectionner le premier widget possible
     * @return true si un widget a pris le focus, sinon false
     */
    bool focusFirstWidget();

    /**
     * Sélectionner le prochain widget
     * @return true si un widget a pris le focus, sinon false
     */
    bool focusNextWidget();

    /**
     * Sélectionner le widget précédent
     * @return true si un widget a pris le focus, sinon false
     */
    bool focusPreviousWidget();

    /**
     * Obtenir un widget positionné sous un point
     * @param x: position x
     * @param y: position y
     * @return widget sous le point, ou NULL
     */
    Widget* getHoveredWidget(int x, int y) const;

private:
    typedef std::vector<Widget*> WidgetList;
    WidgetList widgets_;
    int focus_index_;
    Widget* focus_;
    Widget* hovered_widget_;
    const sf::RenderWindow& window_;
    WidgetStyle& theme_;
};

}

#endif // GUI_MENU_HPP
