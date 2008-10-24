#ifndef PARTICLESYSTEM_HPP
#define PARTICLESYSTEM_HPP

#include <list>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

/*
 * Moteur de particules pour gérer des effets graphiques
 * (singleton)
 */
class ParticleSystem
{
public:
    /*
     * Récupérer l'instance unique
     */
    static ParticleSystem& GetInstance();
    
    /*
     * Ajouter une explosion à la position "offset"
     */
    void AddExplosion(const sf::Vector2f& offset);
    
    /*
     * Ajouter un impact de "n" particules à la position "offset"
     */
    void AddImpact(const sf::Vector2f& offset, int n);
    
    /*
     * Ajouter "count" étoiles défilantes dans la scène
     */
    void AddStars(int count = 25);
    
    void AddShield(int count, const sf::Sprite* handle);
    
    void RemoveShield(const sf::Sprite* handle);
        
    /*
     * Mise à jour des particules (déplacement)
     */
    void Update(float frametime);
    
    /*
     * Affichage des particules dans la fenêtre de rendu
     */
    void Show(sf::RenderWindow& app);
    
    /*
     * Suppression de toutes les particules
     */
    void Clear();
    
private:
    ParticleSystem();
    ParticleSystem(const ParticleSystem& other);
    ParticleSystem& operator=(const ParticleSystem& other);
    ~ParticleSystem();
    
    /*
     * Bon ça fait un peu torcheballe là, avec toutes les sous classes.
     * en plus les LinkedParticle sont toutes utilisées en tant que boules
     * de bouclier...
     * -> à nettoyer '^^
     */
    
    /*
     * Particule abstraite
     */
    class Particle: public sf::Sprite
    {
    public: 
        /*
         * Animation de la particule lors de l'update du ParticleSystem
         * return true si la particule est morte, sinon false
         */
        virtual bool OnUpdate(float frametime) = 0;
        
    protected:
        float speed_;
        float angle_; // en radians
    };
    
    
    /*
     * Projection d'une explosion
     */
    class Fiery: public Particle
    {
    public:
        Fiery() {}
        Fiery(const sf::Vector2f& offset);
        virtual bool OnUpdate(float frametime);
    protected:
        float lifetime_;
        // FIXME: speed_ est non utilisé ! supprimer lifetime_ ? 
        // utiliser speed_ comme facteur de déplacement ?
        // virer speed_ de Particle ?
    };
    
    
    class Impact: public Fiery
    {
    public:
        Impact(const sf::Vector2f& offset);
        //bool OnUpdate(float frametime);
    };
    
    /*
     * Une étoile défilante
     */
    class Star: public Particle
    {
    public:
        Star();
        bool OnUpdate(float frametime);
    };
    
    
    /*
     * Particule non autonome associée à une
     * position variable dans le temps
     */
    class LinkedParticle: public Particle
    {
    public:
        /* TODO: faire une classe abstraite
        toutes les LinkedParticle sont des éléments de bouclier !
        */
        LinkedParticle() {} 
        LinkedParticle(const sf::Sprite* handle, float angle, float radius);
        
        virtual bool OnUpdate(float frametime);
        
        bool IsMyHandle(const sf::Sprite* handle);
        
    protected:
        const sf::Sprite* handle_;
    };
    /*
    class Follow: public LinkedParticle
    {
    public:
        Follow(const sf::Sprite* handle);
        bool OnUpdate(float frametime);
    };
    
    */
    typedef std::list<Particle*> ParticleList;
    
    ParticleList particles_;
    sf::Sound boom_sfx_;
};

#endif /* guard PARTICLESYSTEM_HPP */

