#include "DBhelper.h"
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

void DBhelper::AddUser(int userID,std::string firstName)
{
    pqxx::connection connectionObject(SecretInfo::GetConnectionString().c_str());
    pqxx::work worker(connectionObject);
    try
    {
        worker.exec("INSERT INTO USERS VALUES(" + std::to_string(userID) + ", '" + firstName + "')");
        worker.commit();
    }
    catch (...) {}
}

