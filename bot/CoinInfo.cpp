#include "CoinInfo.h"
std::string CoinInfo::GetCoinInfo(std::string ticker)
{
    if (ticker[0] == '/')
    {
        ticker.erase(0,1);
    }
    boost::to_upper(ticker);

    try
    {
        auto js_obj = nlohmann::json::parse(Get_request("https://api.binance.com/api/v3/ticker/24hr?symbol=" + ticker + "USDT"));

        std::string price = js_obj["lastPrice"].get<std::string>();
        std::string lowPrice = js_obj["lowPrice"].get<std::string>();
        std::string highPrice = js_obj["highPrice"].get<std::string>();
        std::string priceChangePercent = js_obj["priceChangePercent"].get<std::string>();
        for (int i = price.size() - 1; price[i] == '0'; i--)
        {
            price.pop_back();
        }
        for (int i = lowPrice.size() - 1; lowPrice[i] == '0'; i--)
        {
            lowPrice.pop_back();
        }
        for (int i = highPrice.size() - 1; highPrice[i] == '0'; i--)
        {
            highPrice.pop_back();
        }
        std::string substr = "up";
        if (priceChangePercent[0] == '-') { substr = "down"; priceChangePercent.erase(0, 1);
        }

        return "The " + ticker + " price is " + price + " usdt now.\n"+"It's "+ substr+" "+ priceChangePercent + "% last 24h\nMax price: "+ highPrice+"$\nLow price: "+ lowPrice+"$";
        


    }
    catch (...)
    {
        return "please enter correct ticker";
    }

}