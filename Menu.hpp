#ifndef MENU_HPP
#define MENU_HPP

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

/*
 * Permet de sélectionner une action
 */
class Menu
{
public:
    Menu();
    
    /*
     * Apparence normale des éléments
     * @param[in] style: flag sf::String::Style
     * @param[in] color: couleur
     */
    void SetNormalLook(unsigned long style, const sf::Color& color);
    
    /*
     * Apparence de l'élément pré-sélectionné
     * @param[in] style: flag sf::String::Style
     * @param[in] color: couleur
     */
    void SetHighlightLook(unsigned long style, const sf::Color& color);
    
    /*
     * Taille du texte des éléments
     * @param[in] size: text size
     */
    void SetTextSize(int size);
    
    /*
     * Fixer l'orignie haut-gauche du menu
     * @param[in] offset: position
     */
    void SetOffset(const sf::Vector2f& offset);
    
    /*
     * Fixer l'intergline entre les éléments
     * @param[in] linespace: interligne en pixels
     */
    void SetLineSpace(int linespace);
    
    /*
     * Ajout d'un élément dans le menu
     * @param[in] label: texte de l'élément
     * @param[in] id: identifiant de l'action associée
     */
    void AddItem(const std::string label, int id);
    
    /*
     * Détermine si un évènement a validé une action
     * @param[in] key: évènement à gérer
     * @param[out] id: identifiant de l'action validée
     * @return true si une action a été validée, sinon fasle
     */
    bool ActionChosen(const sf::Event::KeyEvent& key, int& id);
    
    /*
     * Afficher les éléments du menu
     * @param[in] app: fenêtre de rendu
     */
    void Show(const sf::RenderWindow& app) const;
    
private:
    /*
     * Un élément du menu
     */
    struct MenuItem
    {
        sf::String label;
        int id;
    };
    
    /*
     * Apparence d'un élément
     */
    struct ItemLook
    {
        sf::Color color;
        unsigned long style;
    };
    
    /*
     * Appliquer une apparence sur un élément
     * @param[out] item: élément à modifier
     * @param[in] look: apparence à appliquer
     */
    static void ApplyStyle(MenuItem& item, const ItemLook& look);
    
    std::vector<MenuItem> items_;
    sf::Vector2f offset_;
    int selected_; // indice de l'élément sélectionné
    int textsize_;
    int linespace_; // interligne en pixels
    ItemLook highlight_look_;
    ItemLook normal_look_;
};

#endif /* guard MENU_HPP */

