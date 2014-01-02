#ifndef MULTIPARTENTITY_HPP
#define MULTIPARTENTITY_HPP

#include "Damageable.hpp"

/**
 * Base class for entities made of assembled sprites (sub parts)
 */
class MultiPartEntity: public Entity
{
public:
	class Part: public Damageable
	{
	public:
		friend class MultiPartEntity;

		Part(int id = -1, int hp = 1);

		void onUpdate(float frametime) override;

		void takeDamage(int damage) override;

		int getID() const;

		void setDestructible(bool destructible);

		void setParent(MultiPartEntity* parent);

		void onDestroy() override;

	private:
		int  m_id;
		bool m_destructible;
		MultiPartEntity* m_parent;
	};

	MultiPartEntity();

	void collides(Entity& entity);

	// callbacks ---------------------------------------------------------------

	virtual void onPartDestroyed(const Part&) {};
	virtual void onPartDamaged(const Part&) {};

	float getSpeedX() const override; // hack for decors

protected:
	void updateParts(float frametime);

	void addPart(Part& part, float x=0.f, float y=0.f);

	Part& getPartAt(size_t index);

	/**
	 * Get the first part matching a given ID
	 */
	Part* getPartByID(int id);

	/**
	 * Destroy parts matching a given ID
	 * @param id: part id
	 * @return nb parts destroyed
	 */
	int destroyPartByID(int id);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	typedef std::vector<Part> PartVector;
	PartVector m_parts;
};


#endif // MULTIPARTENTITY_HPP
