#pragma once

#include <string>

enum class LogLevel {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL
};

class LoggerChannel {
        public:
                const static std::string DEFAULT;
};

class Logger {
        public:
                static void log(std::string message, std::string channel,LogLevel l);

};
