//
// Created by David Pena on 4/26/15.
//

#ifndef WESERVD_PARSERPROCESSOR_H
#define WESERVD_PARSERPROCESSOR_H

#include <exception>
#include <unordered_map>
#include <vector>
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
    /** Constructor */
    ParserProcessor(NginxConfig config);

    /** Gets the port from the parsed configuration */
    unsigned short get_port();

    /** Gets the static handlers from the parsed configuration */
    strmap *get_static_handlers();
    /** Gets the echo handlers from the parsed configuration */
    std::vector<std::string> *get_echo_handlers();
    /** Gets the hello world handlers from the parsed configuration */
    std::vector<std::string> *get_helloworld_handlers();

    /**
     * Check whether the value has the given prefix
     */
    static bool value_has_prefix(std::string value, std::string prefix);

private:
    NginxConfig config;

    std::string value_for_key(statements parser_statements, std::string key);
    void values_for_key(statements parser_statements, std::string key,
                        std::vector<std::string> *results);
    strmap *values_like_key(statements parser_statements, std::string prefix,
                            strmap *results);
};


#endif //WESERVD_PARSERPROCESSOR_H
