#ifndef ERROR_HPP
#define ERROR_HPP

#include <sstream>
#include <stdexcept>

class Error
{
public:
	static std::ostringstream log;

	static std::runtime_error exception();

	static std::runtime_error exception(const std::string& message);

	static void dump();
};



#endif // ERROR_HPP
