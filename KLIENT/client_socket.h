#pragma once
#include <iostream>
#include "message.h"

void send_file(SOCKET* sock, const std::string& file_name)
{
	std::fstream file;
	file.open(file_name, std::ios_base::in | std::ios_base::binary);

	if (file.is_open())
	{
		std::cout << "файл Messages.txt открыт" << '\n';
		int file_size = std::filesystem::file_size(file_name);

		char* bytes = new char[file_size];

		file.read((char*)bytes, file_size);
		//std::cout << "Размер файла: " << file_size << " байт;" << '\n';
		//std::cout << "   Имя файла: " << file_name << '\n';
		//std::cout << "      Данные: " << bytes << '\n';

		send(*sock, std::to_string(file_size).c_str(), 16, 0);
		send(*sock, file_name.c_str(), 32, 0);
		send(*sock, bytes, file_size, 0);

		delete[] bytes;
	}
	else
	{
		std::cout << "send-client: Ошибка открытия файла!" << '\n';
	}
	file.close();
}

void recv_file(SOCKET* sock)
{
	char file_size_str[16];
	char file_name[32];

	recv(*sock, file_size_str, 16, 0);
	int file_size = atoi(file_size_str);
	char* bytes = new char[file_size];

	recv(*sock, file_name, 32, 0);

	std::fstream file;
	file.open(file_name, std::ios_base::out | std::ios_base::binary);

	//std::cout << "Размер файла: " << file_size << " байт;" << '\n';
	//std::cout << "   Имя файла: " << file_name << '\n';

	if (file.is_open())
	{
		recv(*sock, bytes, file_size, 0);
		//std::cout << "      Данные: " << bytes << '\n';

		file.write(bytes, file_size);
		//std::cout << "Данные сохранены!" << '\n';
	}
	else
	{
		std::cout << "recv-client: Ошибка открытия файла!" << '\n';
	}
	delete[] bytes;
	file.close();
}
