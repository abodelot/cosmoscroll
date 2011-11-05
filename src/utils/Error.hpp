#ifndef ERROR_HPP
#define ERROR_HPP

#include <sstream>
#include <stdexcept>

class Error
{
public:
	static std::ostringstream Log;

	static std::runtime_error Exception();

	static std::runtime_error Exception(const std::string& message);
};



#endif // ERROR_HPP
