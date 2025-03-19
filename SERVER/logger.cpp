#include "logger.h"
#include <vector>
#include <iostream>

Logger::Logger(const std::string& filename)
{
        //const std::string& filename = "logfile.txt";
    logfile.open(filename, std::ios::app);
    if (!logfile.is_open())
    {
        std::cerr << "Îøèáêà îòêðûòèÿ ôàéëà ëîãîâ" << std::endl;
    }
    sm.unlock();
}
//Logger::~Logger(){}


void Logger::logWriter()
{
    sm.lock();
    std::string line;
    //sm.lock();
    std::ifstream in("Messages.txt"); // îêðûâàåì ôàéë äëÿ ÷òåíèÿ 
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
    in.close();     // çàêðûâàåì ôàéë
}

void Logger::readLastStr() /*Ôóíêöèÿ ÷òåíèÿ ïåðñîíàëüíûõ ñîîáùåíèé - ïîñëåäíåé ñòðîêè â ôàéëå.
                           Ïîëó÷åíà ñ ñàéòà https://stackoverflow.com/questions/11876290/c-fastest-way-to-read-only-last-line-of-text-file.
                           Вначале я написал функцию с использованием загрузки файла в вектор с последующим чтением последнего элемента
                           массива. Но поскольку загрузка вектора имеет квадратичную временную сложность и в случае большого размера текстового
                           файла это было бы конкретное торможение, поискал альтернативный вариант*/
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
        std::cout << "Ïîñëåäíÿÿ çàïèñü â ôàéëå àðõèâà: " << lastLine << '\n';     
        sm.unlock_shared();
        fin.close();
    }
}




