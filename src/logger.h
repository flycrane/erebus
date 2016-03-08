#pragma once

#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/trivial.hpp>

#include <string>
#include <ostream>

enum LogLevel {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL
};

class LogChannel {
        public:
                const static std::string DEFAULT;
};

class Logger {
        using logger_sev_channel = boost::log::sources::severity_channel_logger<boost::log::trivial::severity_level, std::string>;
        public:
                static void log(std::string message, std::string channel,LogLevel l);

                static Logger& getInstance();

                void enableConsoleLogging();
                void disableTerminalLogging();

        private:
                
                Logger();

                boost::log::formatter logEverything_;

                boost::shared_ptr<boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend>> terminal_;


};

std::ostream& operator<<(std::ostream& strm, LogLevel l);
