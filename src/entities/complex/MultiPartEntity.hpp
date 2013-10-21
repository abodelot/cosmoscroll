#ifndef MULTIPARTENTITY_HPP
#define MULTIPARTENTITY_HPP

#include "Part.hpp"

/**
 * A complex entity is made of several entities (see Part class)
 */
class MultiPartEntity: public Entity
{
public:
	MultiPartEntity();

	void collides(Entity& entity);

	// callbacks ---------------------------------------------------------------

	void onUpdate(float frametime);

	virtual void onPartDestroyed(const Part&) {};

	// override
	float getSpeedX() const;


protected:
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

	// override
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	typedef std::vector<Part> PartVector;
	PartVector m_parts;
};


#endif // MULTIPARTENTITY_HPP
