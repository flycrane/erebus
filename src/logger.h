#pragma once

#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions/filter.hpp>

#include <string>
#include <ostream>
#include <map>

#define LOG(severity,channel,message) Logger::log(message,channel,boost::log::trivial::severity); 

#define LOG_TRACE(channel,message) LOG(trace,channel,message)
#define LOG_DEBUG(channel,message) LOG(debug,channel,message)
#define LOG_INFO(channel,message) LOG(info,channel,message)
#define LOG_WARNING(channel,message) LOG(warning,channel,message)
#define LOG_ERROR(channel,message) LOG(error,channel,message)
#define LOG_FATAL(channel,message) LOG(fatal,channel,message)

#define LOG_TRACED(message) LOG(trace,LogChannel::DEFAULT,message)
#define LOG_DEBUGD(message) LOG(debug,LogChannel::DEFAULT,message)
#define LOG_INFOD(message) LOG(info,LogChannel::DEFAULT,message)
#define LOG_WARNINGD(message) LOG(warning,LogChannel::DEFAULT,message)
#define LOG_ERRORD(message) LOG(error,LogChannel::DEFAULT,message)
#define LOG_FATALD(message) LOG(fatal,LogChannel::DEFAULT,message)


class LogChannel {
        public:
                const static std::string DEFAULT;
};

class LogFormat {
        public:
                static boost::log::formatter getLogEverythingFormatter();
                static boost::log::formatter getLogChannelFormatter();
                static boost::log::formatter getLogSeverityFormatter();
                static boost::log::formatter getLogChannelSeverityFormatter();
                static boost::log::formatter getLogIdChannelSeverityFormatter();
        private:
                LogFormat();
};

class Logger {
        using logger_sev_channel = boost::log::sources::severity_channel_logger<boost::log::trivial::severity_level, std::string>;
        public:
                static void log(std::string message, std::string channel,boost::log::trivial::severity_level level);

                static Logger& getInstance();
                
                void enableLogging();
                void disableLogging();
                void setFilter(boost::log::filter& filter);
                void resetFilter();

                void enableConsoleLogging();
                void disableConsoleLogging();
                void setConsoleFormatter(boost::log::formatter& formatter);
                void setConsoleFilter(boost::log::filter& filter);
                void resetConsoleFilter();

                void addSink(const std::string& sinkName,boost::shared_ptr<boost::log::sinks::sink> sink);
                boost::log::sinks::sink* getSink(const std::string& sinkName);
                boost::shared_ptr<boost::log::sinks::sink> removeSink(const std::string& sinkName);
                void disableSink(const std::string& sinkName);
                void enableSink(const std::string& sinkName);

        private:
                
                Logger();

                std::map<std::string,boost::shared_ptr<boost::log::sinks::sink>> sinks_;
                std::map<std::string,boost::shared_ptr<boost::log::sinks::sink>> disabledSinks_;

                logger_sev_channel logger_;


                boost::shared_ptr<boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend>> terminal_;
                boost::shared_ptr<boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend>> null_;


};
