#include <iostream>

#include "parser/config_parser.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: ./webserver <config file>\n");
        return 1;
    }

    NginxConfigParser config_parser;
    NginxConfig config;
    config_parser.Parse(argv[1], &config);

    printf("%s", config.ToString().c_str());
    return 0;
}
