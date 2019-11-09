#ifndef GUI_MENU_HPP
#define GUI_MENU_HPP

#include <vector>
#include <SFML/Graphics.hpp>


#include "WidgetStyle.hpp"


namespace gui
{

class Widget;

/**
 * Gestionnaire et conteneur d'éléments graphiques
 * Classe à dériver pour implémenter ses propres menus
 */
class Menu
{
public:
    Menu(const sf::RenderWindow& window, WidgetStyle& style);

    virtual ~Menu();

    void OnEvent(const sf::Event& event);

    void Update(float frametime);

    void Show(sf::RenderTarget& target) const;

    void AddWidget(Widget* widget);

    const Widget* GetFocusedWidget() const;

    /**
     * Récupérer le thème des widgets
     */
    WidgetStyle& GetWidgetStyle();

protected:
    bool FocusWidget(const Widget* widget);

    // callbacks
    virtual void OnWidgetHovered() {};
    virtual void OnWidgetFocused() {};

    /**
     * Sélectionner un widget dans le menu
     * @param index: position du widget
     * @return true si le widget a pris le focus, sinon false
     */
    bool FocusWidget(int index);

    /**
     * Sélectionner le premier widget possible
     * @return true si un widget a pris le focus, sinon false
     */
    bool FocusFirstWidget();

    /**
     * Sélectionner le prochain widget
     * @return true si un widget a pris le focus, sinon false
     */
    bool FocusNextWidget();

    /**
     * Sélectionner le widget précédent
     * @return true si un widget a pris le focus, sinon false
     */
    bool FocusPreviousWidget();

    /**
     * Obtenir un widget positionné sous un point
     * @param x: position x
     * @param y: position y
     * @return widget sous le point, ou NULL
     */
    Widget* GetHoveredWidget(int x, int y) const;

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
