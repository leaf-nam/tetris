#include "util/path.hpp"

using namespace std;

PathService::PathService()
{
    filesystem::path path = filesystem::path(getenv("APPDATA")) / "tetrissen";
    filesystem::create_directories(path);
    
}

string PathService::get_path()
{
    return path;
}