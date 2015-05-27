
#include "NotFoundHandler.h"
#include <fstream>
#include <string>
#include <vector>

// With many thanks to DreamTeamMinusSpencer!

void NotFoundHandler::Configure(const NginxConfig& child_config_block) {
  return;
}

std::string NotFoundHandler::HandleRequest(const HTTPRequest& req) {
    std::string status = status_strings::not_found;

    std::string content = "<html>"
                          "<head>"
                            "<title>404: Not Found</title>"
                          "</head>"
                          "<body>"
                            "<div style=\"margin: 0 auto; width: 50%%; text-align: center;\">"
                            "There may come a day when you can load this file.<br/>"
                            "<img src=\"static/notthisday.jpeg\" />"
                            "</div>"
                          "</body>"
                          "</html>";

    std::vector<std::pair<std::string, std::string>> headers;
    headers.push_back({ "Content-Length", std::to_string(content.size()) });
    headers.push_back({ "Content-Type", "text/html" });


    std::string response = status;
    // Loop over headers and concatenate them
    for(unsigned int i = 0; i < headers.size(); i++) {
        response = response + headers[i].first + ": " + headers[i].second + "\r\n";
    }
    response += "\r\n" + content;

    return response;
}