//
// Created by daniel on 24/05/2020.
//

#ifndef HW2_LOGGER_H
#define HW2_LOGGER_H


#include <string>
#include <vector>
#include <fstream>

class Logger {
private:
    std::ofstream log_stream;
    pthread_mutex_t llock;

public:
    explicit Logger(const std::string& filename);
    void writeln(const std::string& line);
    ~Logger();

};


#endif //HW2_LOGGER_H
