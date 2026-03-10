#ifndef __SETTING_TITLE__HPP__
#define __SETTING_TITLE__HPP__

enum class SettingTitle
{
    BEGIN = 0,

    NICKNAME = BEGIN,
    THEME,
    SHADOW,
    SAVE,

    END
};

extern const char* SETTING_TITLE[4];
extern int setting_size;

#endif
