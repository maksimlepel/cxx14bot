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
#include <cpr/cpr.h>
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
        
        //bot.getApi().sendMessage(message->chat->id, "Hi "+ message->chat->firstName+", please type coin ticker, chatid: "+ std::to_string(message->chat->id));
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
          //  auto thr = std::thread([&bot, &message]()
          //      {
            srand(time(NULL));
                    auto cat = bot.getApi().sendPhoto(message->chat->id, InputFile::fromFile("cat.jpg", "image/jpeg"));
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    auto process = bot.getApi().sendAnimation(message->chat->id, InputFile::fromFile("process.gif", "image/gif"), message->messageId);
                    std::this_thread::sleep_for(std::chrono::seconds(3));
                    auto msg = bot.getApi().sendPhoto(message->chat->id, InputFile::fromFile("tarot/" + std::to_string(rand()%20+1) + ".jpg", "image/jpeg"));
                    std::vector<std::string> LS = { "long","short" };
                    std::vector<std::string> laverage = { "x1","x2","x3","x5","x10","x15","x20","x25","x50","x100" };
                    std::vector<std::string> tickers = { "BTCUSDT","ETHUSDT","NEARUSDT","BNBUSDT","ADAUSDT","DOGEUSDT","XRPUSDT","DOTUSDT","UNIUSDT","LINKUSDT","LTCUSDT","BCHUSDT","SOLUSDT","MATICUSDT","ICPUSDT" };
                  
                  
                    int lsInd = rand() % LS.size();
                    int tickInd = rand() % tickers.size();
                    int lavInd = rand() % laverage.size();

                    std::string mass = "It means you should take " + LS[lsInd] + " " + tickers[tickInd] + " at " + laverage[lavInd] + ".";

                    bot.getApi().sendMessage(message->chat->id,mass);
                    bot.getApi().deleteMessage(cat->chat->id, cat->messageId);
                    bot.getApi().deleteMessage(process->chat->id, process->messageId);
            //    }

           // );
          //  thr.join();
            
        }

        if (message->text == commands[4])
        {
            needChart = true;
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
            boost::to_upper(message->text);
            bot.getApi().sendMessage(message->chat->id, "parsing chart from binance...");
            cpr::Response r = cpr::Post(cpr::Url{"https://api.chart-img.com/v2/tradingview/advanced-chart"},
            cpr::Header{{"x-api-key", "n61GiZUCj26uvpfJ72J0d4J6UMgNzkE7OvAddO14"}, { "content-type","application/json" }},
            cpr::Body{"{\"symbol\":\"BINANCE:"+ message->text+"USDT\",\"interval\":\"4h\",\"theme\":\"dark\"}"});
            std::ofstream out("chart.png", std::ios::binary);
            out << r.text;
            out.close();
            std::cout << r.status_code << std::endl;
            std::cout << r.header["content-type"] << std::endl;
            auto msg = bot.getApi().sendPhoto(message->chat->id, InputFile::fromFile("chart.png", "image/png"));
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