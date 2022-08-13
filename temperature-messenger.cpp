#include <iostream>
#include <curl/curl.h>
#include <json/json.h>
#include "curlProwl.h"
#include <fstream>
#include <string.h>

namespace
{
    std::size_t callback(
            const char* in,
            std::size_t size,
            std::size_t num,
            std::string* out)
    {
        const std::size_t totalBytes(size * num);
        out->append(in, totalBytes);
        return totalBytes;
    }
}

int main()
{
    std::string apiKey = "ajr1weuitju1qb1nfu1pv27qbawyxz";
    std::string userKey = "uhzz5j27magashnwpky9grvf3yuz3h";
    //std::string applicationName = "Elkwater+Temperature+Monitor+";
    //std::string eventContent = "+Update";
    //std::string messagePriority = "2";
    
    const std::string url("http://10.0.1.101/sensors");

    CURL* curl = curl_easy_init();

    // Set remote URL.
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // Don't bother trying IPv6, which would increase DNS resolution time.
    curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);

    // Don't wait forever, time out after 10 seconds.
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);

    // Follow HTTP redirects if necessary.
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    // Response information.
    int httpCode(0);
    std::unique_ptr<std::string> httpData(new std::string());

    // Hook up data handling function.
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);

    // Hook up data container (will be passed as the last parameter to the
    // callback handling function).  Can be any pointer type, since it will
    // internally be passed as a void pointer.
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());

    // Run our HTTP GET command, capture the HTTP response code, and clean up.
    curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    curl_easy_cleanup(curl);

    if (httpCode == 200)
    {
        std::cout << "\nGot successful response from " << url << std::endl;

        // Response looks good - done using Curl now.  Try to parse the results
        // and print them out.
        Json::Value jsonData;
        Json::Reader jsonReader;

        if (jsonReader.parse(*httpData, jsonData))
        {
            std::cout << "Successfully parsed JSON data" << std::endl;
            std::cout << "\nJSON data received:" << std::endl;
            std::cout << jsonData.toStyledString() << std::endl;

            const std::string tempString(jsonData["temperature"].asString());
//            const std::size_t unixTimeMs(jsonData["milliseconds_since_epoch"].asUInt64());
//            const std::string timeString(jsonData["time"].asString());

            std::cout << "Natively parsed:" << std::endl;
            std::cout << "\tTemperature string: " << tempString << std::endl;
//            std::cout << "\tUnix timeMs: " << unixTimeMs << std::endl;
//            std::cout << "\tTime string: " << timeString << std::endl;
            std::cout << std::endl;
        }
        else
        {
            std::cout << "Could not parse HTTP data as JSON" << std::endl;
            std::cout << "HTTP data was:\n" << *httpData.get() << std::endl;
            return 1;
        }
    }
    else
    {
        std::cout << "Couldn't GET from " << url << " - exiting" << std::endl;
        return 1;
    }
    
 /*
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
*/
    return 0;
}
