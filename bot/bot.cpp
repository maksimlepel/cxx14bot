#pragma once
#include <stdio.h>
#include <tgbot/tgbot.h>
#include <vector>
#include <ctime>
#include <cmath>
#include <thread>
#include <chrono>
#include "CoinInfo.h"
#include "DBhelper.h"
#define _CRT_SECURE_NO_WARNINGS

int main() {

    setlocale(LC_ALL, "Russian");
    DBhelper db;

    CoinInfo ci;
    std::vector<std::string> commands = { "start"};
    TgBot::Bot bot(SecretInfo::GetToken());

    bot.getEvents().onCommand("start", [&bot,&db](TgBot::Message::Ptr message) {

        db.AddUser(message->chat->id, message->chat->firstName);
        
        bot.getApi().sendMessage(message->chat->id, "Hi "+ message->chat->firstName+", please type coin ticker, chatid: "+ std::to_string(message->chat->id));
        });





    bot.getEvents().onAnyMessage([&db, &ci ,&bot,&commands](TgBot::Message::Ptr message) {

        if (message->chat->id == 879628270 && message->text.substr(0,3)=="###") {
            std::vector<int> ids = db.getUsersID();
            for (int i : ids)
            {
                try
                {
                    bot.getApi().sendMessage(i, message->text.substr(3, message->text.length()-3));
                }
                catch (...) {}

            }
            return;
        }

        for (auto i : commands)
        {
            if (message->text == "/"+i) {
                return;
            }
        }

        bot.getApi().sendMessage(message->chat->id, ci.GetCoinPrice(message->text));
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