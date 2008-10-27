#include "Misc.hpp"


std::string str_sprintf(const char format[], ...)
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


int find_replace(std::string& target, const std::string& look_for, const std::string& replace_by)
{
    short unsigned cpt = 0;
    size_t pos = 0;
    size_t step = replace_by.size();
    size_t offset = look_for.size();

    pos = target.find(look_for, pos);

    while (pos != std::string::npos)
    {
        target.replace(pos, offset, replace_by);
        pos = target.find(look_for, pos + step);
        ++cpt;
    }
    return cpt;
}

