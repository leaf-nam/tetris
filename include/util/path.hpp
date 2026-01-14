#ifndef __PATH_HPP__
#define __PATH_HPP__

#include <string>
#include <filesystem>

class PathService
{
private:
    std::string path;
    PathService();
public:
    static PathService& get_instance();
    PathService(const PathService&) = delete;
    PathService& operator=(const PathService&) = delete;
    std::string get_path();
};

#endif
