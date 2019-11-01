#include "FileSystem.hpp"

#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>

#if defined(_WIN32) || defined(__WIN32__)
    // Windows
    #define SYS_WINDOWS
    #include <direct.h>     // _mkdir
    #if defined(_MSC_VER)
        #define S_ISDIR(mode) (((mode) & S_IFMT) == S_IFDIR)
        #define S_ISREG(mode) (((mode) & S_IFMT) == S_IFREG)
    #endif

#elif defined(__linux__)
    // Linux
    #define SYS_LINUX

#elif defined(__APPLE__) || defined(MACOSX) || defined(macintosh) || defined(Macintosh)
    // Mac OS
    #define SYS_MACOS

#endif

namespace filesystem {

bool is_directory(const std::string& path)
{
    struct stat sb;
    if (stat(path.c_str(), &sb) == 0)
    {
        return S_ISDIR(sb.st_mode);
    }
    return false;
}


bool is_file(const std::string& path)
{
    struct stat sb;
    if (stat(path.c_str(), &sb) == 0)
    {
        return S_ISREG(sb.st_mode);
    }
    return false;
}


bool create_directory(const std::string& name)
{
    bool success = false;

#ifdef SYS_WINDOWS
    success = _mkdir(name.c_str()) == 0;
#elif defined(SYS_LINUX) || defined(SYS_MACOS)
    success = mkdir(name.c_str(), 0755) == 0;
#endif

    if (!success)
        std::cerr << "cannot create directory " << name << std::endl;
    return success;
}


std::string init_settings_directory(const std::string& appname)
{
    std::string config_dir;

    // Set config directory path
#ifdef SYS_WINDOWS
    config_dir = getenv("APPDATA");
    config_dir += "\\" + appname;

#elif defined(SYS_LINUX)
    config_dir = getenv("HOME");
    config_dir += "/.config/" + appname;

#elif defined(SYS_MACOS)
    config_dir = getenv("HOME");
    config_dir += "/Library/Application Support/" + appname;

#endif

    // Create directory if it doesn't already exist
    if (!is_directory(config_dir))
    {
        create_directory(config_dir);
    }
    return config_dir;
}

}
