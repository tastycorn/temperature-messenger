#include <iostream>
#include <curl/curl.h>
#include "curlProwl.h"
#include <fstream>

int main()
{
    std::string apiKey = "d62c5250f10cd07eb19672f9736b7334b1d0356b";
    std::string applicationName = "Elkwater Temperature Monitor";
    std::string descriptionContent = "Message sent to Prowl from Raspberry Pi temperature probe";
    std::string messagePriority = "2";
    
    // Get the raw temperature value from the temperature probe and write it to rawTemp
    std::ifstream inf{"/sys/bus/w1/devices/28-00000a23e435/temperature"};
    int rawTemp;
    inf >> rawTemp;
    
    double degreesC = rawTemp/1000.0;

    std::cout << degreesC;

    return 0;
}