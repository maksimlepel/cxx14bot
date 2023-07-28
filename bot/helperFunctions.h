#pragma once
#include <string>
namespace helperFunctions
{
	void RemoveZeroes(std::string &str)
	{
		int it = str.length() - 1;
		while (str[it] == '0' || str[it] == ',')
		{
			str.pop_back();
		}
	}
}