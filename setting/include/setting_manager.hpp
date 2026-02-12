#ifndef __SETTING_MANAGER_HPP__
#define __SETTING_MANAGER_HPP__

#include "app_state.hpp"
#include "i_setting_input_handler.hpp"
#include "i_setting_renderer.hpp"
#include "setting_storage.hpp"

class SettingManager
{
  private:
    Setting* setting;
    SettingTitle setting_title;
    ISettingInputHandler* input;
    ISettingRenderer* render;
    SettingStorage* setting_storage;
    SettingTitle next_menu(SettingTitle);
    SettingTitle prev_menu(SettingTitle);

  public:
    SettingManager(Setting*, ISettingRenderer*, ISettingInputHandler*, SettingStorage*);
    void reload();
    AppState update();
};

#endif //!__SETTING_MANAGER_HPP__
