#pragma once
#include "JSONParser.h"
#include <iostream>
#include <boost/algorithm/string.hpp>
class CoinInfo:JSONParser
{
public:
	 std::string GetCoinInfo(std::string ticker);
};
