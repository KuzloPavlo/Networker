#include "Listener.h"

Listener::Listener(
	boost::asio::io_service& io_service,
	short port,
	std::shared_ptr<std::mutex> mutexOutgoingDistribution)
	: m_Acceptor(io_service, tcp::endpoint(tcp::v4(), port)),
	m_Socket(io_service), m_mutexOutgoingDistribution(mutexOutgoingDistribution)
{
	accept();
}

void Listener::accept()
{
	m_Acceptor.async_accept(m_Socket,
		[this](boost::system::error_code ec)
	{ 
	if (!ec)
	{
		display("Listener::accept New connect");
		std::make_shared<ClientSession>(std::move(m_Socket), m_mutexOutgoingDistribution, display)->start();
	}
	accept();
	});
}