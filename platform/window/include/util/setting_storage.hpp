#ifndef __SETTING_STORAGE_HPP__
#define __SETTING_STORAGE_HPP__

#include <string>
#include <util/setting.hpp>

class SettingStorage
{
  private:
    SettingStorage() = default;
    std::string filename;

  public:
    static SettingStorage& getInstance()
    {
        static SettingStorage instance;
        return instance;
    }
    void initialize(const std::string& fname);

    Setting load();

    void save(const Setting&);
};

#endif // !__SETTING_STORAGE_HPP__
