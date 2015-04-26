//
// Created by David Pena on 4/26/15.
//

#include "ParserProcessor.h"

ParserProcessor::ParserProcessor(NginxConfig config) {
    this->config = config;
}

/**
 * Gets the value from the statements for the given search key
 */
int ParserProcessor::value_for_key(statements parser_statements, std::string key)
{
    // Flag specifying that the next token read will be the port number
    bool next_token_port = false;

    // Number of statements to iterate through
    unsigned long num_statements = parser_statements.size();
    for(unsigned long i = 0; i < num_statements; i++)
    {
        // Statement at the current index
        std::shared_ptr<NginxConfigStatement> cur_statement;
        cur_statement = parser_statements.at(i);

        // Check if there are any child statements
        if (cur_statement->child_block_.get() != nullptr) {
            // Get child statements and process them recursively
            statements child_statements;
            child_statements = cur_statement->child_block_->statements_;

            // Try to get the port from the child statements, if found return it
            int try_port = this->value_for_key(child_statements, key);
            if (try_port != -1)
                return try_port;
        }

        // Number of tokens for the current statement to iterate through
        unsigned long num_tokens = cur_statement->tokens_.size();
        for(unsigned long j = 0; j < num_tokens; j++) {
            // Token at the index
            std::string cur_token = cur_statement->tokens_.at(j);
            // Last token read was the port specifier
            if (next_token_port)
                return std::stoi(cur_token);
            // Port string specifier
            if (cur_token == key)
                next_token_port = true;
        }
    }
    return -1;
}

unsigned short ParserProcessor::get_port() {
    // Get the statements from the config
    statements parser_statements = this->config.statements_;

    int port = this->value_for_key(parser_statements, "port");

    if (port < 0)
        throw ParsedValueError("ERROR: No port given in configuration file");
    else if (port == 0)
        throw ParsedValueError("ERROR: port given cannot be 0.");
    else if (port > 65536)
        throw ParsedValueError("ERROR: Port out of bounds: not in range 1-65536");

    return (unsigned short)port;
}
