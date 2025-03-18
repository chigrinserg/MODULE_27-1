#include "logger.h"
#include <vector>
#include <iostream>

Logger::Logger(const std::string& filename)
{
        //const std::string& filename = "logfile.txt";
    logfile.open(filename, std::ios::app);
    if (!logfile.is_open())
    {
        std::cerr << "Ошибка открытия файла логов" << std::endl;
    }
    sm.unlock();
}
//Logger::~Logger(){}


void Logger::logWriter()
{
    sm.lock();
    std::string line;
    //sm.lock();
    std::ifstream in("Messages.txt"); // окрываем файл для чтения
    if (in.is_open())
    {
        while (std::getline(in, line))
        {
            std::ofstream out("logfile.txt", std::ios::app);
            if (out.is_open())
            {
               out << line << std::endl;
            }
            out.close();
        }
    }
    sm.unlock();
    in.close();     // закрываем файл
}

void Logger::readLastStr() /*Функция чтения персональных сообщений - последней строки в файле.
                           Получена с сайта https://stackoverflow.com/questions/11876290/c-fastest-way-to-read-only-last-line-of-text-file*/
{
    sm.lock_shared();
    std::string filename = "logfile.txt";
    std::ifstream fin;
    fin.open(filename);
    if (fin.is_open())
    {
        fin.seekg(-3, std::ios_base::end);                

        bool keepLooping = true;
        while (keepLooping)
        {
            char ch;
            fin.get(ch);                            

            if ((int)fin.tellg() <= 1)             
            {
                fin.seekg(0);                       
                keepLooping = false;                
            }
            else if (ch == '\n') 
            {
                keepLooping = false;               
            }
            else
            {                                  
                fin.seekg(-2, std::ios_base::cur);        
            }
        }
        std::string lastLine;
        std::getline(fin, lastLine);                      
        std::cout << "Последняя запись в файле архива: " << lastLine << '\n';     
        sm.unlock_shared();
        fin.close();
    }
}




