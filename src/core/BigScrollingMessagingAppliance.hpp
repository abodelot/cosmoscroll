#ifndef		_BSMA_HPP_
# define	_BSMA_HPP_

#include <SFML/Graphics.hpp>
#include <deque>
#include <vector>

class BSMA {
public:

	typedef struct {
		sf::String	str;
		float				delay;
	}	text_message;

	enum Speed		{SLOW, MEDIUM, FAST, MAX_Speed};

	enum Direction	{UP, DOWN, RIGHT, LEFT, MAX_Direction, HORIZ=LEFT|RIGHT};

	/**
	 * Récupérer l'instance unique
	 * @return référence sur le grand afficheur déroulant de textes
	 */
	static BSMA&	GetInstance();
	void			Push(BSMA::text_message&);
	void			Override(BSMA::text_message&);
	inline void		Clear() {messages_.clear();};

	void			Update(float frametime);

	void			Show(sf::RenderTarget &) const;

	void			SetOrigin(const sf::Vector2f &);
	const sf::Vector2f &GetOrigin() const;

	void			SetDirection(BSMA::Direction);
	BSMA::Direction GetDirection(void) const;

	void			SetHeight(const float);
	float			GetHeight(void) const;

	void			SetSpeed(BSMA::Speed);
	BSMA::Speed		GetSpeed(void) const;

	void			Init(const sf::Vector2f &origin = sf::Vector2f(640.f, 300.f), BSMA::Direction direction = LEFT, BSMA::Speed speed = FAST);
private:
	BSMA();
	BSMA(const BSMA &);
	~BSMA();

	std::deque<BSMA::text_message> messages_;
	bool shown_;
#define time_base_  42.f
	Speed speed_;
	Direction direction_;
	sf::Vector2f origin_;
	std::vector<sf::Text> chars_;


};
#endif		/*_BSMA_HPP_*/
