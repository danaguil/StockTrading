#ifndef TRADINGBOTFUNC_H
#define TRADINGBOTFUNC_H

#include "StockAbstractFactory.h"
#include "BankingSystem.h"
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

struct StockFields {
    Stock *stock;
    StockPriceGenerator *generator;
    string ticker_symbol;
    string name;
    double cur;
    double prev;
    double openingPrice;


    // checks if stock price went up or down during that day
    bool priceDown() const {
        return cur < prev;
    }

    bool priceUp() const {
        return cur > prev;
    }


    //get change in percentage of current price from the day before
    double getPercentChange() {
        if(prev == 0) {
            return 0.0;
        }

        return ((cur - prev) / prev) * 100.0;
    }

};



struct StockRanks {
    string ticker_symbol;
    double cur;
    double score;
    int recommendedShares;
};

struct TradeRecords {
    string type;
    string ticker_symbol;
    int shares;
    double cost;
    double total;
    int day;
    string reason;

};


struct Portfolio {

    string ticker_symbol;
    int shares;
    double averageCost;
    double totalCost;


    double getValue(double cost) const {
        return shares * cost;
    }

    double getProfits(double cost) const {
        return getValue(cost) - totalCost;
    }

    double getProfitPercent(double cost) const {
        if (totalCost <= 0) {
            return 0.0;
        }

        return (getProfits(cost) / totalCost) * 100.0;
    }


};


class TradeStrategy {
public:
    virtual ~TradeStrategy() = default;

    virtual vector<StockRanks> rankStocks(const vector<StockFields>& stocks, double balance) = 0;

    virtual double getTakeProfit() const = 0;  // When to sell for profit
    virtual double getStopLoss() const = 0;    // When to sell to prevent loss
    virtual int getMaxHoldings() const = 0;
    virtual string getStrategyName() const = 0;

};

/*

    A simple strategy where the bot buys stocks that are currently down.
    Simulates a person buying during a dip in stock price hoping to earn more.
    Will sell if profit is greater than the set percetage amount or if the loss is around or greater than 10%.

*/
class AggressiveStrategy : public TradeStrategy {

public:
    vector<StockRanks> rankStocks(const vector<StockFields>& stocks, double balance) override {

        vector<StockRanks> stockRankings;

        for (const auto& stock : stocks) {

            StockRanks theRank;
            theRank.ticker_symbol = stock.ticker_symbol;
            theRank.cur = stock.cur;


            if (stock.priceDown()) {
                theRank.score = 100;
            } else {
                theRank.score = 0;
            }


            theRank.recommendedShares = (int)((balance*0.25) / stock.cur);

            if (theRank.recommendedShares < 1) theRank.recommendedShares = 1;

            stockRankings.push_back(theRank);

        }

        sort(stockRankings.begin(), stockRankings.end(), [](const StockRanks& a, const StockRanks& b) {
            return a.score > b.score;
        });

        return stockRankings;
    }

    string getStrategyName() const override {
        return "Aggressive";
    }
    double getTakeProfit() const override {
        return 0.15;
    }  // Sell at 15% profit

    double getStopLoss() const override {
        return -0.10;
    }   // Sell at 10% loss
    int getMaxHoldings() const override {
        return 3;
    }       // Max 3 stocks
};


/*

    A simple strategy where the bot buys stocks that are currently up.
    Simulates a person buying during a safe time where they follow the trend of stock optimism.
    Will sell if profit is greater than the set percetage amount or if the loss is around or greater than 3%.

*/
class ConservativeStrategy : public TradeStrategy {

public:
    vector<StockRanks> rankStocks(const vector<StockFields>& stocks, double balance) override {

        vector<StockRanks> stockRankings;

        for (const auto& stock : stocks) {

            StockRanks theRank;
            theRank.ticker_symbol = stock.ticker_symbol;
            theRank.cur = stock.cur;


            if (stock.priceUp()) {
                theRank.score = 100;
            } else {
                theRank.score = 0;
            }


            theRank.recommendedShares = (int)((balance*0.25) / stock.cur);

            if (theRank.recommendedShares < 1) theRank.recommendedShares = 1;

            stockRankings.push_back(theRank);

        }

        sort(stockRankings.begin(), stockRankings.end(), [](const StockRanks& a, const StockRanks& b) {
            return a.score > b.score;
        });

        return stockRankings;
    }

