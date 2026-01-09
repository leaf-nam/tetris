#include "callbacks.hpp"
#include "input/wpf_input_handler.hpp"

WpfInputHandler::WpfInputHandler() 
{

}

string WpfInputHandler::scan() 
{
	return this->cbs.scan_callback();
}

void WpfInputHandler::set_callback(Callbacks cbs) 
{
	this->cbs = cbs;
}

WpfInputHandler::~WpfInputHandler() 
{
	
}