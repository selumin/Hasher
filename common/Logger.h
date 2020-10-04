#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

#ifndef LOG_TAG
#define LOG_TAG ""
#endif

#define LOG_INFO std::cout << "[" << LOG_TAG << "]::"
#define LOG_ERR std::cerr << "[" << LOG_TAG << "]::"

#endif // LOGGER_H
