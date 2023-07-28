#pragma once
#include <iostream>
#include <vector>
#include <pqxx/pqxx>
#include "SecretInfo.h"
#include "JSONParser.h"
#include <boost/algorithm/string.hpp>
class DBhelper:JSONParser
{
public:
	DBhelper();
	std::vector<int> getUsersID();
	void UpdateCoinTable();
	void AddUser(int userID, std::string firstName);
	std::string GetCoinInfo(std::string ticker);
	std::string Get10Gainers();
	std::string Get10Loosers();
private:
	std::string tickers = "[\"BTCUSDT\",\"ETHUSDT\",\"NEARUSDT\",\"BNBUSDT\",\"ADAUSDT\",\"DOGEUSDT\",\"XRPUSDT\",\"DOTUSDT\",\"UNIUSDT\",\"LINKUSDT\",\"LTCUSDT\",\"BCHUSDT\",\"SOLUSDT\",\"MATICUSDT\",\"ICPUSDT\",\"XLMUSDT\",\"ETCUSDT\",\"VETUSDT\",\"THETAUSDT\",\"FILUSDT\",\"TRXUSDT\",\"EOSUSDT\",\"AAVEUSDT\",\"ATOMUSDT\",\"ALGOUSDT\",\"XTZUSDT\",\"NEOUSDT\",\"AVAXUSDT\",\"SHIBUSDT\",\"CAKEUSDT\",\"KSMUSDT\",\"BATUSDT\",\"DASHUSDT\",\"ZECUSDT\",\"CRVUSDT\",\"GRTUSDT\",\"SUSHIUSDT\",\"ONEUSDT\",\"MKRUSDT\",\"MANAUSDT\",\"HBARUSDT\",\"SCUSDT\",\"ENJUSDT\",\"IOSTUSDT\",\"ZILUSDT\",\"SANDUSDT\",\"FLOWUSDT\",\"CELOUSDT\",\"OMGUSDT\",\"RENUSDT\",\"LUNAUSDT\",\"BALUSDT\",\"SRMUSDT\",\"CHRUSDT\",\"ANKRUSDT\",\"HNTUSDT\",\"KAVAUSDT\",\"RUNEUSDT\",\"SKLUSDT\",\"OGNUSDT\",\"COTIUSDT\",\"GTCUSDT\",\"DODOUSDT\",\"RLCUSDT\",\"REEFUSDT\",\"VTHOUSDT\",\"1INCHUSDT\",\"MTLUSDT\",\"RAYUSDT\",\"CTSIUSDT\",\"OCEANUSDT\",\"STXUSDT\",\"NKNUSDT\",\"MASKUSDT\",\"AUDIOUSDT\",\"LRCUSDT\",\"ALPHAUSDT\",\"FETUSDT\"]";
};