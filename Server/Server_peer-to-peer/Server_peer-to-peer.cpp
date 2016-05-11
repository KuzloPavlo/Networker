// Server_peer-to-peer.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "resource.h"
#include "server.h"
#include "session.h"
#include "MyFile.h"
#define SIZE_FOR_COUNT 4

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");
	try
	{
		boost::asio::io_service io_service;
		std::thread out(Show);
		out.detach();
		server s(io_service, std::atoi("33333"));
		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
	return 0;
}
