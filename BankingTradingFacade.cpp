// BankingTradingFacade.cpp
// Implementation of the Facade pattern for the Banking Trading System

#include "BankingTradingFacade.h"

// Constructor
BankingTradingFacade::BankingTradingFacade() : currentDay_(1) {
}

// Helper methods to access singleton subsystems
BankingSystem& BankingTradingFacade::getBankingSystem() const {
    return BankingSystem::getInstance();
}

TradingBot& BankingTradingFacade::getTradingBot() const {
    return TradingBot::getInstance();
}

std::string BankingTradingFacade::transactionTypeToString(Transaction::Type type) const {
    switch(type) {
        case Transaction::DEPOSIT: return "Deposit";
        case Transaction::WITHDRAWAL: return "Withdrawal";
        case Transaction::STOCK_PURCHASE: return "Stock Purchase";
        case Transaction::STOCK_SALE: return "Stock Sale";
        case Transaction::FEE: return "Fee";
        default: return "Unknown";
    }
}

// Authentication methods
bool BankingTradingFacade::login(const std::string& username, const std::string& password) {
    return getBankingSystem().login(username, password);
}

bool BankingTradingFacade::registerUser(const std::string& username, const std::string& password, 
                                double initialBalance) {
    return getBankingSystem().registerUser(username, password, initialBalance);
}

void BankingTradingFacade::logout() {
    // Stop bot before logging out
    if (getTradingBot().isRunning()) {
        getTradingBot().stopBot();
    }
    getBankingSystem().logout();
}

bool BankingTradingFacade::isLoggedIn() const {
    return getBankingSystem().isLoggedIn();
}

std::string BankingTradingFacade::getCurrentUser() const {
    return getBankingSystem().getCurrentUser();
}

// --- Banking Operations ---
// Handle deposits, withdrawals, transactions, and scheduled deposits

bool BankingTradingFacade::deposit(double amount, const std::string& description, int day) {
    return getBankingSystem().deposit(amount, description, day);
}

bool BankingTradingFacade::withdraw(double amount, const std::string& description, int day) {
    return getBankingSystem().withdraw(amount, description, day);
}

double BankingTradingFacade::getBalance() const {
    return getBankingSystem().getBalance();
}

std::vector<BankingTradingFacade::SimpleTransaction> BankingTradingFacade::getTransactionHistory() const {
    std::vector<SimpleTransaction> result;
    std::vector<Transaction> transactions = getBankingSystem().getTransactionHistory();
    
    for (const auto& trans : transactions) {
        SimpleTransaction st;
        st.type = transactionTypeToString(trans.getType());
        st.amount = trans.getAmount();
        st.description = trans.getDescription();
        st.day = trans.getDay();
        st.timestamp = trans.getTimestamp();
        result.push_back(st);
    }
    
    return result;
}

bool BankingTradingFacade::scheduleDeposit(int day, double amount, const std::string& description) {
    return getBankingSystem().scheduleDeposit(day, amount, description);
}

std::vector<BankingTradingFacade::SimpleScheduledDeposit> BankingTradingFacade::getScheduledDeposits() const {
    std::vector<SimpleScheduledDeposit> result;
    std::vector<ScheduledDeposit> scheduled = getBankingSystem().getScheduledDeposits();
    
    for (const auto& dep : scheduled) {
        SimpleScheduledDeposit sd;
        sd.scheduledDay = dep.getScheduledDay();
        sd.amount = dep.getAmount();
        sd.description = dep.getDescription();
        sd.executed = dep.isExecuted();
        result.push_back(sd);
    }
    
    return result;
}

int BankingTradingFacade::executeScheduledDeposits(int currentDay) {
    return getBankingSystem().executeScheduledDeposits(currentDay);
}

// --- Trading Bot Operations ---
// Start, stop, and monitor the automated trading bot

bool BankingTradingFacade::startBot() {
    if (!isLoggedIn()) {
        return false;
    }
    
    getTradingBot().startBot();
    return true;
}

bool BankingTradingFacade::stopBot() {
    getTradingBot().stopBot();
    return true;
}

bool BankingTradingFacade::isBotRunning() const {
    return getTradingBot().isRunning();
}

std::string BankingTradingFacade::getBotStatus() const {
    return isBotRunning() ? "ACTIVE" : "INACTIVE";
}

// --- Stock Market Data ---
// Get current market prices and stock information

