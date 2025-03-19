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
#include "serv_socket.h"
#include "logger.h"
#include <thread>
#pragma comment(lib, "Ws2_32.lib") 


int main(void)
{
	setlocale(LC_ALL, "Rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	Message ms;
	Logger lg;

	bool repeat = true;
	while (true)
	{
		const char IP_SERV[] = "";			
		const int PORT_NUM = 1234;			
		const short BUFF_SIZE = 1024;		

		int erStat;
		in_addr ip_to_num; //Структура in_addr представляет собой интернет-адрес IPv4.
		erStat = inet_pton(AF_INET, "127.0.0.1", &ip_to_num);
		if (erStat <= 0)
		{
			std::cout << "Ошибка перевода IP-адреса" << '\n';
		}

		WSADATA wsData;

		erStat = WSAStartup(MAKEWORD(2, 2), &wsData);

		if (erStat != 0)
		{
			std::cout << "Ошибка инициализации WinSock-функций!" << '\n';
			std::cout << WSAGetLastError();
		}
		/*ЭТАП 2 - СОЗДАНИЕ СОКЕТА*/

		SOCKET ServSock = socket(AF_INET, SOCK_STREAM, 0);

		if (ServSock == INVALID_SOCKET)
		{
			std::cout << "Ошибка инициализации сокета: " << WSAGetLastError() << '\n';
			closesocket(ServSock); //функция закрытия сокета
			WSACleanup();          //Метод деинициализации сокетов Win32API 
		}

		sockaddr_in servInfo;  //Создание  структуры типа sockaddr_in;

		ZeroMemory(&servInfo, sizeof(servInfo));

		servInfo.sin_family = AF_INET;
		servInfo.sin_addr = ip_to_num;
		servInfo.sin_port = htons(PORT_NUM);

		erStat = bind(ServSock, (sockaddr*)&servInfo, sizeof(servInfo));

		if (erStat != 0)
		{
			std::cout << "Ошибка привязки сокета к адресу или порту. Код ошибки: " << WSAGetLastError() << '\n';
			closesocket(ServSock);
			WSACleanup();
		}
		erStat = listen(ServSock, SOMAXCONN);

		if (erStat != 0)
		{
			std::cout << "Прослушивание порта невозможно. Код ошибки: " << WSAGetLastError() << '\n';
			closesocket(ServSock);
			WSACleanup();
		}
		else
		{
			std::cout << "ПОРТ ПРОСЛУШИВАЕТСЯ..." << '\n';
		}
		sockaddr_in clientInfo;

		ZeroMemory(&clientInfo, sizeof(clientInfo));

		int clientInfo_size = sizeof(clientInfo);

		SOCKET ClientConn = accept(ServSock, (sockaddr*)&clientInfo, &clientInfo_size);

		if (ClientConn == INVALID_SOCKET) {
			std::cout << "Клиент найден, но соединение невозможно. Ошибка " << WSAGetLastError() << std::endl;
			closesocket(ServSock);
			closesocket(ClientConn);
			WSACleanup();
		}
		std::cout << "_______________________________________________________________" << '\n';
		std::cout << "СООБЩЕНИЕ ОТ КЛИЕНТА: " << '\n';

		recv_file(&ClientConn);
		ms.readPersonalMessages();
		std::thread t1(&Logger::logWriter, std::ref(lg)); //Поток, пишущий в файл логов принятые сообщения
		std::cout << '\n';
		std::thread t2(&Logger::readLastStr, std::ref(lg)); //поток, читающий из файла логов последнее принятое сообщение
		std::cout << "_______________________________________________________________" << '\n';

		Sleep(1000); //Данная функция введена для предотвращения чрезмерно раннего вызова функции createMessages(), в результате
		             //чего выведенная потоком t2 запись последней строки файла logfile попадает в поле "Отправитель" указанной функции   
		std::cout << '\n';
		std::cout << "ОТВЕТ КЛИЕНТУ: " << '\n';
		ms.createMessages();
		std::thread t3(&Logger::logWriter, std::ref(lg)); //Поток, пишущий в файл логов отправленные сообщения
		std::string path = "Messages.txt";
		send_file(&ClientConn, path);
		std::thread t4(&Logger::readLastStr, std::ref(lg)); //поток, читающий из файла логов последнее отправленное сообщение
		std::cout << '\n';
		
		t1.join();
		t2.join();
		t3.join();
		t4.join();

		closesocket(ServSock);
		closesocket(ClientConn);
		WSACleanup();

	}
	return 0;

}
