#include "Client.h"

DWORD __stdcall Client::threatListen(void *arg)
{
	SOCKET listenSocket = INVALID_SOCKET;
	SOCKET clientSocket = INVALID_SOCKET;
	WSADATA wsaData;
	int iResult;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0)
	{
		// ���������� ��������� ������
		return 1;
	}

	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(NULL, "6060", &hints, &result);

	if (iResult != 0)
	{
		// ���������� ��������� ������
		WSACleanup();
		return 1;
	}

	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listenSocket == INVALID_SOCKET)
	{
		// ���������� ��������� ������
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		// ���������� ��������� ������
		freeaddrinfo(result);
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	iResult = listen(listenSocket, 10);
	if (iResult == SOCKET_ERROR)
	{
		// ���������� ��������� ������
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	while (true)
	{
		Sleep(1000);
		clientSocket = INVALID_SOCKET;
		clientSocket = accept(listenSocket, NULL, NULL);  // !�� ���� ����� ���������� IP �������
		if (clientSocket == INVALID_SOCKET)
		{
			// ���������� ��������� ������
			continue;
		}
		// ����� ���������� �������� ���������� �����
	}

	closesocket(listenSocket);
	closesocket(clientSocket);
	WSACleanup();
	return 0;
}

DWORD __stdcall Client::threadServer(void *arg)
{
	return 0;
}

DWORD __stdcall Client::threadClient(void *arg)
{
	return 0;
}