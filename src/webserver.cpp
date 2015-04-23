#include <iostream>

#include "parser/config_parser.h"

using namespace std;

int get_port(NginxConfig *config) {

    // Flag specifying that the next token read will be the port number
    bool next_token_port = false;

    // Number of statements to iterate through
    unsigned long num_statements = config->statements_.size();
    for(unsigned long i = 0; i < num_statements; i++)
    {
        // Statement at the current index
        shared_ptr<NginxConfigStatement> cur_statement;
        cur_statement = config->statements_.at(i);

        // Number of tokens for the current statement to iterate through
        unsigned long num_tokens = cur_statement->tokens_.size();
        for(unsigned long j = 0; j < num_tokens; j++) {
            // Token at the index
            std::string cur_token = cur_statement->tokens_.at(j);
            // Last token read was the port specifier
            if (next_token_port)
                return std::stoi(cur_token);
            // Port string specifier
            if (cur_token == "port")
                next_token_port = true;
        }
    }
    return -1;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: ./webserver <config file>\n");
        return 1;
    }

    // Parse the given configuration file
    NginxConfigParser config_parser;
    NginxConfig config;
    config_parser.Parse(argv[1], &config);

    // Get the port from the parsed configuration file
    int port = get_port(&config);

    printf("Port given is %d", port);

    return 0;
}

