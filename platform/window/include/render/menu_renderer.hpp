
#include "menu.hpp"
#include "util/setting.hpp"
#include "window_renderer.hpp"

class MenuRenderer : public WindowRenderer
{
  public:
    void render_menu_frame();
    void render_menu(Menu);
    void render_settings_frame();
    void render_settings(SettingMenu, Setting&);
    void render_side();
};
