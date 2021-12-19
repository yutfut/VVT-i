#pragma once

#include <string>
#include <fstream>
#include <boost/log/sinks.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/core.hpp>
#include <boost/log/sources/global_logger_storage.hpp>

#include "log_defines.h"

namespace bl = boost::log;
namespace kw = bl::keywords;

using backend_type = bl::sinks::text_file_backend;
using sink_type = bl::sinks::synchronous_sink<backend_type>;
using logger_type = bl::sources::severity_logger<bl::trivial::severity_level>;
static logger_type g_logger;

const std::string g_format = "[%TimeStamp%] [%Severity%] %Message%";
BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string);

class Log {
public:
    Log(const std::string &filename, bool flush_flag, bl::trivial::severity_level lvl);

    Log() = default;

    void log(const std::string &s, bl::trivial::severity_level level_message);

private:
    std::string tag;

    bool key_flush;

    bl::trivial::severity_level log_level;
};
