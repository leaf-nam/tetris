#include "input/window_input_factory.hpp"

IInputHandler* WindowInputFactory::create_input_handler()
{
    return new WindowInput();
}

ISettingInputHandler* WindowInputFactory::create_setting_input_handler()
{
    return new WindowInput();
}

IMenuInputHandler* WindowInputFactory::create_menu_input_handler()
{
    return new WindowInput();
}

ILobbyInputHandler* WindowInputFactory::create_lobby_input_handler()
{
    return new WindowLobbyInputHandler();
} 