    string getStrategyName() const override {
        return "Conservative";
    }

    // sells when there is a 5% margin of profit
    double getTakeProfit() const override {
        return 0.05;
    }

    // sells when there is a loss of 3%
    double getStopLoss() const override {
        return -0.03;
    }
    // need
    int getMaxHoldings() const override {
        return 5;
    }
};


/*
    Analyses the stock market simulation. Labels the current day as BEARISH where stock prices in the sim are going down.
    Day is labeled BULLISH if stock prices in the sim are going up in the day.

    Simple count of stocks. More stocks that day that went up = BULLISH, otherwise the day is labeled BEARISH

*/
class StockMarketAnalyser {
public:
    enum Condition { BULLISH, BEARISH };


    Condition analyzeMarket(const vector<StockFields>& stocks) {

        int upCount = 0;

        int downCount = 0;

        for (const auto& stock : stocks) {
            if (stock.priceUp()) upCount++;
            else if (stock.priceDown()) downCount++;
        }

        // If more stocks went up, market is bullish
        if (upCount >= downCount) {
            return BULLISH;
        } else {
            return BEARISH;
        }
    }

    // function to tell bot that market conditions are good and to perform appropriate strategy
    bool goAggressive(Condition state) {
        return (state == BULLISH);
    }

    // returns the market condition as a string
    string stringCondition(Condition state) {
        if (state == BULLISH) {
            return "BULLISH";
        }

        return "BEARISH";
    }
};


/*
    Trading Bot logic implementation

*/

class TradingBot {
private:

    vector<StockFields> stocks;
    unordered_map<string, Portfolio> portfolio;
    vector<TradeRecords> history;
    vector<StockRanks> rankings;

    bool running;
    bool autoSwitch;
    int currentDay;
    double realizedProfit;
    string marketCondition;

    StockAbstractFactory* factory;
    TradeStrategy* strategy;
    StockMarketAnalyser analyser;



    // Private constructor using a Singleton
    //this inititates the trading bot and simulation. Default Conservative.
    TradingBot() {

        running = false;
        autoSwitch = true;
        currentDay = 1;
        realizedProfit = 0;
        marketCondition = "UNKNOWN";

        factory = new SimpleStockFactory();
        strategy = new ConservativeStrategy();

        addStock("GOOG", "Alphabet", 320.12);
        addStock("AMZN", "Amazon", 233.22);
        addStock("NVDA", "Nvidia", 176.98);
        addStock("MSFT", "Microsoft", 491.92);
        addStock("META", "Meta Platforms", 647.95);
        addStock("GME", "GameStop Corp", 22.53);
        addStock("TSLA", "Tesla Inc", 430.17);
        addStock("GM", "General Motors", 45.00);
        addStock("F", "Ford Motor Co", 13.28);
        addStock("WMT", "Walmart Inc", 110.51);
        addStock("YELP", "Yelp Inc", 28.91);
        addStock("SONY", "Sony Group Corp", 29.35);
        addStock("MCD", "McDonalds Corp", 311.82);
        addStock("CSUSM", "San Marcos", 100.00);
    }

    void addStock(string symbol, string name, double price) {

        StockFields s;
        s.ticker_symbol = symbol;
        s.name = name;
        s.cur = price;
        s.prev = price;
        s.openingPrice = price;
        s.stock = factory->createStock(symbol);
        s.generator = factory->createPriceGenerator();
        stocks.push_back(s);
    }



    // check the current portfolio for stocks to sell
    void checkSells() {
        vector<string> toSell;

        for (auto& p : portfolio) {
            double price = getPrice(p.first);
            double profitPct = p.second.getProfitPercent(price) / 100.0;

            if (profitPct >= strategy->getTakeProfit()) {
                toSell.push_back(p.first);
            }
            else if (profitPct <= strategy->getStopLoss()) {
                toSell.push_back(p.first);
            }
        }

        // Sell them
        for (int i = 0; i < toSell.size(); i++) {
            string symbol = toSell[i];

            int shares = portfolio[symbol].shares;

            double price = getPrice(symbol);

            double profitPct = portfolio[symbol].getProfitPercent(price) / 100.0;

            string reason = (profitPct > 0) ? "Take profit" : "Stop loss";
            sell(symbol, shares, reason);
        }
    }

