//
// Created by David Pena on 4/26/15.
//

#ifndef WESERVD_PARSERPROCESSOR_H
#define WESERVD_PARSERPROCESSOR_H

#include <exception>
#include "ConfigParser.h"

typedef std::vector<std::shared_ptr<NginxConfigStatement>> statements;

class ParsedValueError: public std::exception {
private:
    std::string message_;
public:
    ParsedValueError(const std::string& message) {
        this->message_ = message;
    }
    virtual const char* what() const throw() {
        return message_.c_str();
    }
};

class ParserProcessor {
public:
    ParserProcessor(NginxConfig config);
    /**
     * Gets the port from the given parsed configuration
     */
    unsigned short get_port();

private:
    int value_for_key(statements parser_statements, std::string key);
    NginxConfig config;
};


#endif //WESERVD_PARSERPROCESSOR_H
