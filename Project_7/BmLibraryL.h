#pragma once

#include <string>
#include <fstream>
#include "BmLibrary.h"

enum Level : int
{
    BEGIN,
    FLIGHT,
    END,
    DEBUG,
    INFO
};

//! Логгер в контексте БР
class Logger
{
public:
    static Logger& instance()
    {
        static Logger logger;
        return logger;
    }

    void log(Level p_level, const std::string& p_message, tPoint p) const
    {
        if (p_level < c_level) return;

        std::ofstream file(m_fileName, std::ios::app);
        if (file.is_open())
        {
            if (p_level == FLIGHT)
            {
                file << "[" << levelToString(p_level) << "] " << p_message << " x = " << p.x << ", y = " << p.y << std::endl;
            }
            else
            {
                file << "[" << levelToString(p_level) << "] " << p_message << std::endl;
            }

            file.close();
        }
    }

    void Inflog(Level p_level, const std::string& p_message)
    {
        std::ofstream file(m_fileName, std::ios::app);
        if (file.is_open())
        {
            if (p_level == INFO)
            {
                //        system:[ INFO]: Completed: 0.0
                file << "system:[ " << levelToString(p_level) << "]: " << p_message << std::endl;
            }
        }
    }

    void Deblog(Level p_level, int count)
    {
        std::ofstream file(m_fileName, std::ios::app);
        if (file.is_open())
        {
            if (p_level == DEBUG)
            {
                //        system:[DEBUG]: New strike element added: 49
                file << "system:[" << levelToString(p_level) << "]: New strike element added: " << count << std::endl;
            }
        }
    }

    void loglevel(Level p_level)
    {
        c_level = p_level;
    }

    Level loglevel() const
    {
        return c_level;
    }

    bool isEnabled(Level p_level) const
    {
        return p_level >= c_level;
    }

private:
    Logger() : c_level(BEGIN), m_fileName("log.txt") {}

    std::string levelToString(Level p_level) const
    {
        switch (p_level)
        {
        case BEGIN:
            return "BEGIN";
        case FLIGHT:
            return "FLIGHT";
        case END:
            return "END";
        case DEBUG:
            return "DEBUG";
        case INFO:
            return "INFO";
        default:
            return "";
        }
    }

    Level c_level;
    std::string m_fileName;
};