    // check for stocks to buy
    void checkBuys() {
        int holdings = portfolio.size();

        for (int i = 0; i < rankings.size(); i++) {
            if (rankings[i].score <= 0) continue;
            if (portfolio.count(rankings[i].ticker_symbol)) continue;

            //check to see if we have the max amount of holdings
            if (holdings >= strategy->getMaxHoldings()) break;

            // Gets the reason for the bot buying the stock. This will be displayed.
            if (buy(rankings[i].ticker_symbol, rankings[i].recommendedShares, strategy->getStrategyName() + " pick")) {
                holdings++;
            }
        }
    }

public:

    static TradingBot& getInstance() {
        static TradingBot instance;
        return instance;
    }

    // Delete copy
    TradingBot(TradingBot&) = delete;

    void operator=(TradingBot&) = delete;

    // Used in MainWindow.cpp to check bot status. Bot is ON
    void startBot() {

        running = true;

    }

    // same thing for this function. Bot is OFF.
    void stopBot() {

        running = false;

    }

    // same thing here. Used in MainWindow.cpp
    bool isRunning() {

        return running;

    }

    // getter for the strategy being used
    string getName() {
        return strategy->getStrategyName();
    }

    double getPrice(string symbol) {

        for (int i = 0; i < stocks.size(); i++) {
            if (stocks[i].ticker_symbol == symbol) {
                return stocks[i].cur;
            }
        }
        return 0;
    }

    /*

        Important function that allows switching strategies during the simulation

    */
    void strategySwitch() {
        // checks current sim conditions (Bullish or Bearish markets)
        StockMarketAnalyser::Condition condition = analyser.analyzeMarket(stocks);

        // string conversion to display condition
        marketCondition = analyser.stringCondition(condition);

        // check how agggressive we should be investing
        bool needAggressive = analyser.goAggressive(condition);

        bool isAggressive = (strategy->getStrategyName() == "Aggressive");

        // Condition: If conditions are favorable switch strategies
        if (needAggressive && !isAggressive) {
            delete strategy;
            strategy = new AggressiveStrategy();
        }

        //go back to default conservative if needed here
        else if (!needAggressive && isAggressive) {
            delete strategy;
            strategy = new ConservativeStrategy();
        }
    }

    // Advance market one day
    void advanceDay() {
        currentDay++;

        for (int i = 0; i < stocks.size(); i++) {
            stocks[i].prev = stocks[i].cur;
            stocks[i].cur = stocks[i].generator->generate(stocks[i].cur);
            if (stocks[i].cur < 0.01) {
                stocks[i].cur = 0.01;
            }
        }
    }

    // Main trading cycle
    void executeTradingCycle() {
        // check the bot is still running
        if (!running) return;

        if (autoSwitch) strategySwitch();

        double balance = getAvailableBalance();

        rankings = strategy->rankStocks(stocks, balance);

        checkSells();
        checkBuys();
    }

    // logic for the bot to buy the shares
    bool buy(string symbol, int shares, string reason) {
        if (shares <= 0) return false;

        double price = getPrice(symbol);
        double cost = price * shares;

        BankingSystem& bank = BankingSystem::getInstance();
        if (cost > bank.getBalance()) return false;
        if (!bank.withdraw(cost, "Buy " + symbol, currentDay)) return false;

        // Update portfolio
        if (portfolio.count(symbol)) {
            portfolio[symbol].averageCost = (portfolio[symbol].totalCost + cost) / (portfolio[symbol].shares + shares);
            portfolio[symbol].shares += shares;
            portfolio[symbol].totalCost += cost;
        } else {
            Portfolio h;
            h.ticker_symbol = symbol;
            h.shares = shares;
            h.averageCost = price;
            h.totalCost = cost;
            portfolio[symbol] = h;
        }

        TradeRecords t;
        t.type = "BUY";
        t.ticker_symbol = symbol;
        t.shares = shares;
        t.cost = price;
        t.total = cost;
        t.day = currentDay;
        t.reason = reason;
        history.push_back(t);

        return true;
    }

