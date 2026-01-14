#include "util/path.hpp"
#include <filesystem>

using namespace std;
static_assert(__cplusplus >= 201703L, "C++아님");
PathService::PathService()
{
    filesystem::path path = filesystem::path(getenv("APPDATA")) / "tetrissen";
    filesystem::create_directories(path);
    
}

string PathService::get_path()
{
    return "";
}