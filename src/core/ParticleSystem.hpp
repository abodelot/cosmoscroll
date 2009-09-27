#ifndef PARTICLESYSTEM_HPP
#define PARTICLESYSTEM_HPP

#include <list>
#include <SFML/Graphics.hpp>

#include "../utils/MediaManager.hpp"

/**
 * Moteur de particules pour gérer des effets graphiques (singleton)
 */
class ParticleSystem: public sf::Drawable
{
public:
	/**
	 * Récupérer l'instance unique
	 */
	static ParticleSystem& GetInstance();

	/**
	 * Ajouter une explosion
	 * @param[in] offset: position de l'explosion
	 */
	void AddExplosion(const sf::Vector2f& offset);

	/**
	 * Ajouter un effet d'impact
	 * @param[in] offset: position de l'impact
	 * @param[in] count: nombre d'étincelles
	 */
	void AddImpact(const sf::Vector2f& offset, int count);

	/**
	 * Ajouter des étoiles défilantes dans la scène
	 * @param[in] count: nombre d'étoiles
	 */
	void AddStars(int count = 33);

	/**
	 * Ajouter un message défilant
	 * @param[in] offset: position
	 * @param[in] text: contenu du message
	 */
	void AddMessage(const sf::Vector2f& offset, const wchar_t* text);

	void AddShield(int count, const sf::Sprite* handle);

	void AddFollow(int count, const sf::Sprite* handle);

	void RemoveShield(const sf::Sprite* handle);

	/**
	 * Mise à jour des particules (déplacement)
	 */
	void Update(float frametime);

	/**
	 * Suppression de toutes les particules
	 */
	void Clear();

private:
	ParticleSystem();
	ParticleSystem(const ParticleSystem& other);
	ParticleSystem& operator=(const ParticleSystem& other);
	~ParticleSystem();

	// inherited
	void Render(sf::RenderTarget& target) const;

	/**
	 * Particule abstraite
	 */
	class Particle
	{
	public:
		/**
		 * Animation de la particule lors de l'update du ParticleSystem
		 * @return true si la particule est morte, sinon false
		 */
		virtual bool OnUpdate(float frametime) = 0;
		virtual ~Particle() {};

		/**
		 * Affichage de la particule
		 */
		virtual void Show(sf::RenderTarget& target) const = 0;
	};


	/**
	 * Impact d'une explosion
	 */
	class Fiery: public Particle
	{
	public:
		Fiery(const sf::Vector2f& offset, const sf::Image& img);
		~Fiery() {};
		bool OnUpdate(float frametime);

		inline void Show(sf::RenderTarget& target) const
		{
			target.Draw(sprite_);
		}
	private:
		sf::Sprite sprite_;
		float angle_; // en radians
		float timer_;
		float lifetime_;
	};


	/**
	 * Étoile défilante
	 */
	class Star: public Particle
	{
	public:
		Star();
		~Star() {};
		bool OnUpdate(float frametime);

		inline void Show(sf::RenderTarget& target) const
		{
			target.Draw(sprite_);
		}
	private:
		int speed_;
		sf::Sprite sprite_;
	};


	/**
	 * Court message défilant
	 */
	class TextParticle: public Particle
	{
	public:
		TextParticle(const sf::Vector2f& offset, const wchar_t* text);
		~TextParticle() {};
		bool OnUpdate(float frametime);

		inline void Show(sf::RenderTarget& target) const
		{
			target.Draw(text_);
		}
	private:
		sf::String text_;
		float timer_;
	};


	/**
	 * Particule liée à un objet externe au ParticleSystem (non autonome)
	 */
	class LinkedParticle: public Particle
	{
	public:
		LinkedParticle(const sf::Sprite* handle, float angle);
		~LinkedParticle() {};
		bool OnUpdate(float frametime);

		inline void Show(sf::RenderTarget& target) const
		{
			target.Draw(sprite_);
		}

		inline const sf::Sprite* GetHandle() const
		{
			return handle_;
		}
	private:
		const sf::Sprite* handle_; // hack... le sprite cible sert de handle
		sf::Sprite sprite_;
		float angle_; // en radians
	};

	class Follow: public Particle
	{
	public:
		Follow(const sf::Sprite* handle);
		~Follow() { }
		bool OnUpdate(float frametime);
		inline void Show(sf::RenderTarget& target) const
		{
			target.Draw(sprite_);
		}
	private:
		float angle_;
		float speed_;
		sf::Sprite sprite_;
		const sf::Sprite* handle_;
		float timer_;
	};

	typedef std::list<Particle*> ParticleList;

	ParticleList particles_;
	MediaManager& media_;
};

#endif // PARTICLESYSTEM_HPP

