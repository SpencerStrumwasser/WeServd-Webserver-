//
// Created by David Pena on 4/26/15.
//

#include "ParserProcessor.h"

// String Signifying value was not found
const std::string NOT_FOUND("NOT FOUND");

/* --- Config file value tokens --- */
const std::string PORT_TOKEN("port");
const std::string PATH_TOKEN("location");
const std::string TYPE_TOKEN("server_type");

/* --- Server Type Keywords --- */
const std::string ECHO_TYPE("echo");
const std::string FILE_TYPE("file");

/* ----------------------- Public ----------------------- */

ParserProcessor::ParserProcessor(NginxConfig config) {
    this->config = config;
}

unsigned short ParserProcessor::get_port() {
    // Get the statements from the config
    statements parser_statements = this->config.statements_;

    int port = std::stoi(this->value_for_key(parser_statements, PORT_TOKEN));

    if (port < 0)
        throw ParsedValueError("ERROR: No port given in configuration file");
    else if (port == 0)
        throw ParsedValueError("ERROR: port given cannot be 0.");
    else if (port > 65536)
        throw ParsedValueError("ERROR: Port out of bounds: not in range 1-65536");

    return (unsigned short)port;
}

ServerType::server_type ParserProcessor::get_server_type() {
    // Get the statements from the config
    statements parser_statements = this->config.statements_;
    // Get server type string
    std::string type_str = this->value_for_key(parser_statements, TYPE_TOKEN);

    return this->type_from_string(type_str);
}

strmap *ParserProcessor::get_paths()
{
    strmap *results = new strmap;
    // Get the statements from the config
    statements parser_statements = this->config.statements_;
    return this->values_like_key(parser_statements, PATH_TOKEN, results);
}

/* ----------------------- Private ----------------------- */

/**
 * Gets the value from the statements for the given search key
 */
std::string ParserProcessor::value_for_key(statements parser_statements, std::string key)
{
    // Flag specifying that the next token read will be the key value
    bool next_token_key_value = false;

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

            // Try to get the value from the child statements, if found return it
            std::string try_key = this->value_for_key(child_statements, key);
            if (try_key != NOT_FOUND)
                return try_key;
        }

        // Number of tokens for the current statement to iterate through
        unsigned long num_tokens = cur_statement->tokens_.size();
        for(unsigned long j = 0; j < num_tokens; j++) {
            // Token at the index
            std::string cur_token = cur_statement->tokens_.at(j);
            // Last token read was the key specifier
            if (next_token_key_value)
                return cur_token;
            // Key string specifier
            if (cur_token == key)
                next_token_key_value = true;
        }
    }
    return NOT_FOUND;
}

/**
 * Returns a map with the keys that start with the given search key, and
 * the values for those keys.
 */
strmap *ParserProcessor::values_like_key(statements parser_statements,
                                         std::string prefix,
                                         strmap *results)
{
    // Flag specifying that the next token read will be the key value
    bool next_token_key_value = false;
    std::string prev_token_key;

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

            // Try to get the values from the child statements
            this->values_like_key(child_statements, prefix, results);
        }

        // Number of tokens for the current statement to iterate through
        unsigned long num_tokens = cur_statement->tokens_.size();
        for(unsigned long j = 0; j < num_tokens; j++) {
            // Token at the index
            std::string cur_token = cur_statement->tokens_.at(j);
            // Last token read was the key specifier
            if (next_token_key_value) {
                // Store the found value
                results->insert({{prev_token_key, cur_token}});
                // Clear the values
                next_token_key_value = false;
                prev_token_key = "";
            }

            // Key string specifier
            if (this->token_has_prefix(cur_token, prefix)) {
                next_token_key_value = true;
                prev_token_key = cur_token;
            }
        }
    }

    // No tokens found
    if (results->empty()) {
        delete results;
        return NULL;
    }
    else
        return results;

}
/* ------------- Helper Functions ------------- */
/**
 * Check if the token has the given prefix string
 */
bool ParserProcessor::token_has_prefix(std::string token, std::string prefix)
{
    // Check for prefix match
    auto mismatch = std::mismatch(prefix.begin(), prefix.end(), token.begin());
    if (mismatch.first == prefix.end())
        return true;
    else
        return false;
}

ServerType::server_type ParserProcessor::type_from_string(std::string str)
{
    using namespace ServerType;

    if (str == FILE_TYPE)
        return file_server;
    else if (str == ECHO_TYPE)
        return echo_server;
    else
        throw ParsedValueError("Invalid server type");
}


