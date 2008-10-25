#include "Misc.hpp"


std::string epic_sprintf(const char format[], ...)
{
    va_list args;
    va_start(args, format);
    
    int length = vsnprintf(NULL, 0, format, args);
    char* p = new char [length + 1];
    va_end(args);
    va_start(args, format);
    vsprintf(p, format, args);
    va_end(args);    
    
    std::string str(p);
    delete [] p;
    return str;
}

