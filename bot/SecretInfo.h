#pragma once
#include <string>
class SecretInfo
{
public:
	static std::string GetConnectionString() 
	{
		return "host=localhost port=5432 dbname=CXXbot user=postgres password =Auashi_Sagami_2020";
	}
	static std::string GetToken()
	{
		return "6360658598:AAHb3xzL0XBEXHkZ3Jog1bAI4EuoSE08kOw";
	}
};