#ifndef __WPF_NETWORK_HPP__
#define __WPF_NETWORK_HPP__

#include "callback/callbacks.hpp"
#include "api/i_network.hpp"

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

class WpfNetwork : public INetwork
{
private:
	Callbacks* cbs;
public:
	WpfNetwork();

	void set_callback(Callbacks*);

    void send_udp(const Board& board, const Tetromino& tetromino, const char* another_user_ip) override;
    bool recv_udp(packet& recv_pkt) override;

    ~WpfNetwork();
};

#endif