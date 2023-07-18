#include <stdio.h>
#include <tgbot/tgbot.h>
#include <vector>
#include <ctime>
#include <cmath>
#include <thread>
#include <chrono>
#include "CoinInfo.h"
#include <pqxx/pqxx>
#define _CRT_SECURE_NO_WARNINGS

int main() {

    setlocale(LC_ALL, "Russian");
    std::string connectionString = "host=localhost port=5432 dbname=CXXbot user=postgres password =Auashi_Sagami_2020";
    try
    {
        pqxx::connection connectionObject(connectionString.c_str());
        pqxx::work worker(connectionObject);
        pqxx::result response = worker.exec("SELECT * FROM users");

        for (size_t i = 0; i < response.size(); i++)
        {
            std::cout << "Id: " << response[i][0] << " Username: " << response[i][1] << std::endl;
        }
        worker.commit();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }


    CoinInfo ci;
    std::vector<std::string> commands = { "start","getnearprice"};
    TgBot::Bot bot("6360658598:AAHb3xzL0XBEXHkZ3Jog1bAI4EuoSE08kOw");

    bot.getEvents().onCommand("start", [&bot,&connectionString](TgBot::Message::Ptr message) {
        try
        {
            pqxx::connection connectionObject(connectionString.c_str());
            pqxx::work worker(connectionObject);
            worker.exec("INSERT INTO USERS VALUES(" + std::to_string(message->chat->id) + ", '" + message->chat->firstName + "')");
            worker.commit();
        }
        catch (...){}
        
        bot.getApi().sendMessage(message->chat->id, "Hi "+ message->chat->firstName+", please type coin ticker, chatid: "+ std::to_string(message->chat->id));
        });


    bot.getEvents().onCommand("getnearprice", [&bot](TgBot::Message::Ptr message) {
       
       /* while (1) {
            bot.getApi().sendMessage(message->chat->id, getCoinPrice("near"));
            this_thread::sleep_for(chrono::milliseconds(500));
        }*/
        });


    bot.getEvents().onAnyMessage([&ci ,&bot,&commands](TgBot::Message::Ptr message) {
        for (auto i : commands)
        {
            if (message->text == "/"+i) {
                return;
            }
        }

        bot.getApi().sendMessage(message->chat->id, ci.GetCoinPtice(message->text));
        });
    try {


        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    }
    catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }
    
    return 0;
}