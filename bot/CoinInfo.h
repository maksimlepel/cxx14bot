#pragma once
#include "JSONParser.h"
#include <iostream>
class CoinInfo:JSONParser
{
public:
	 std::string GetCoinPtice(std::string ticker);


};
//some changes