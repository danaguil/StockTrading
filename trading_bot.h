#ifndef TRADINGBOT_H
#define TRADINGBOT_H
#include <string>
#include <vector>

/*
    Implementation Plan
*/ 


//Should be an abstract class where concrete stratefies inherit from.
class TradeStrategy {

    virtual vector<stocks> stockRanking() = 0;

};

//Strategy for consservative investing. Should be used in general*. A low risk strategy with minimal gains.
class ConservativeStrat : public TradingStrategy {

};

//Strategy for aggressive investing, A high risk startegy that tries for large gains.
class AggressiveGrowthStrat : public TradeStrategy {

};


// class used to be able to change strategy at runtime depending on simulation conditions
class StockMarketAnalyser {

};


//Trading bot functionality 
class TradingBot {

    //function to buy and sell stocks
    void buy() {

    }

    void sell() {

    }

    //use the stock market analyser to be able to switch strats at runtime
    void strategySwitch() {

    }

    public:

        //need a way to know when to start and stop the bot from investing
        void startBot() {

        }

        void stopBot() {

        }

        //get the current investible amount
        double getAvailableBalance() {

        }

        //get the amount of profit gained during simulation
        double getProfit() {

        }

};