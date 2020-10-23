#include <iostream>
#include <string>
#include <curl/curl.h>

void sendProwlMessage(std::string apiKey,std::string applicationName,std::string eventContent,std::string descriptionContent,std::string messagePriority);