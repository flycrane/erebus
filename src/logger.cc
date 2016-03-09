#include "logger.h"

#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/core/core.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/expressions/attr.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/attributes/timer.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>
#include <boost/log/expressions/filter.hpp>

#include <string>
#include <iostream>
#include <iomanip>
#include <map>

BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", boost::log::trivial::severity_level)
BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)
BOOST_LOG_ATTRIBUTE_KEYWORD(uptime, "Uptime", boost::log::attributes::timer::value_type)

const std::string LogChannel::DEFAULT="default";

boost::log::formatter LogFormat::getLogEverythingFormatter() {
        static boost::log::formatter format = 
                boost::log::expressions::stream
                    << std::setw(7) << std::setfill('0') << line_id << " | " 
                    << boost::log::expressions::format_date_time(uptime, "%H:%M:%S")
                    << " (" << boost::log::expressions::attr<std::string>("Channel") << ")"
                    << " [" << boost::log::trivial::severity<<"]"
                    << " " << boost::log::expressions::smessage;

        return format;
}

boost::log::formatter LogFormat::getLogChannelFormatter() {
        static boost::log::formatter format =
                boost::log::expressions::stream
                    << " (" << boost::log::expressions::attr<std::string>("Channel") << ")"
                    << " " << boost::log::expressions::smessage;
        
        return format;
}

boost::log::formatter LogFormat::getLogSeverityFormatter() {
        static boost::log::formatter format =
                boost::log::expressions::stream
                    << " [" << boost::log::trivial::severity<<"]"
                    << " " << boost::log::expressions::smessage;

        return format;
}

boost::log::formatter LogFormat::getLogChannelSeverityFormatter() {
        static boost::log::formatter format =
                boost::log::expressions::stream
                    << " (" << boost::log::expressions::attr<std::string>("Channel") << ")"
                    << " [" << boost::log::trivial::severity<<"]"
                    << " " << boost::log::expressions::smessage;

        return format;
}

boost::log::formatter LogFormat::getLogIdChannelSeverityFormatter() {
        static boost::log::formatter format = 
                boost::log::expressions::stream
                    << std::setw(7) << std::setfill('0') << line_id << " | " 
                    << " (" << boost::log::expressions::attr<std::string>("Channel") << ")"
                    << " [" << boost::log::trivial::severity<<"]"
                    << " " << boost::log::expressions::smessage;

        return format;
}

void Logger::log(std::string message, std::string channel,boost::log::trivial::severity_level level) {
        BOOST_LOG_CHANNEL_SEV(getInstance().logger_,channel,level)<<message;
}

Logger& Logger::getInstance() {
        static Logger logger;

        return logger;
}

Logger::Logger() {
        auto core = boost::log::core::get();

        boost::log::add_common_attributes();
        core->add_global_attribute("LineID",boost::log::attributes::counter<unsigned int>(1));
        core->add_global_attribute("Uptime" ,boost::log::attributes::timer());
        
        
        auto nullBackend = boost::make_shared<boost::log::sinks::text_ostream_backend>();
        nullBackend->add_stream(boost::shared_ptr<std::ostream>(&std::clog ,boost::null_deleter()));
        
        null_ = boost::make_shared<boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend>>(nullBackend);
        null_->set_filter(severity > boost::log::trivial::fatal);

        core->add_sink(null_);
        
        auto terminalBackend = boost::make_shared<boost::log::sinks::text_ostream_backend>();
        terminalBackend->add_stream(boost::shared_ptr<std::ostream>(&std::clog, boost::null_deleter()));
        terminalBackend->auto_flush(true);
        
        terminal_ = boost::make_shared<boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend>>(terminalBackend);
        terminal_->set_formatter(LogFormat::getLogEverythingFormatter());

}

void Logger::enableConsoleLogging() {
        boost::log::core::get()->add_sink(terminal_);
}

void Logger::disableConsoleLogging() {
        boost::log::core::get()->remove_sink(terminal_);
}

void Logger::enableLogging() {
        boost::log::core::get()->set_logging_enabled(true);
}

void Logger::disableLogging() {
        boost::log::core::get()->set_logging_enabled(false);
}

void Logger::setFilter(boost::log::filter& filter) {
        boost::log::core::get()->set_filter(filter);
}

void Logger::resetFilter() {
        boost::log::core::get()->reset_filter();
}

void Logger::setConsoleFormatter(boost::log::formatter& formatter) {
        terminal_->set_formatter(formatter);
}

void Logger::setConsoleFilter(boost::log::filter& filter) {
        terminal_->set_filter(filter);
}

void Logger::resetConsoleFilter() {
        terminal_->reset_filter();
}

void Logger::addSink(const std::string& sinkName,boost::shared_ptr<boost::log::sinks::sink> sink) {
        if(sinks_.find(sinkName) != sinks_.end())
                return;
        if(disabledSinks_.find(sinkName) != disabledSinks_.end())
                return;

        sinks_[sinkName]=sink;
        boost::log::core::get()->add_sink(sink);
}

boost::log::sinks::sink* Logger::getSink(const std::string& sinkName) {
        auto value=sinks_.find(sinkName);
        if(value==sinks_.end())
                return nullptr;
        return value->second.get();
}

boost::shared_ptr<boost::log::sinks::sink> Logger::removeSink(const std::string& sinkName) {
    auto value=sinks_.find(sinkName);
    if(value==sinks_.end())
            return boost::shared_ptr<boost::log::sinks::sink>();
    auto sink=value->second;
    sinks_.erase(sinkName);
    boost::log::core::get()->remove_sink(sink);

    return sink;
}

void Logger::disableSink(const std::string& sinkName) {
       auto value=sinks_.find(sinkName);
       if(value==sinks_.end())
               return;
       auto sink=value->second;
       sinks_.erase(sinkName);
       boost::log::core::get()->remove_sink(sink);
       disabledSinks_[sinkName]=sink;
}

void Logger::enableSink(const std::string& sinkName) {
        auto value=disabledSinks_.find(sinkName);
        if(value==disabledSinks_.end())
                return;
        auto sink=value->second;
        disabledSinks_.erase(sinkName);
        boost::log::core::get()->add_sink(sink);
        sinks_[sinkName]=sink;
}
