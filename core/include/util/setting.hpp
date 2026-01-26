#ifndef __SETTING_HPP__
#define __SETTING_HPP__

#include <cstdint>
#include <cstring>
#include <string>

class Setting
{
  public:
    Setting()
        : color_theme(0), shadow_on(true), nick_name("Player"), server_ip_address("127.0.0.1"),
          server_port("41234")
    {
    }

    uint8_t color_theme;
    bool shadow_on;
    std::string nick_name;
    std::string server_ip_address;
    std::string server_port;

    void set(const std::string& key, const std::string& value)
    {
        if (key == "nick_name") {
            nick_name = value;
        }
        else if (key == "shadow_on") {
            shadow_on = (value == "1");
        }
        else if (key == "color_theme") {
            color_theme = static_cast<uint8_t>(stoi(value));
        }
        else if (key == "server_ip_address") {
            server_ip_address = value;
        }
        else if (key == "server_port") {
            server_port = value;
        }
    }
};

#endif // !__SETTING_HPP__
