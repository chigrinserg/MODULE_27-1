#define _CRT_SECURE_NO_WARNINGS //������� ��� �������������� ���� � ������� localtime(&timep)
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <fstream>
#include "message.h"

Message::Message() {};
Message::Message(std::string& time, std::string& nameOf, std::string& nameTo, std::string& text)
{
    m_time = time;
    m_nameOf = nameOf;
    m_nameTo = nameTo;
    m_text = text;
}

Message::~Message() {};

std::string Message::Time() //������� ��������� �������� �������
{
    time_t timep;
    struct tm* p;
    time(&timep); // ��������, ������� ������ ������ � 1970 ����, � ��������� ��� � timep ���� time_t
    p = localtime(&timep);// ���������� localtime ��� �������������� ���������� ������ � ��������� struct tm
    int day = p->tm_mday;
    int mon = 1 + p->tm_mon;
    int year = 1900 + p->tm_year;
    int hour = p->tm_hour;
    int min = p->tm_min;
    int sec = p->tm_sec;
    std::string currentTime = (std::to_string(day) + "." + std::to_string(mon) + "." + std::to_string(year) + "_" + std::to_string(hour) + ":"
        + std::to_string(min) + ":" + std::to_string(sec));
    return currentTime;
}

std::ostream& operator<< (std::ostream& out, const Message& z)
{
    out << z.m_time << " " << " " << " " << z.m_nameOf << " " << " " << z.m_nameTo << " " << z.m_text;
    return out;
}

std::istream& operator>> (std::istream& in, Message& z)
{
    in >> z.m_time;
    in >> z.m_nameOf;
    in >> z.m_nameTo;
    in >> z.m_text;
    return in;
}

void Message::setData()  //��������� ������ ���������
{
    m_time = Time();     //������� � ��������� ������� ��������
    std::cout << "�����������: ";
    std::cin >> m_nameOf; //������� ������ �����������
    std::cout << " ����������: ";
    std::cin >> m_nameTo; //������� ������ ����������
    std::cout << "  ���������: ";
    std::getline(std::cin >> std::ws, m_text); //������������ ����  ������ � ���������
    for (size_t i = static_cast<int>(m_text.find(' ')); i != -1; i = m_text.find(' ')) m_text.replace(i, 1, "_");
}

void Message::getData() //��������� ������ ���������
{
    std::cout << "  ����������: " << m_time << '\n';
    std::cout << "     �� ����: " << m_nameOf << '\n';
    std::cout << "        ����: " << m_nameTo << '\n';
    std::cout << "   ���������: " << m_text << '\n';
}

Message message; //���������� ���������� ������ ��� ������������� � ������� �������� � ���������� ���������


void Message::createMessages() //������� �������� ������ ���������
{
    int size{ 1 };
    std::vector<Message> array(size); //������ ���������� ���������
    for (int i = 0; i < size; ++i)
    {
        array[i].setData();
    }

    std::ofstream out("Messages.txt"); //��������� ������ � ��������� ����
    for (int i = 0; i < (end(array) - begin(array)); ++i)
    {
        out << array[i] << '\n';
    }
}

int Message::countRecords(const std::string name_file) // ������������ ����� ����� � �����
{
    int count = 0;
    std::ifstream input_file("Messages.txt");
    std::string s;
    while (!input_file.eof())
    {
        getline(input_file, s);
        ++count;
    }
    input_file.close();
    return count;
}

void Message::readPersonalMessages() //������� ������ ������������ ���������
{
    std::vector<Message>array(Message::countRecords("Messages.txt"));
    std::ifstream input_file("Messages.txt", std::ios::in);
    for (int i = 0; i < (Message::countRecords("Messages.txt")); ++i)
    {
        input_file >> array[i];
    }
    input_file.close();

    for (int i = 0; i < (Message::countRecords("Messages.txt")) - 1; ++i)
    {
        array[i].getData();
    }

}
