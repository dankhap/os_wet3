//
// Created by daniel on 24/05/2020.
//

#include "logger.h"

using std::endl;

Logger::Logger(const std::string& filename) {
    log_stream.exceptions(std::ios::failbit|std::ios::badbit); // if there is a problem with stream throw exception.
    log_stream.open(filename, std::ofstream::app);
    pthread_mutex_init(&llock, nullptr);
}

void Logger::writeln(const std::string& line) {
//Lock L(llock);
    log_stream << line << endl;
}

Logger::~Logger() {
    pthread_mutex_destroy(&llock);
    log_stream.close();
}
