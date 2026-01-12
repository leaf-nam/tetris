#ifndef __WPF_INPUT_HANDLER_HPP__
#define __WPF_INPUT_HANDLER_HPP__

#include "api/i_input_handler.hpp"

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

class WpfInputHandler : public IInputHandler 
{
private:
	Callbacks* cbs;
public:
	WpfInputHandler();

	void set_callback(Callbacks*);

	char scan() override;

	~WpfInputHandler() override;
};

#endif