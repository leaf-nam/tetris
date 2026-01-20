#include "input/wpf_input_handler.hpp"

#include "callback/callbacks.hpp"

WpfInputHandler::WpfInputHandler() {}

void WpfInputHandler::set_callback(Callbacks* cbs) { this->cbs = cbs; }

char WpfInputHandler::scan() { return this->cbs->scan_callback(); }

WpfInputHandler::~WpfInputHandler() {}