std::vector<BankingTradingFacade::SimpleStockInfo> BankingTradingFacade::getMarketData() {
    std::vector<SimpleStockInfo> result;
    
    // Get stock data from TradingBot
    std::vector<StockFields> stocks = getTradingBot().getAllStocks();
    
    for (const auto& stock : stocks) {
        SimpleStockInfo info;
        info.symbol = stock.ticker_symbol;
        info.name = stock.name;
        info.currentPrice = stock.cur;
        info.previousPrice = stock.prev;
        info.percentChange = ((stock.cur - stock.prev) / stock.prev) * 100.0;  // Calculate inline
        
        // Determine trend
        if (stock.priceUp()) {
            info.trend = "UP";
        } else if (stock.priceDown()) {
            info.trend = "DOWN";
        } else {
            info.trend = "STABLE";
        }
        
        result.push_back(info);
    }
    
    return result;
}

void BankingTradingFacade::refreshMarketData() {
    // Market data updates automatically through TradingBot
    // Could add manual refresh capability here later
}

// --- Portfolio Management ---
// Track owned stocks and their current values

std::vector<BankingTradingFacade::SimplePortfolioItem> BankingTradingFacade::getPortfolio() {
    std::vector<SimplePortfolioItem> result;
    
    std::vector<Portfolio> portfolio = getTradingBot().getPortfolio();
    
    for (const auto& item : portfolio) {
        SimplePortfolioItem spi;
        spi.symbol = item.ticker_symbol;
        spi.shares = item.shares;
        spi.averagePrice = item.averageCost;
        
        // Get current price
        double currentPrice = getTradingBot().getPrice(item.ticker_symbol);
        spi.currentValue = item.getValue(currentPrice);
        spi.profit = item.getProfits(currentPrice);
        spi.profitPercent = item.getProfitPercent(currentPrice);
        
        result.push_back(spi);
    }
    
    return result;
}

// --- Performance Tracking ---
// Calculate profit, success rate, and trading statistics

BankingTradingFacade::PerformanceSummary BankingTradingFacade::getPerformance() {
    PerformanceSummary summary;
    
    summary.daysElapsed = currentDay_;
    
    // Get trade history from TradingBot
    std::vector<TradeRecords> trades = getTradingBot().getHistory();
    summary.tradesExecuted = trades.size();
    
    // Calculate totals from portfolio
    summary.totalProfit = 0.0;
    summary.totalShares = 0;
    summary.totalValue = 0.0;
    
    std::vector<Portfolio> portfolio = getTradingBot().getPortfolio();
    for (const auto& item : portfolio) {
        summary.totalShares += item.shares;
        double currentPrice = getTradingBot().getPrice(item.ticker_symbol);
        summary.totalValue += item.getValue(currentPrice);
        summary.totalProfit += item.getProfits(currentPrice);
    }
    
    // Calculate success rate
    int profitable = 0;
    for (const auto& trade : trades) {
        if (trade.type == "SELL" && trade.total > 0) {
            profitable++;
        }
    }
    
    if (trades.size() > 0) {
        summary.successRate = (double)profitable / trades.size() * 100.0;
    } else {
        summary.successRate = 0.0;
    }
    
    return summary;
}

std::vector<BankingTradingFacade::SimpleTradeRecord> BankingTradingFacade::getTradeHistory() {
    std::vector<SimpleTradeRecord> result;
    
    std::vector<TradeRecords> trades = getTradingBot().getHistory();
    
    for (const auto& trade : trades) {
        SimpleTradeRecord str;
        str.type = trade.type;
        str.symbol = trade.ticker_symbol;
        str.shares = trade.shares;
        str.price = trade.cost;
        str.total = trade.total;
        str.day = trade.day;
        str.reason = trade.reason;
        
        result.push_back(str);
    }
    
    return result;
}

// --- Day Management & Simulation Control ---
// Advance time and reset the simulation

int BankingTradingFacade::advanceDay() {
    currentDay_++;
    
    // Execute scheduled deposits
    executeScheduledDeposits(currentDay_);
    
    // If bot is running, execute trading cycle
    if (isBotRunning()) {
        getTradingBot().executeTradingCycle();
    }
    
    return currentDay_;
}

int BankingTradingFacade::getCurrentDay() const {
    return currentDay_;
}

void BankingTradingFacade::resetSimulation() {
    // Stop bot
    if (isBotRunning()) {
        getTradingBot().stopBot();
    }
    
    // Reset trading bot
    getTradingBot().reset();
    
    // Reset current account
    if (isLoggedIn()) {
        getBankingSystem().resetCurrentAccount(10000.0);
    }
    
    // Reset day
    currentDay_ = 1;
}
