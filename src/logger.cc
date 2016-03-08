#include "logger.h"

#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/core/core.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/expressions/attr.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>

#include <string>
#include <iostream>
#include <iomanip>

BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", boost::log::trivial::severity_level)
BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)
BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", boost::posix_time::ptime)


/*namespace {
        std::string LogLevelToString(LogLevel l) {
                switch(l) {
                        case LogLevel::DEBUG:
                                return "debug";
                        default:
                                return "invalid";
                }
        }
}*/

const std::string LogChannel::DEFAULT="default";

void Logger::log(std::string message, std::string channel, LogLevel l) {
        getInstance();
        static logger_sev_channel logger(boost::log::keywords::channel = "TEST");
        //std::cout<<message<<" ["<<channel<<"] "<<LogLevelToString(l)<<std::endl;
        BOOST_LOG_SEV(logger,boost::log::trivial::trace)<<message;
}

Logger& Logger::getInstance() {
        static Logger logger;

        return logger;
}

Logger::Logger() {
        auto core = boost::log::core::get();

        boost::log::add_common_attributes();
        core->add_global_attribute("LineID",boost::log::attributes::counter<unsigned int>(1));
        core->add_global_attribute("TimeStamp" ,boost::log::attributes::local_clock());
        //boost::log::register_simple_formatter_factory< boost::log::trivial::severity_level, char >("Severity");
        logEverything_ = boost::log::expressions::stream
                << std::setw(7) << std::setfill('0')
                << line_id 
                << " | " << boost::log::expressions::format_date_time(
                                timestamp,
                                "%Y-%m-%d, %H:%M:%S")
                << " [" << boost::log::trivial::severity<<"]"
                << " " << boost::log::expressions::smessage;

        auto terminalBackend = boost::make_shared<boost::log::sinks::text_ostream_backend>();
        terminalBackend->add_stream(boost::shared_ptr<std::ostream>(&std::clog, boost::null_deleter()));
        terminalBackend->auto_flush(true);
        
        terminal_ = boost::make_shared<boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend>>(terminalBackend);
        terminal_->set_formatter(logEverything_);

        core->add_sink(terminal_);

}

void Logger::enableConsoleLogging() {

}

void Logger::disableTerminalLogging() {

}

std::ostream& operator<<(std::ostream& strm, LogLevel l) {
    strm<<"test";

    return strm;
}