    // logic for bot to sell shares
    bool sell(string symbol, int shares, string reason) {
        if (shares <= 0) return false;
        if (!portfolio.count(symbol)) return false;
        if (portfolio[symbol].shares < shares) return false;

        double price = getPrice(symbol);
        double revenue = price * shares;

        BankingSystem& bank = BankingSystem::getInstance();
        if (!bank.deposit(revenue, "Sell " + symbol, currentDay)) return false;

        // Calculate the profits from selling the update the portfolio
        double costBasis = portfolio[symbol].averageCost * shares;
        realizedProfit += revenue - costBasis;

        portfolio[symbol].shares -= shares;

        portfolio[symbol].totalCost -= costBasis;

        // Get rid of the stock symbol from portfolio if no shares owned

        if (portfolio[symbol].shares <= 0) {
            portfolio.erase(symbol);
        }

        TradeRecords t;

        t.type = "SELL";

        t.ticker_symbol = symbol;
        t.shares = shares;
        t.cost = price;
        t.total = revenue;
        t.day = currentDay;
        t.reason = reason;
        history.push_back(t);

        return true;
    }

    // Sell everything if desperate
    void liquidateAll() {
        vector<pair<string, int>> toSell;

        for (auto& p : portfolio) {
            toSell.push_back({p.first, p.second.shares});
        }

        for (int i = 0; i < toSell.size(); i++) {
            sell(toSell[i].first, toSell[i].second, "Liquidation");
        }
    }

    // Sell only profitable positions
    void liquidateProfitableOnly() {
        vector<pair<string, int>> toSell;
        for (auto& p : portfolio) {
            double price = getPrice(p.first);
            if (p.second.getProfits(price) > 0) {
                toSell.push_back({p.first, p.second.shares});
            }
        }
        for (int i = 0; i < toSell.size(); i++) {
            sell(toSell[i].first, toSell[i].second, "Take profit");
        }
    }

    // Try to end with profit
    bool tryEndWithProfit(int maxWaitDays, int currentWaitDay) {
        liquidateProfitableOnly();
        if (portfolio.empty()) return true;

        // maxWaitdays implementation if auto run bot here?
        // sell all if reaching max days
        if (currentWaitDay >= maxWaitDays) {
            liquidateAll();
            return true;
        }
        return false;
    }

    // Getters for data gathering
    double getAvailableBalance() {
        return BankingSystem::getInstance().getBalance();
    }

    double getProfit() {
        double unrealized = 0;
        for (auto& p : portfolio) {
            double price = getPrice(p.first);
            unrealized += p.second.getProfits(price);
        }
        return realizedProfit + unrealized;
    }

    int getTotalShares() {
        int total = 0;
        for (auto& p : portfolio) {
            total += p.second.shares;
        }
        return total;
    }

    int getCurrentDay() { return currentDay; }
    string getMarketCondition() { return marketCondition; }
    vector<StockFields> getAllStocks() { return stocks; }
    vector<TradeRecords> getHistory() { return history; }
    vector<StockRanks> getRankings() { return rankings; }

    vector<Portfolio> getPortfolio() {

        vector<Portfolio> result;

        for (auto& p : portfolio) {

            result.push_back(p.second);

        }
        return result;
    }

    void setAutoSwitch(bool enabled) {

        autoSwitch = enabled;

    }

    // Reset for new simulation
    void reset() {
        running = false;
        currentDay = 1;
        realizedProfit = 0;
        marketCondition = "UNKNOWN";
        portfolio.clear();
        history.clear();
        rankings.clear();

        delete strategy;
        strategy = new ConservativeStrategy();

        for (int i = 0; i < stocks.size(); i++) {
            stocks[i].cur = stocks[i].openingPrice;
            stocks[i].prev = stocks[i].openingPrice;
        }
    }

};



#endif
