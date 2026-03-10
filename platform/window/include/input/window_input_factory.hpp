#ifndef __INPUT_FACTORY_HPP__
#define __INPUT_FACTORY_HPP__

#include "i_input_factory.hpp"
#include "input/window_input.hpp"
#include "input/window_lobby_input_handler.hpp"

class WindowInputFactory : public IInputFactory
{
  public:
    IInputHandler* create_input_handler() override;
    ISettingInputHandler* create_setting_input_handler() override;
    IMenuInputHandler* create_menu_input_handler() override;
    ILobbyInputHandler* create_lobby_input_handler() override;
};

#endif // !__INPUT_FACTORY_HPP__
