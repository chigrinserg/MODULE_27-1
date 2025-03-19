#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <shared_mutex>

class Message;

class Logger
{
private:
    std::ofstream logfile;
    std::string line;
    std::shared_mutex sm;

public:
    Logger() {}

    Logger(const std::string& filename);

    ~Logger(){} //По всей видимости, вводить процедуру закрытия файла в деструктор нет необходимости, поскольку функция close() и так имеется
                //в каждом методе класса

    void logWriter();
    void readLastStr();
};
