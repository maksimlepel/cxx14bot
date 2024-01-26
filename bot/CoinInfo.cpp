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
bool CoinInfo::getChart(std::string ticker, int userId)
{

        boost::to_upper(ticker);
        cpr::Response r = cpr::Post(cpr::Url{"https://api.chart-img.com/v2/tradingview/advanced-chart"},
        cpr::Header{{"x-api-key", "n61GiZUCj26uvpfJ72J0d4J6UMgNzkE7OvAddO14"}, { "content-type","application/json" }},
        cpr::Body{"{\"symbol\":\"BINANCE:" + ticker + "USDT\",\"interval\":\"4h\",\"theme\":\"dark\"}"});//,\"theme\":\"dark\"
        std::cout << r.status_code << std::endl;
        std::cout << r.header["content-type"] << std::endl;
        if (r.status_code != 200)
        {
            return false;
        }
        std::ofstream out("charts/" + std::to_string(userId) + "chart.png", std::ios::binary);
        out << r.text;
        out.close();
        return true;
    
}