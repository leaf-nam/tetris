#include "input/linux_input_factory.hpp"

IInputHandler* LinuxInputFactory::create_input_handler()
{
    return new LinuxInput();
}

ISettingInputHandler* LinuxInputFactory::create_setting_input_handler()
{
    return new LinuxInput();
}

IMenuInputHandler* LinuxInputFactory::create_menu_input_handler()
{
    return new LinuxInput();
}

ILobbyInputHandler* LinuxInputFactory::create_lobby_input_handler()
{
    return new LinuxLobbyInputHandler();
} 
