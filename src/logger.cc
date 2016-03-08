#include "logger.h"

#include <string>
#include <iostream>

namespace {
        std::string LogLevelToString(LogLevel l) {
                switch(l) {
                        case LogLevel::DEBUG:
                                return "debug";
                        default:
                                return "invalid";
                }
        }
}

const std::string LoggerChannel::DEFAULT="default";

void Logger::log(const std::string& message, const std::string& channel, LogLevel l) {
        std::cout<<message<<" ["<<channel<<"] "<<LogLevelToString(l)<<std::endl;
}
