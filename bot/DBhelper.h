#pragma once
#include <iostream>
#include <vector>
#include <pqxx/pqxx>
#include "SecretInfo.h"
class DBhelper
{
public:
	DBhelper();
	std::vector<int> getUsersID();
	void AddUser(int userID, std::string firstName);
};