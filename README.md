# cppTest
Arbitrage opportunity analytical tool.


How To Run: ./Debug/Test.exe

The main file is located in /Main_Code/Main.cpp


The 2 API's used are bitstamp and coinbase.
They are called through a cURL function.
Packages that need installing: -cURL -cpprestsdk



![Request_A_Quote](https://github.com/Arkhamides/cppTest/blob/main/Screenshots/Stream_Quoting.PNG?raw=true)

The last value in the table indicates the delay of the api call compared to when the first API call arrived. Its kind of a margin of error.




![Request_A_Quote](https://github.com/Arkhamides/cppTest/blob/main/Screenshots/Request_a_quote.PNG?raw=true)

Here, the api calls once per second, until the value changes. when the value changes, the linear interpolation calculates the expected next value.
