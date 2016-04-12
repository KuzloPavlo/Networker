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
	{ std::ofstream out("C:\\Users\\PC\\Desktop\\777\\testttt.txt", std::ios::out | std::ios::binary);
	if (!ec)
	{
		std::make_shared<ClientSession>(std::move(m_Socket), m_mutexOutgoingDistribution)->start();
	}
	accept();
	});
}

