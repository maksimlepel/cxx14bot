#pragma once
#include "JSONParser.h"
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <cpr/cpr.h>
class CoinInfo:JSONParser
{
public:
	 std::string GetCoinInfo(std::string ticker);
	 bool getChart(std::string ticker, int userId);
};
