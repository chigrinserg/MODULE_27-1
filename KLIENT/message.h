#pragma once
#include <iostream>
#include <string>
#include <vector>

class Message
{
    std::string m_time;   //���������� �������� �������
    std::string m_nameOf; //���������� ����� �����������
    std::string m_nameTo; //���������� ����� ����������
    std::string m_text;   //���������� ������ ���������
public:
    Message();
    Message(std::string& time, std::string& nameOf, std::string& nameTo, std::string& text);
    std::string Time(); //��������� �������� �������
    std::vector<Message> array; //������ ������ ���������
    void setData();//��������� ������ ������������ ���������
    void getData(); //��������� � ����� ��������� � ������ ������
    void createMessages(); //�������� ������������ ���������
    static int countRecords(const std::string name_file); //������� ����� ����� � ��������� �����
    void readPersonalMessages(); //������ ������������ ���������
    friend std::ostream& operator<< (std::ostream& out, const Message& z);
    friend std::istream& operator>> (std::istream& in, Message& z);
    ~Message();
};