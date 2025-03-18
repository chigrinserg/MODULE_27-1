#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include "message.h"
#include <WinSock2.h> 
#include <WS2tcpip.h> 
#include <stdio.h>
#include <Windows.h> 
#include "client_socket.h"
#pragma comment(lib, "Ws2_32.lib") 

int main()
{
	setlocale(LC_ALL, "Rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	Message ms;


	bool repeat = true;
	while (true)
	{
		const char SERVER_IP[] = "";					
		const short SERVER_PORT_NUM = 1234;				
		const short BUFF_SIZE = 1024;
		int erStat;
		in_addr ip_to_num;
		erStat = inet_pton(AF_INET, "127.0.0.1", &ip_to_num);

		WSADATA wsData;

		erStat = WSAStartup(MAKEWORD(2, 2), &wsData);

		if (erStat != 0)
		{
			std::cout << "Ошибка инициализации WinSock-функций!" << '\n';
			std::cout << WSAGetLastError();
		}

		/*ЭТАП 2 - СОЗДАНИЕ СОКЕТА*/

		SOCKET ClientSock = socket(AF_INET, SOCK_STREAM, 0);

		if (ClientSock == INVALID_SOCKET)
		{
			std::cout << "Ошибка инициализации сокета: " << WSAGetLastError() << '\n';
			closesocket(ClientSock); 
			WSACleanup();          
		}
	
		sockaddr_in servInfo;  

		ZeroMemory(&servInfo, sizeof(servInfo));
		
		servInfo.sin_family = AF_INET;
		servInfo.sin_addr = ip_to_num;   
		servInfo.sin_port = htons(SERVER_PORT_NUM);

		erStat = connect(ClientSock, (sockaddr*)&servInfo, sizeof(servInfo));

		if (erStat != 0)
		{
			std::cout << "Сбой подключения к серверу. Код ошибки: " << WSAGetLastError() << std::endl;
			closesocket(ClientSock);
			WSACleanup();
		}
		std::cout << "________________________________________________________________" << '\n';
		std::cout << "СООБЩЕНИЕ СЕРВЕРУ: " << '\n';
		ms.createMessages();
		std::string path = "Messages.txt";
		send_file(&ClientSock, path);
		std::cout << '\n';

		std::cout << "ОТВЕТ СЕРВЕРА: " << '\n';
		recv_file(&ClientSock);
		ms.readPersonalMessages();

		closesocket(ClientSock);
		WSACleanup();

		int z{};
		std::cout << '\n';
		std::cout << "Продолжить - 1, завершить - '0'" << '\n';
		std::cin >> z;
		if (z == 1)
		{
			repeat = true;
		}
		else
		{
			repeat = false;
			break;
		}

	}

	return 0;
}