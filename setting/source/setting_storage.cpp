#include "setting_storage.hpp"

#include <filesystem>
#include <fstream>

using namespace std;

void SettingStorage::initialize(const std::string& fname) { this->filename = fname; }

Setting SettingStorage::load()
{
    Setting setting;

    ifstream in(filename);

    if (!in.is_open()) {
        save(setting);
        return setting;
    }

    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;

        auto pos = line.find('=');
        if (pos == string::npos) continue;

        string key = line.substr(0, pos);
        string value = line.substr(pos + 1);

        setting.set(key, value);
    }

    return setting;
}

void SettingStorage::save(const Setting& setting)
{
    ofstream out(filename);

    if (!out.is_open()) {
        filesystem::path path_obj(filename);
        filesystem::create_directories(path_obj.parent_path());
        out.open(filename);
    }

    out << "nick_name=" << setting.nick_name << '\n'
        << "shadow_on=" << (setting.shadow_on ? "true" : "false") << '\n'
        << "color_theme=" << static_cast<int>(setting.color_theme) << '\n'
        << "server_ip_address=" << setting.server_ip_address << '\n'
        << "server_port=" << setting.server_port << '\n';
}
