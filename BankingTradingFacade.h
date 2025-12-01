// BankingTradingFacade.h
// Facade Pattern: Provides a unified, simplified interface to the complex subsystems
// (BankingSystem, TradingBot, Stock Market) for the GUI layer

#ifndef BANKINGRADINGFACADE_H
#define BANKINGRADINGFACADE_H

#include "BankingSystem.h"
#include "TradingBot.h"
#include <string>
#include <vector>

// Banking Trading Facade - Singleton + Facade Pattern
// Simplifies interaction between GUI and backend subsystems
class BankingTradingFacade {
public:
    // Singleton getInstance method
    static BankingTradingFacade& getInstance() {
        static BankingTradingFacade instance;
        return instance;
    }
    
    // Delete copy constructor and assignment operator
    BankingTradingFacade(const BankingTradingFacade&) = delete;
    BankingTradingFacade& operator=(const BankingTradingFacade&) = delete;
    
    // Authentication methods
    bool login(const std::string& username, const std::string& password);
    bool registerUser(const std::string& username, const std::string& password, 
                     double initialBalance = 10000.0);
    void logout();
    bool isLoggedIn() const;
    std::string getCurrentUser() const;
    
    // Banking operations
    bool deposit(double amount, const std::string& description, int day);
    bool withdraw(double amount, const std::string& description, int day);
    double getBalance() const;
    
    // Transaction history
    struct SimpleTransaction {
        std::string type;
        double amount;
        std::string description;
        int day;
        std::string timestamp;
    };
    std::vector<SimpleTransaction> getTransactionHistory() const;
    
    // Scheduled deposits
    struct SimpleScheduledDeposit {
        int scheduledDay;
        double amount;
        std::string description;
        bool executed;
    };
    bool scheduleDeposit(int day, double amount, const std::string& description);
    std::vector<SimpleScheduledDeposit> getScheduledDeposits() const;
    int executeScheduledDeposits(int currentDay);
    
    // Trading bot operations
    bool startBot();
    bool stopBot();
    bool isBotRunning() const;
    std::string getBotStatus() const;
    
    // Market data access
    struct SimpleStockInfo {
        std::string symbol;
        std::string name;
        double currentPrice;
        double previousPrice;
        double percentChange;
        std::string trend;  // "UP", "DOWN", "STABLE"
    };
    std::vector<SimpleStockInfo> getMarketData();
    void refreshMarketData();
    
    // Portfolio information
    struct SimplePortfolioItem {
        std::string symbol;
        int shares;
        double averagePrice;
        double currentValue;
        double profit;
        double profitPercent;
    };
    std::vector<SimplePortfolioItem> getPortfolio();
    
    // Performance summary
    // ========================================================================
    
    struct PerformanceSummary {
        double totalProfit;
        double totalValue;
        int totalShares;
        int daysElapsed;
        int tradesExecuted;
        double successRate;
    };
    PerformanceSummary getPerformance();
    
    // Trade history
    struct SimpleTradeRecord {
        std::string type;      // "BUY" or "SELL"
        std::string symbol;
        int shares;
        double price;
        double total;
        int day;
        std::string reason;
    };
    std::vector<SimpleTradeRecord> getTradeHistory();
    
    // Day management and simulation control
    int advanceDay();
    int getCurrentDay() const;
    void resetSimulation();
    
private:
    // Private constructor for Singleton
    BankingTradingFacade();
    ~BankingTradingFacade() = default;
    
    // Current simulation day
    int currentDay_;
    
    // Helper methods to access subsystems
    BankingSystem& getBankingSystem() const;
    TradingBot& getTradingBot() const;
    
    // Helper to convert Transaction enum to string
    std::string transactionTypeToString(Transaction::Type type) const;
};

#endif // BANKINGRADINGFACADE_H