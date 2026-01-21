
#include <cstdint>
#include <string>

using namespace std;

enum ColorTheme : uint8_t
{
    DRACULA = 0,
    MONOKAI,
    NORD,
    SOLARIZED_DARK
};

class Setting
{
  public:
    ColorTheme color_theme;
    bool shadow_on;
    string nick_name;
    string server_ip_address;
    string server_port;
};
