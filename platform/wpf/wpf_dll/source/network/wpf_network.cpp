#include "callback/callbacks.hpp"
#include "network/wpf_network.hpp"

WpfNetwork::WpfNetwork()
{
}

void WpfNetwork::set_callback(Callbacks* cbs)
{
	this->cbs = cbs;
}

void WpfNetwork::send_udp(const Board& board, const Tetromino& tetromino, const char* another_user_ip)
{
	return;
}

bool WpfNetwork::recv_udp(packet& recv_pkt)
{
	return false;
}

WpfNetwork::~WpfNetwork()
{

}