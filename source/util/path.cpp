#include "util/path.hpp"

using namespace std;

PathService& PathService::get_instance()
{
    static PathService instance;
    return instance;
}