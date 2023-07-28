#pragma once
#include <curl\curl.h>
#include <nlohmann\json.hpp>

class JSONParser
{
protected:
	static size_t Writer(char* buffer, size_t size, size_t nmemb, std::string* html);
	std::string Get_request(std::string link);
};