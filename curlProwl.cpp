#include <iostream>
#include <string>
#include <curl/curl.h>

void sendProwlMessage(std::string apiKey,std::string applicationName,std::string eventContent,std::string descriptionContent,std::string messagePriority)
{
    CURL *curl;
    CURLcode result;
    std::string theURL="https://api.prowlapp.com/publicapi/add?";
    theURL.append("apikey=");
    theURL.append(apiKey);
    theURL.append("&application=");
    theURL.append(applicationName);
    theURL.append("&event=");
    theURL.append(eventContent);
    theURL.append("&description=");
    theURL.append(descriptionContent);
    theURL.append("&priority=");
    theURL.append(messagePriority);

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl,CURLOPT_URL,theURL);
        result = curl_easy_perform(curl);
        if (result != CURLE_OK)
        {
            std::cerr << "Error during curl request: " << curl_easy_strerror(result) << std::endl;
        }
        curl_easy_cleanup(curl);
    }
    else
    {
        std::cerr << "Error handling curl." << std::endl;
    }
    curl_global_cleanup();
}