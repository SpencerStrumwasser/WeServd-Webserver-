// file_server.cpp
// serves static content. must be either html or jpeg

#include "FileServer.hpp"

std::string FileServer::run(std::string request, std::string *URLpaths) {
	std::string filename = get_file_name(request);
	return filename;
}

bool FileServer::does_file_exist(std::string filepath)
{
    std::ifstream infile(filepath);
    infile.close();
    return infile.good();
}

std::string FileServer::return_file(std::string filename, std::string *URLpaths) {
	for (int i = 0; i < URLpaths->size(); i++) {
		std::string full_path = URLpaths[i] + filename;
		if (does_file_exist(full_path)) {
			return full_path;
		}
	}
	return "false";
}

std::string FileServer::get_file_name(std::string request) {
	return request.substr(request.find("/static") + strlen("/static"), request.find(" HTTP") - strlen(" HTTP/1.1 \r"));
}


