#include "DBhelper.h"
#include "helperFunctions.h"
DBhelper::DBhelper()
{
    
}
std::vector<int> DBhelper:: getUsersID()
{
    pqxx::connection connectionObject(SecretInfo::GetConnectionString().c_str());
    pqxx::work worker(connectionObject);
    std::vector<int>UsersID;
    try
    {
        for (auto [id, name] : worker.query< int, std::string>(
            "SELECT * FROM users"))
        {
            UsersID.push_back(id);
        }
        worker.commit();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return UsersID;
}

void DBhelper::UpdateCoinTable()
{
    setlocale(LC_ALL, "Russian");
    std::cout << "update coins info..." << std::endl;
    auto js_obj = nlohmann::json::parse(Get_request("https://api.binance.com/api/v3/ticker/24hr?symbols=" + tickers));
    std::cout << "update coins info OK" << std::endl;
    pqxx::connection connectionObject(SecretInfo::GetConnectionString().c_str());
    pqxx::work worker(connectionObject);
    try
    {
        std::cout << "update DB..." << std::endl;
        for (int i = 0; i < 78; i++)
        {
            std::string ticker = js_obj[i]["symbol"].get<std::string>();
            std::string maxPrice = js_obj[i]["highPrice"].get<std::string>();
            std::string minPrice = js_obj[i]["lowPrice"].get<std::string>();
            std::string currPrice = js_obj[i]["lastPrice"].get<std::string>();
            std::string volume = js_obj[i]["quoteVolume"].get<std::string>();
            std::string priceChangePercent = js_obj[i]["priceChangePercent"].get<std::string>();
            //std::string query = "INSERT INTO COINS VALUES('" + ticker + "', " + maxPrice + ", " + minPrice + ", " + priceChangePercent + ", " + volume + ", " + currPrice + ")";
            std::string query1 = "UPDATE COINS SET maxprice = " + maxPrice + ", minprice = " + minPrice + ", pricechangepercent = " + priceChangePercent + ", volume = " + volume + ", lastprice = " + currPrice + " WHERE ticker = '" + ticker+"'";
            worker.exec(query1);
        }

        worker.commit();
        std::cout << "update DB OK" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}


void DBhelper::AddUser(int userID,std::string firstName)
{
    pqxx::connection connectionObject(SecretInfo::GetConnectionString().c_str());
    pqxx::work worker(connectionObject);
    try
    {
        worker.exec("INSERT INTO USERS VALUES(" + std::to_string(userID) + ", '" + firstName + "')");
        worker.commit();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

std::string DBhelper::GetCoinInfo(std::string ticker)
{
    if (ticker[0] == '/')
    {
        ticker.erase(0, 1);
    }
    boost::to_upper(ticker);

    pqxx::connection connectionObject(SecretInfo::GetConnectionString().c_str());
    pqxx::work worker(connectionObject);

    try
    {
        
        std::string price = worker.query_value <std::string>("SELECT lastprice from coins where ticker = '" + ticker + "USDT'");
        std::string lowPrice = worker.query_value <std::string>("SELECT minprice from coins where ticker = '" + ticker + "USDT'");
        std::string highPrice = worker.query_value <std::string>("SELECT maxprice from coins where ticker = '" + ticker + "USDT'");
        std::string priceChangePercent = worker.query_value <std::string>("SELECT pricechangepercent from coins where ticker = '" + ticker + "USDT'");

        std::string substr = "up";
        if (priceChangePercent[0] == '-') {
            substr = "down"; priceChangePercent.erase(0, 1);
        }
        worker.commit();
        helperFunctions::RemoveZeroes(price);
        helperFunctions::RemoveZeroes(lowPrice);
        helperFunctions::RemoveZeroes(highPrice);
        return "The " + ticker + " price is " + price + " usdt now.\n" + "It's " + substr + " " + priceChangePercent + "% last 24h\nMax price: " + highPrice + "$\nLow price: " + lowPrice + "$";
        

    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return "please enter correct ticker";
    }
   
}

std::string DBhelper::Get10Gainers()
{
    std::string result = "";
    pqxx::connection connectionObject(SecretInfo::GetConnectionString().c_str());
    pqxx::work worker(connectionObject);
    std::vector<int>UsersID;
    try
    {
        int count = 1;
        for (auto [ticker,maxprice,minprice, pricechangepercent,volume,lastprice] : worker.query< std::string, double,double,std::string, double, double>(
            "SELECT * FROM coins ORDER BY pricechangepercent DESC LIMIT 10"))
        {
            result += std::to_string(count) + ". " + ticker + ": " + pricechangepercent + "%\n";
                count++;
        }
        worker.commit();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return result;
}

std::string DBhelper::Get10Loosers()
{
    std::string result = "";
    pqxx::connection connectionObject(SecretInfo::GetConnectionString().c_str());
    pqxx::work worker(connectionObject);
    std::vector<int>UsersID;
    try
    {
        int count = 1;
        for (auto [ticker, maxprice, minprice, pricechangepercent, volume, lastprice] : worker.query< std::string, double, double, std::string, double, double>(
            "SELECT * FROM coins ORDER BY pricechangepercent ASC LIMIT 10"))
        {
            result += std::to_string(count) + ". " + ticker + ": " + pricechangepercent + "%\n";
            count++;
        }
        worker.commit();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return result;
}
