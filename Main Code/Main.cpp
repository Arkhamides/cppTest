// ConsoleApplication3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <map>
#include <set>
#include <string>

#include <cpprest/http_client.h>
#include <cpprest/http_listener.h>
#include <cpprest/filestream.h>
#include <curl/curl.h>

#include <cpprest/json.h>

#include <chrono>
#include <stdio.h>
#include <time.h>
#include <ctime>

using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace std;






const wstring consumerKey = L"tb0bWLoWfXOU071Q";
const wstring consumerSecret = L"s6eSuNW7UheM4aI7KBkjeeKZ1bDRbafU";



static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

static void GetCurrentDate() {
    struct tm newtime;
    char am_pm[] = "AM";
    __time64_t long_time;
    char timebuf[26];
    errno_t err;

    // Get time as 64-bit integer.
    _time64(&long_time);
    // Convert to local time.
    err = _localtime64_s(&newtime, &long_time);
    if (err)
    {
        printf("Invalid argument to _localtime64_s.");
        exit(1);
    }
    if (newtime.tm_hour > 12)        // Set up extension.
        strcpy_s(am_pm, sizeof(am_pm), "PM");
    if (newtime.tm_hour > 12)        // Convert from 24-hour
        newtime.tm_hour -= 12;        // to 12-hour clock.
    if (newtime.tm_hour == 0)        // Set hour to 12 if midnight.
        newtime.tm_hour = 12;

    // Convert to an ASCII representation.
    err = asctime_s(timebuf, 26, &newtime);
    if (err)
    {
        printf("Invalid argument to asctime_s.");
        exit(1);
    }
    printf("%.19s %s", timebuf, am_pm);

}

static char GetSeconds() {
    struct tm newtime;
    char am_pm[] = "AM";
    __time64_t long_time;
    char timebuf[26];
    errno_t err;

    // Get time as 64-bit integer.
    _time64(&long_time);
    // Convert to local time.
    err = _localtime64_s(&newtime, &long_time);
    if (err)
    {
        printf("Invalid argument to _localtime64_s.");
        exit(1);
    }
    if (newtime.tm_hour > 12)        // Set up extension.
        strcpy_s(am_pm, sizeof(am_pm), "PM");
    if (newtime.tm_hour > 12)        // Convert from 24-hour
        newtime.tm_hour -= 12;        // to 12-hour clock.
    if (newtime.tm_hour == 0)        // Set hour to 12 if midnight.
        newtime.tm_hour = 12;

    // Convert to an ASCII representation.
    err = asctime_s(timebuf, 26, &newtime);
    if (err)
    {
        printf("Invalid argument to asctime_s.");
        exit(1);
    }
    return timebuf[18];
}

 
static string* Curl_ExchangeRates_BTC_USD_bitstamp() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {


        //curl -H "X-CMC_PRO_API_KEY: 4ae49c0a-1c32-438c-896b-0e3b9de46097" -H "Accept: application/json" -G https://pro-api.coinmarketcap.com/v1/exchange/quotes/latest

        //sets up API call
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.bitstamp.net/api/v2/ticker/btcusd/");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);


        //get bid
        size_t pos = readBuffer.find("bid");
        string bid = readBuffer.substr(pos + 7, 8);
        bid = bid.substr(0, bid.find(",")-1);
        double d_bid = stod(bid);

        //get ask
        pos = readBuffer.find("ask");
        string ask = readBuffer.substr(pos + 7, 8);
        ask = ask.substr(0, ask.find(",") - 1);
        double d_ask = stod(ask);

        // Prints out Row
        GetCurrentDate();
        cout << "     Bitstamp      ";
        cout << "   bid       " << bid ;
        cout << "      " << d_bid/1000;
        cout << "      " << d_bid * 5;
        cout << "      " << d_bid * 10;
        cout << "     " <<  "0 ms." << endl;

        GetCurrentDate();
        cout << "     Bitstamp      ";
        cout << "   Offer     " << ask ;
        cout << "      " << d_ask / 1000;
        cout << "      " << d_ask * 5;
        cout << "      " << d_ask * 10;
        cout << "     " << "0 ms." << endl;

        string* o = new string[2];
        o[0] = bid;
        o[1] = ask;

        return o;
    }
}

static string Curl_ExchangeRates_BTC_USD_Coinbase_buy() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {

        //curl https://api.coinbase.com/v2/prices/BTC-USD/buy -H "CB-ACCESS-KEY: tb0bWLoWfXOU071Q"
        //curl https://api.coinbase.com/v2/time

        curl_easy_setopt(curl, CURLOPT_URL, "https://api.coinbase.com/v2/prices/BTC-USD/buy");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        size_t pos = readBuffer.find("amount");
        string bid = readBuffer.substr(pos + 9, 8);
        bid = bid.substr(0, bid.find(",") - 1);
        double d_bid = stod(bid);


        GetCurrentDate();
        cout << "     CoinBase    ";
        cout << "     bid       " << bid ;
        cout << "      " << d_bid / 1000;
        cout << "      " << d_bid * 5;
        cout << "      " << d_bid * 10;
        return bid;
    }
}

