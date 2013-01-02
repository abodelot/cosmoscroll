#ifndef COMPLEXENTITY_HPP
#define COMPLEXENTITY_HPP

#include "entities/Entity.hpp"
#include "Part.hpp"

/**
 * A complex entity is made of several entities (see Part class)
 */
class ComplexEntity: public Entity
{
public:
	ComplexEntity(const sf::Vector2f& pos);

	// override
	void onUpdate(float frametime);

	// override
	void onCollision(const Entity& entity);

	// override
	float GetSpeedX() const;

	const ComplexEntity* toComplexEntity() const { return this; }

	// callbacks
	virtual void onPartDestroyed(const Part&) {};
protected:
	void AddPart(Part& part, float x=0.f, float y=0.f);


	Part* GetPartAt(size_t index);
	Part* GetPartByID(int id);

	/**
	 * Destroy part(s) added in the complex entity
	 * @param id: part id
	 * @return nb parts destroyed
	 */
	int DestroyPart(int id);



	// override
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	typedef std::vector<Part> PartVector;
	PartVector parts_;
};


#endif // COMPLEXENTITY_HPP
