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
                static void log(const std::string& message, const std::string& channel,LogLevel l);

};
