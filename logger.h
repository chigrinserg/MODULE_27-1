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

    ~Logger(){}

    void logWriter();
    void readLastStr();
};