#include <iostream>
#include <curl/curl.h>
#include "curlProwl.h"
#include <fstream>
#include <string.h>

int main()
{
    std::string apiKey = "ajr1weuitju1qb1nfu1pv27qbawyxz";
    std::string userKey = "uhzz5j27magashnwpky9grvf3yuz3h";
    //std::string applicationName = "Elkwater+Temperature+Monitor+";
    //std::string eventContent = "+Update";
    //std::string messagePriority = "2";
    
    // Get the raw temperature value from the temperature probe and write it to rawTemp
    std::ifstream inf{"/sys/bus/w1/devices/28-00000a23e435/temperature"};
    int rawTemp;
    inf >> rawTemp;
    
    double degreesC = rawTemp/1000.0;

    std::string descriptionContent = "%0AThe+current+temperature+is+";
    descriptionContent.append(std::to_string(degreesC),0,4);
    descriptionContent.append("\u00B0");
    // descriptionContent.append("degrees");
    descriptionContent.append("C.");

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

    std::cout << theURL;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl,CURLOPT_URL,theURL.c_str());
        //curl_easy_setopt(curl,CURLOPT_URL,"https://api.prowlapp.com/publicapi/add?apikey=d62c5250f10cd07eb19672f9736b7334b1d0356b&application=test&event=Temperature+Probe&description=test&priority=1");
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

    // sendProwlMessage(apiKey,applicationName,eventContent,descriptionContent,messagePriority);

    return 0;
}
