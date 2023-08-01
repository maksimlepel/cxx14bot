#pragma once
#include <stdio.h>

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>

#include <thread>
#include <tgbot/tgbot.h>
#include <vector>
#include <ctime>
#include <cmath>
#include <thread>
#include <chrono>
#include "CoinInfo.h"
#include "DBhelper.h"
#define _CRT_SECURE_NO_WARNINGS
using namespace TgBot;

void createOneColumnKeyboard(const std::vector<std::string>& buttonStrings, ReplyKeyboardMarkup::Ptr& kb)
{
    for (size_t i = 0; i < buttonStrings.size(); ++i) {
        std::vector<KeyboardButton::Ptr> row;
        KeyboardButton::Ptr button(new KeyboardButton);
        button->text = buttonStrings[i];
        row.push_back(button);
        kb->keyboard.push_back(row);
    }
}

void createKeyboard(const std::vector<std::vector<std::string>>& buttonLayout, ReplyKeyboardMarkup::Ptr& kb)
{
    for (size_t i = 0; i < buttonLayout.size(); ++i) {
        std::vector<KeyboardButton::Ptr> row;
        for (size_t j = 0; j < buttonLayout[i].size(); ++j) {
            KeyboardButton::Ptr button(new KeyboardButton);
            button->text = buttonLayout[i][j];
            row.push_back(button);
        }
        kb->keyboard.push_back(row);
    }
}

int main() {
    setlocale(LC_ALL, "");
    DBhelper db;
    std::thread t([&db]() 
        {
            while (1)
            {
                db.UpdateCoinTable();
                std::this_thread::sleep_for(std::chrono::milliseconds(5000));
            }      

        });


    CoinInfo ci;
    std::vector<std::string> commands = { "start","TopGainers", "TopLoosers" };
    TgBot::Bot bot(SecretInfo::GetToken());

    ReplyKeyboardMarkup::Ptr keyboardWithLayout(new ReplyKeyboardMarkup);
    createKeyboard({
         {"TopGainers", "TopLoosers"},
      {"btc", "eth", "near","xrp","bnb"},
      {"ada", "sol", "doge","trx","matic"},
      {"ltc", "atom", "shib","icp","link"}
        }, keyboardWithLayout);

    bot.getEvents().onCommand("start", [&keyboardWithLayout ,&bot,&db](TgBot::Message::Ptr message) {

        db.AddUser(message->chat->id, message->chat->firstName);
        
        //bot.getApi().sendMessage(message->chat->id, "Hi "+ message->chat->firstName+", please type coin ticker, chatid: "+ std::to_string(message->chat->id));
        bot.getApi().sendMessage(message->chat->id, "Hi " + message->chat->firstName + ", please type coin ticker, chatid: " + std::to_string(message->chat->id), false, 0, keyboardWithLayout);
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
        if (message->text == commands[1])
        {
            bot.getApi().sendMessage(message->chat->id, db.Get10Gainers());
        }

        if (message->text == commands[2])
        {
            bot.getApi().sendMessage(message->chat->id, db.Get10Loosers());
        }
        
        for (auto i : commands)
        {
            if (message->text == "/"+i  || message->text ==  i)
            {
                return;
            }
        }


        bot.getApi().sendMessage(message->chat->id, db.GetCoinInfo(message->text));
        });

    try {


        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        bot.getApi().deleteWebhook();

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