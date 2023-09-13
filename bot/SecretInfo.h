#pragma once
#include <string>
//
class SecretInfo
{
public:
	static std::string GetConnectionString() 
	{
		return connectionString;
	}
	static std::string GetToken()
	{  
		return token;
	}
private:
	inline static std::string connectionString = "host=localhost port=5432 dbname=CXXbot user=postgres password =Auashi_Sagami_2020";
	inline static std::string token = "6360658598:AAHb3xzL0XBEXHkZ3Jog1bAI4EuoSE08kOw";
};