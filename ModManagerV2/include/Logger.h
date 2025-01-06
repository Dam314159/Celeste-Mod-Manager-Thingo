#ifndef LOGGER_H
#define LOGGER_H

#include <string>

namespace logger {

void init();

void log(std::string location, std::string text);

}  // namespace logger

#endif