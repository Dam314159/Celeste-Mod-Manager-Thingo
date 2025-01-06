#ifndef LOGGER_H
#define LOGGER_H

#include <string>

namespace logger {

void init();

void log(const std::string &location, const std::string &text);

}  // namespace logger

#endif