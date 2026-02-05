#ifndef __SETTING_STORAGE_HPP__
#define __SETTING_STORAGE_HPP__

#include "setting.hpp"

#include <string>

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
