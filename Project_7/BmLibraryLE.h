#pragma once

#include <string>
#include <fstream>

enum Level_errors : int
{
    TYPEofMODEL,
    INTERPOLATION,
    CONTRACT_RESTRICTION,
    RANGEofTIME,
    NOT_FOUND,
    NOT_PREPARE,
};

//!Логгирование в контексте приложения
class Logger_errors
{
public:
    static Logger_errors& instance()
    {
        static Logger_errors logger;
        return logger;
    }

    void log(Level_errors p_level, const std::string& p_message) const
    {
        if (p_level < c_level) return;

        std::ofstream file(m_fileName, std::ios::app);
        if (file.is_open())
        {
            file << "[" << levelToString(p_level) << "] " << p_message << std::endl;
            file.close();
        }
    }

    void loglevel(Level_errors p_level)
    {
        c_level = p_level;
    }

    Level_errors loglevel() const
    {
        return c_level;
    }

    bool isEnabled(Level_errors p_level) const
    {
        return p_level >= c_level;
    }

private:
    Logger_errors() : c_level(TYPEofMODEL), m_fileName("log_errors.txt") {}

    std::string levelToString(Level_errors p_level) const
    {
        switch (p_level)
        {
        case TYPEofMODEL:
            return "TYPEofMODEL";
        case INTERPOLATION:
            return "INTERPOLATION";
        case CONTRACT_RESTRICTION:
            return "CONTRACT_RESTRICTION";
        case RANGEofTIME:
            return "RANGEofTIME";
        case NOT_FOUND:
            return "NOT_FOUND";
        case NOT_PREPARE:
            return "NOT_PREPARE";
        default:
            return "";
        }
    }

    Level_errors c_level;
    std::string m_fileName;
};