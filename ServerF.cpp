﻿
#define WIN32_LEAN_AND_MEAN

#include <iostream>
#pragma comment(lib,"ws2_32.lib")
#include<WinSock2.h>
#include<string>
#pragma warning(disable: 4996)
#include<WS2tcpip.h>

SOCKET Connections[100];
int Counter;

void ClientPrecieve(int index)
{
	char buffer[522];
	char msg[512];
	while (true) {
		int result = 0;
		int result2 = 0;
		result = recv(Connections[index], buffer, sizeof(buffer), NULL);
		result2 = recv(Connections[index], msg, sizeof(msg), NULL);
		if (result > 0 && result2 > 0)
		{
			for (int i = 0; i < Counter; i++)
			{
				if (i == index)continue;

				send(Connections[i], buffer, sizeof(buffer), NULL);
				send(Connections[i], msg, sizeof(msg), NULL);
			}
		}
	}
}
int main(int argc, char* argv[])
{
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0)
	{
		std::cout << "Error" << std::endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	SOCKET newConnection;
	for (int i = 0; i < 100; i++)
	{
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);
		if (newConnection == 0)
		{
			std::cout << "Error 2#\n";
		}
		else {
			std::cout << "Client Connected!\n";
			char msg[256] = "Hello. It's my first network mprogram!";
			send(newConnection, msg, sizeof(msg), NULL);
			Connections[i] = newConnection;
			Counter++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientPrecieve, (LPVOID)(i), NULL, NULL);

		}
	}

	system("pause");
	return 0;
}