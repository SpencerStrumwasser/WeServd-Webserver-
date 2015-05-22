//
// Created by David Pena on 4/26/15.
//

#include "ParserProcessor.h"

// String Signifying value was not found
const std::string NOT_FOUND("NOT FOUND");

/* --- Config file value tokens --- */
const std::string PORT_TOKEN("listen");
const std::string HANDLER_TOKEN("handler");
const std::string STATIC_TOKEN("static");
const std::string ECHO_TOKEN("echo");
const std::string HELLOWORLD_TOKEN("helloworld");
const std::string ROOT_TOKEN("root");

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

strmap *ParserProcessor::get_static_handlers()
{
    strmap *results = new strmap;
    // Get the statements from the config
    statements parser_statements = this->config.statements_;

    // Flag specifying that this is a static handler
    bool token_static_handler = false;
    // Flag specifiying that the next token is for a handler
    bool token_handler = false;

    // Number of statements to iterate through
    unsigned long num_statements = parser_statements.size();
    for(unsigned long i = 0; i < num_statements; i++) {
        // Statement at the current index
        std::shared_ptr<NginxConfigStatement> cur_statement;
        cur_statement = parser_statements.at(i);

        // Number of tokens for the current statement to iterate through
        unsigned long num_tokens = cur_statement->tokens_.size();
        for(unsigned long j = 0; j < num_tokens; j++) {
            // Token at the index
            std::string cur_token = cur_statement->tokens_.at(j);

            // This is indeed a static handler, so the current token
            // must be the web path
            if (token_handler && token_static_handler)
            {
                std::string web_path = cur_token;
                // Root path corresponding to the web path from child block
                statements child_statements;
                child_statements = cur_statement->child_block_->statements_;
                std::string root = value_for_key(child_statements, ROOT_TOKEN);
                // Store to paths
                results->insert({{web_path, root}});
                // Set back flags
                token_handler = false;
                token_static_handler = false;
                continue;
            }

            // Token is a handler
            if (cur_token == HANDLER_TOKEN) {
                token_handler = true;
                continue;
            }

            // Last token read was a handler specifier, check if static handler
            if (cur_token == STATIC_TOKEN && token_handler)
                token_static_handler = true;
                // Handler was not a static handler, no longer considering a handler
            else
                token_handler = false;
        }
    }

    return results;
}

std::vector<std::string> *ParserProcessor::get_echo_handlers()
{
    std::vector<std::string> *results = new std::vector<std::string>();
    // Get the statements from the config
    statements parser_statements = this->config.statements_;
    values_for_key(parser_statements, ECHO_TOKEN, results);
    return results;
}

std::vector<std::string> *ParserProcessor::get_helloworld_handlers()
{
    std::vector<std::string> *results = new std::vector<std::string>();
    // Get the statements from the config
    statements parser_statements = this->config.statements_;
    values_for_key(parser_statements, HELLOWORLD_TOKEN, results);
    return results;
}


/* ----------------------- Private ----------------------- */

/**
 * Gets the value from the statements for the given search key
 */
std::string ParserProcessor::value_for_key(statements parser_statements,
                                           std::string key)
{
    std::vector<std::string> results = std::vector<std::string>();
    values_for_key(parser_statements, key, &results);
    return results[0];
}

/**
 * Gets the values from the statements for the given search key
 */
void ParserProcessor::values_for_key(statements parser_statements,
                                     std::string key,
                                     std::vector<std::string> *results)
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

        // Number of tokens for the current statement to iterate through
        unsigned long num_tokens = cur_statement->tokens_.size();
        for(unsigned long j = 0; j < num_tokens; j++) {
            // Token at the index
            std::string cur_token = cur_statement->tokens_.at(j);
            // Last token read was the key specifier
            if (next_token_key_value) {
                results->push_back(cur_token);
                next_token_key_value = false;
            }
            // Key string specifier
            if (cur_token == key)
                next_token_key_value = true;
        }

        // Check if there are any child statements
        if (cur_statement->child_block_.get() != nullptr) {
            // Get child statements and process them recursively
            statements child_statements;
            child_statements = cur_statement->child_block_->statements_;

            // Try to get the value from the child statements
            values_for_key(child_statements, key, results);
        }
    }
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
            if (this->value_has_prefix(cur_token, prefix)) {
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
bool ParserProcessor::value_has_prefix(std::string value,
                                       std::string prefix)
{
    // Check for prefix match
    auto mismatch = std::mismatch(prefix.begin(), prefix.end(), value.begin());

    if (mismatch.first == prefix.end())
        return true;
    else
        return false;
}
