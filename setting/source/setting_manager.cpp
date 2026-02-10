#include "setting_manager.hpp"

#include "i_input_handler.hpp"
#include "render/theme.hpp"

using namespace std;

int stting_menu_size = static_cast<int>(SettingTitle::END) - static_cast<int>(SettingTitle::BEGIN);

const char* SETTING_TITLE[4] = {"NICKNAME", "THEME", "SHADOW", "SAVE"};

SettingManager::SettingManager(Setting* a1, ISettingRenderer* a2, ISettingInputHandler* a3,
                               SettingStorage* a4)
    : setting(a1), setting_title(SettingTitle::NICKNAME), render(a2), input(a3), setting_storage(a4)
{
}

void SettingManager::reload()
{
    render->render_settings_frame();
    render->render_settings(setting_title);
}

AppState SettingManager::update()
{
    int in = input->scan();

    // 엔터 처리
    if (in == '\r' || in == ' ') {
        switch (setting_title) {
        // 닉네임 변경
        case SettingTitle::NICKNAME: {
            render->render_input_window({27, 20}, "Type your nickname.[length : 1 ~ 8]");
            string nickname = input->get_line();

            if (!nickname.empty() && nickname.size() <= 8) {
                setting->nick_name = nickname;
            }

            reload();
            break;
        }

        // 테마 변경
        case SettingTitle::THEME: {
            setting->color_theme = (setting->color_theme + 1) % 4;
            Theme::getInstance().apply(static_cast<ThemeKey>(setting->color_theme));
            reload();
            break;
        }

        // 그림자 토글
        case SettingTitle::SHADOW: {
            setting->shadow_on = !setting->shadow_on;
            reload();
            break;
        }

        // 저장 및 종료
        case SettingTitle::SAVE: {
            setting_storage->save(*setting);
            return AppState::MENU;
        }
        default:
            return AppState::SETTINGS;
        }
    }

    // left arrow
    if (in == Arrow::KEY_LEFT) {
        switch (setting_title) {
        case SettingTitle::THEME:
            setting->color_theme = (setting->color_theme + 3) % 4;
            Theme::getInstance().apply(static_cast<ThemeKey>(setting->color_theme));
            reload();
            break;
        case SettingTitle::SHADOW:
            setting->shadow_on = !setting->shadow_on;
            reload();
            break;
        default:
            break;
        }
    }

    // right arrow
    if (in == Arrow::KEY_RIGHT) {
        switch (setting_title) {
        case SettingTitle::THEME:
            setting->color_theme = (setting->color_theme + 1) % 4;
            Theme::getInstance().apply(static_cast<ThemeKey>(setting->color_theme));
            reload();
            break;
        case SettingTitle::SHADOW:
            setting->shadow_on = !setting->shadow_on;
            reload();
            break;
        default:
            break;
        }
    }

    // down arrow
    if (in == Arrow::KEY_DOWN || in == '\t') {
        setting_title = next_menu(setting_title);
        render->render_settings(setting_title);
    }

    // up arrow
    if (in == Arrow::KEY_UP) {
        setting_title = prev_menu(setting_title);
        render->render_settings(setting_title);
    }

    // esc
    if (in == 27) return AppState::EXIT;

    return AppState::SETTINGS;
}

SettingTitle SettingManager::next_menu(SettingTitle m)
{
    return static_cast<SettingTitle>((static_cast<uint8_t>(m) + 1) % stting_menu_size);
}

SettingTitle SettingManager::prev_menu(SettingTitle m)
{
    return static_cast<SettingTitle>((static_cast<uint8_t>(m) + (stting_menu_size - 1)) %
                                     stting_menu_size);
}
