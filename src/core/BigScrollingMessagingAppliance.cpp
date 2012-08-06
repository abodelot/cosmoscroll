
#include "BigScrollingMessagingAppliance.hpp"

	BSMA& BSMA::GetInstance()
{
	static BSMA self;
	return self;
}

void BSMA::Push(BSMA::text_message &m)
{
	messages_.push_back(m);
}

void BSMA::Override(BSMA::text_message &m)
{
	if (shown_)
		messages_.pop_front();
	messages_.push_front(m);
}

//	inline void BSMA::Clear() {messages_.clear();};

// TODO: erase/remove quand hors du champ, timing
void BSMA::Update(float frametime)
{
	sf::Vector2f delta;

	if (direction_ == HORIZ)
		delta.x = (direction_ == LEFT)? -time_base_ : time_base_;
	else
		delta.y = (direction_ == UP)? time_base_ : -time_base_;

	delta *= frametime;

	std::vector<sf::Text>::iterator it = chars_.begin();
	std::vector<sf::Text>::iterator end = chars_.end();
	while (it != end)
	{
		(*it).move(delta);
		++it;
	}
	if (shown_)
	{
		static wchar_t temp[2];
		sf::Vector2f pos(origin_);
		temp[1] = 0;
		if (!(messages_.empty()))
			{
				for (unsigned int i = 0; i < std::string(messages_.front().str).size(); ++i)
				{
					temp[0] = std::wstring(messages_.front().str)[i];
					sf::Text r = sf::Text(sf::String(temp));
					r.setPosition(pos);
					pos.x += 2;
					chars_.push_back(r);

				}
				messages_.pop_front();
			}
	}
}

void BSMA::Show(sf::RenderTarget &r) const
{
	std::vector<sf::Text>::const_iterator it = chars_.begin();
	std::vector<sf::Text>::const_iterator end = chars_.end();
	while (it != end)
	{
		r.draw((*it));
		++it;
	}
}

BSMA::BSMA() :
shown_(true) {Init();}

BSMA::BSMA(const BSMA &other)
{
	(void)other;
}

BSMA::~BSMA()
{
	Clear();
}

void BSMA::Init(const sf::Vector2f &origin, BSMA::Direction direction, BSMA::Speed speed)
{
	origin_ = origin;
	direction_ = direction;
	speed_ = speed;
}