static string Curl_ExchangeRates_BTC_USD_Coinbase_Sell() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {


        //curl https://api.coinbase.com/v2/prices/BTC-USD/sell -H "CB-ACCESS-KEY: tb0bWLoWfXOU071Q"
        //curl https://api.coinbase.com/v2/time


        curl_easy_setopt(curl, CURLOPT_URL, "https://api.coinbase.com/v2/prices/BTC-USD/sell");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);


        size_t pos = readBuffer.find("amount");
        string offer = readBuffer.substr(pos + 9, 8);
        offer = offer.substr(0, offer.find(",") - 1);
        double d_offer = stod(offer);

        GetCurrentDate();
        cout << "     CoinBase    ";
        cout << "     offer     " << offer;
        cout << "      " << d_offer / 1000;
        cout << "      " << d_offer * 5;
        cout << "      " << d_offer * 10;
        return offer;
    }
}


static void CurlTest() {
    CURLcode res;
    std::string readBuffer;
    struct curl_slist* list = NULL;

    CURL* curl = curl_easy_init();

    

    if (curl) {


        list = curl_slist_append(list, "Shoesize: 10");
        list = curl_slist_append(list, "Accept:");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);


        curl_easy_setopt(curl, CURLOPT_URL, "https://pro-api.coinmarketcap.com/v1/cryptocurrency/map");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);


        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl_slist_free_all(list); /* free the list again */

        //curl -H "X-CMC_PRO_API_KEY: 4ae49c0a-1c32-438c-896b-0e3b9de46097" -H "Accept: application/json" -G https://pro-api.coinmarketcap.com/v1/cryptocurrency/map


        std::cout << readBuffer << std::endl;
    }
}


static void getRows() {

    ////////////////////////////////////////  Get Exact Time Between processes ////////////////////////////////////////
    using namespace std::chrono;
    
    Curl_ExchangeRates_BTC_USD_bitstamp();

    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    Curl_ExchangeRates_BTC_USD_Coinbase_buy();
    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    duration<double, std::milli> time_span = t2 - t1;

    std::cout << "     " << time_span.count() << " ms.";
    std::cout << std::endl;

    Curl_ExchangeRates_BTC_USD_Coinbase_Sell();
    t2 = high_resolution_clock::now();

    time_span = t2 - t1;
    std::cout << "     " << time_span.count() << " ms.";
    std::cout << std::endl;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////


}

static void header() {
    cout << "          TIME          " << "|  Provider    " << "|   side    " << "|   Price   " << "|  QTY:0.001  " 
        << "|   QTY:5  " <<"|   QTY:10  " << endl;
}

static void linearInterpolateBuy(int qty) {
    bool ready = false;
    string y[2];

    header();


    double x2 = 0.0;
    char seconds = GetSeconds();
    y[0] = Curl_ExchangeRates_BTC_USD_Coinbase_buy();
    cout << endl;

    while (!ready) {

        if (seconds != GetSeconds()) {

            seconds = GetSeconds();
            y[1] = Curl_ExchangeRates_BTC_USD_Coinbase_buy();
            cout << endl;
            x2++;
            if (y[1] != y[0]) {
                ready = true;
            }

        }
    }

    double y1 = stod(y[0]) * qty;
    double y2 = stod(y[1]) * qty;

    cout << "Calculating price with linear interpolation" << endl << endl;
    cout << "Quantity: " << qty << endl;
    cout << "y1 = " << y1 << endl;
    cout << "y2 = " << y2 << endl;
    cout << "x1 = 0" << endl;
    cout << "x2 =" << x2 << endl;

    double expectedy_y = y1 + ((2 * x2) / (x2)) * (y2 - y1);

    cout << "Expected Y= " << expectedy_y << " at x = " << 2 * x2;
    //y = y1 + ((x – x1) / (x2 – x1)) * (y2 - y1)
}

static void linearInterpolateSell(int qty) {
    bool ready = false;
    string y[2];

    header();


    double x2 = 0.0;
    char seconds = GetSeconds();
    y[0] = Curl_ExchangeRates_BTC_USD_Coinbase_Sell();
    cout << endl;

    while (!ready) {

        if (seconds != GetSeconds()) {

            seconds = GetSeconds();
            y[1] = Curl_ExchangeRates_BTC_USD_Coinbase_Sell();
            cout << endl;
            x2++;
            if (y[1] != y[0]) {
                ready = true;
            }

        }
    }

    double y1 = stod(y[0]) * qty;
    double y2 = stod(y[1]) * qty;

    cout << "Calculating price with linear interpolation" << endl << endl;
    cout << "Quantity: " << qty << endl;
    cout << "y1 = " << y1 << endl;
    cout << "y2 = " << y2 << endl;
    cout << "x1 = 0" << endl;
    cout << "x2 =" << x2 << endl;

    double expectedy_y = y1 + ((2 * x2) / (x2)) * (y2 - y1);

    cout << "Expected Y= " << expectedy_y << " at x = " << 2 * x2;
    //y = y1 + ((x – x1) / (x2 – x1)) * (y2 - y1)
}


int main()
{
    char seconds = GetSeconds();

    cout << "Press: " << endl << "1 - Stream Quoting" << endl << "2 - Request for a quote.\n";
    string input;
    cin >> input;

    if (input == "1") {
        // Stream quoting limited to 10 times, so it won't loop forever.

        int loops = 10;

        header();
        while (loops > 0) {

            if (seconds != GetSeconds()) {
                getRows();
                loops--;
                seconds = GetSeconds();
            }
        }
    }

    if (input == "2") {
        //Request a quote
        cout << "Request Side: \n";
        cout << "1) bid \n";
        cout << "2) offer \n";

        cin >> input;

        int quantity;
        cout << "Request Quantity: \n";
        cin >> quantity;

        if (input == "1") {

            linearInterpolateBuy(quantity);
        }

        if (input == "2") {
            linearInterpolateSell(quantity);
        }

    }


    


    
}