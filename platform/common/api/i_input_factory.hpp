#ifndef __I_INPUT_FACTORY_HPP__
#define __I_INPUT_FACTORY_HPP__

#include "i_input_handler.hpp"
#include "i_lobby_input_handler.hpp"
#include "i_setting_input_handler.hpp"
#include "i_menu_input_handler.hpp"

class IInputFactory
{
  public:
    virtual IInputHandler* create_input_handler() = 0;
    virtual ISettingInputHandler* create_setting_input_handler() = 0;
    virtual IMenuInputHandler* create_menu_input_handler() = 0;
    virtual ILobbyInputHandler* create_lobby_input_handler() = 0;
    virtual ~IInputFactory() = default;
};

#endif // !__I_INPUT_FACTORY_HPP__
