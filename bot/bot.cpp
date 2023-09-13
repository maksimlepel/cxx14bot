#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <random>
#include <filesystem>
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
    bool needChart = false;
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
    std::vector<std::string> commands = { "start","TopGainers", "TopLoosers", "TarotPricePredict","Chart" };
    TgBot::Bot bot(SecretInfo::GetToken());

    ReplyKeyboardMarkup::Ptr keyboardWithLayout(new ReplyKeyboardMarkup);
    createKeyboard({
      {"TopGainers", "TopLoosers", "TarotPricePredict", "Chart"},
      {"btc", "eth", "near","xrp","bnb"},
      {"ada", "sol", "doge","trx","matic"},
      {"ltc", "atom", "shib","icp","link"}
        }, keyboardWithLayout);

    bot.getEvents().onCommand("start", [&keyboardWithLayout ,&bot,&db](TgBot::Message::Ptr message) {

        db.AddUser(message->chat->id, message->chat->firstName);
        
        bot.getApi().sendMessage(message->chat->id, "Hi " + message->chat->firstName + ", please type coin ticker, chatid: " + std::to_string(message->chat->id), false, 0, keyboardWithLayout);
        });






    bot.getEvents().onAnyMessage([&db, &ci ,&bot,&commands,&needChart](TgBot::Message::Ptr message) {

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

        if (message->text == commands[3])
        {

            srand(time(NULL));
                    auto cat = bot.getApi().sendPhoto(message->chat->id, InputFile::fromFile("cat.jpg", "image/jpeg"));
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    auto process = bot.getApi().sendAnimation(message->chat->id, InputFile::fromFile("process.gif", "image/gif"), message->messageId);
                    std::this_thread::sleep_for(std::chrono::seconds(3));
                    auto msg = bot.getApi().sendPhoto(message->chat->id, InputFile::fromFile("tarot/" + std::to_string(rand()%20+1) + ".jpg", "image/jpeg"));
                    std::vector<std::string> LS = { "long","short" };
                    std::vector<std::string> laverage = { "x1","x2","x3","x5","x10","x15","x20","x25","x50","x100" };
                    std::vector<std::string> tickers = { "BTCUSDT","ETHUSDT","NEARUSDT","BNBUSDT","ADAUSDT","DOGEUSDT","XRPUSDT","DOTUSDT","UNIUSDT","LINKUSDT","LTCUSDT","BCHUSDT","SOLUSDT","MATICUSDT","ICPUSDT" };
                  

                    std::string mass = "It means you should take " + LS[rand() % LS.size()] + " " + tickers[rand() % tickers.size()] + " at " + laverage[rand() % laverage.size()] + ".";

                    bot.getApi().sendMessage(message->chat->id,mass);
                    bot.getApi().deleteMessage(cat->chat->id, cat->messageId);
                    bot.getApi().deleteMessage(process->chat->id, process->messageId);

            
        }

        if (message->text == commands[4])
        {
            needChart = true;
            bot.getApi().sendMessage(message->chat->id, "Please type coin ticker");
        }

        for (auto i : commands)
        {
            if (message->text == "/"+i  || message->text ==  i)
            {
                return;
            }
        }

        if(needChart)
        {
            bot.getApi().sendMessage(message->chat->id, "parsing chart from binance...");
            if (ci.getChart(message->text, message->chat->id))
            {
                
                auto msg = bot.getApi().sendPhoto(message->chat->id, InputFile::fromFile("charts/" + std::to_string(message->chat->id) + "chart.png", "image/png"));

            }
            else 
            {
                bot.getApi().sendMessage(message->chat->id, "please enter correct ticker");
            }
            needChart = false;
        }
        else{

            bot.getApi().sendMessage(message->chat->id, db.GetCoinInfo(message->text));
        }

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