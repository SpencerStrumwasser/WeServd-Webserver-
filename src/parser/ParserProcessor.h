//
// Created by David Pena on 4/26/15.
//

#ifndef WESERVD_PARSERPROCESSOR_H
#define WESERVD_PARSERPROCESSOR_H

#include <exception>
#include <unordered_map>
#include "ConfigParser.h"

typedef std::vector<std::shared_ptr<NginxConfigStatement>> statements;

typedef std::unordered_map<std::string, std::string> strmap;

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
     * Gets the port from the parsed configuration
     */
    unsigned short get_port();
    /**
     * Gets the paths from the parsed configuration
     */
    strmap *get_paths();

private:
    std::string value_for_key(statements parser_statements, std::string key);
    strmap *values_like_key(statements parser_statements, std::string prefix,
                            strmap *results);
    bool token_has_prefix(std::string token, std::string prefix);
    NginxConfig config;

};


#endif //WESERVD_PARSERPROCESSOR_H
