#include "JSONParser.h"

 size_t JSONParser::Writer(char* buffer, size_t size, size_t nmemb, std::string* html)
{
    int result = 0;
    if (buffer) {
        html->append(buffer, size * nmemb);
        result = size * nmemb;
    }
    return result;

}
std::string JSONParser:: Get_request(std::string link)
{
    CURL* curl;
    std::string data;
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, link.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return data;
}