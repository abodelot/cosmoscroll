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
	void OnCollide(Entity& entity);

protected:
	void AddPart(Part& part, float x=0.f, float y=0.f);

	// TODO: refactor API
	Part& GetPart(size_t index);
	Part* GetPartByID(int id);

	/**
	 * Destroy part(s) added in the complex entity
	 * @param id: part id
	 * @return nb parts destroyed
	 */
	int DestroyPart(int id);

	// callbacks
	virtual void OnPartDestroyed(const Part& p) {};

	// override
	void Render(sf::RenderTarget& target) const;

private:
	sf::Shape test_;
	typedef std::vector<Part> PartVector;
	PartVector parts_;
};


#endif // COMPLEXENTITY_HPP
