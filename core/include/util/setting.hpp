#ifndef __SETTING_HPP__
#define __SETTING_HPP__

#include <cstdint>
#include <string>

using namespace std;

class Setting
{
  public:
    uint8_t color_theme;
    bool shadow_on;
    string nick_name;
    string server_ip_address;
    string server_port;
};

#endif // !__SETTING_HPP__
