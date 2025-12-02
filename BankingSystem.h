// BankingSystem.h
#ifndef BANKINGSYSTEM_H
#define BANKINGSYSTEM_H

#include <string>
#include <vector>
#include <mutex>
#include <memory>
#include <unordered_map>
#include <ctime>

// --- Transaction Class ---
// Represents a single banking or trading transaction

class Transaction {
public:
    enum Type { DEPOSIT, WITHDRAWAL, STOCK_PURCHASE, STOCK_SALE, FEE };
    
    Transaction(Type type, double amount, const std::string& description, int day)
        : type_(type), amount_(amount), description_(description), day_(day) {
        time_t now = time(0);
        timestamp_ = ctime(&now);
    }
    
    Type getType() const { return type_; }
    double getAmount() const { return amount_; }
    std::string getDescription() const { return description_; }
    int getDay() const { return day_; }
    std::string getTimestamp() const { return timestamp_; }
    
    std::string getTypeString() const {
        switch(type_) {
            case DEPOSIT: return "Deposit";
            case WITHDRAWAL: return "Withdrawal";
            case STOCK_PURCHASE: return "Stock Purchase";
            case STOCK_SALE: return "Stock Sale";
            case FEE: return "Fee";
            default: return "Unknown";
        }
    }
    
private:
    Type type_;
    double amount_;
    std::string description_;
    int day_;
    std::string timestamp_;
};

// --- Scheduled Deposit Class ---
// Represents a future deposit scheduled for a specific day

class ScheduledDeposit {
public:
    ScheduledDeposit(int day, double amount, const std::string& description)
        : scheduledDay_(day), amount_(amount), description_(description), executed_(false) {}
    
    int getScheduledDay() const { return scheduledDay_; }
    double getAmount() const { return amount_; }
    std::string getDescription() const { return description_; }
    bool isExecuted() const { return executed_; }
    void markExecuted() { executed_ = true; }
    
private:
    int scheduledDay_;
    double amount_;
    std::string description_;
    bool executed_;
};

// --- User Account Class ---
// Stores individual user data, balance, and transaction history

class UserAccount {
public:
    UserAccount(const std::string& username, const std::string& password, 
                double initialBalance = 10000.0)
        : username_(username), password_(password), balance_(initialBalance) {}
    
    // Getters
    std::string getUsername() const { return username_; }
    double getBalance() const { return balance_; }
    std::vector<Transaction> getTransactionHistory() const { return transactionHistory_; }
    std::vector<ScheduledDeposit> getScheduledDeposits() const { return scheduledDeposits_; }
    
    // Banking operations
    bool deposit(double amount, const std::string& description, int day) {
        if (amount <= 0) return false;
        
        balance_ += amount;
        transactionHistory_.push_back(Transaction(Transaction::DEPOSIT, amount, description, day));
        return true;
    }
    
    bool withdraw(double amount, const std::string& description, int day) {
        if (amount <= 0 || balance_ < amount) return false;
        
        balance_ -= amount;
        transactionHistory_.push_back(Transaction(Transaction::WITHDRAWAL, amount, description, day));
        return true;
    }
    
    // Password verification
    bool verifyPassword(const std::string& password) const {
        return password_ == password;
    }
    
    // Schedule a deposit for a future day
    void scheduleDeposit(int day, double amount, const std::string& description) {
        scheduledDeposits_.push_back(ScheduledDeposit(day, amount, description));
    }
    
    // Execute scheduled deposits for the current day
    int executeScheduledDeposits(int currentDay) {
        int executed = 0;
        for (auto& scheduled : scheduledDeposits_) {
            if (!scheduled.isExecuted() && scheduled.getScheduledDay() == currentDay) {
                if (deposit(scheduled.getAmount(), scheduled.getDescription(), currentDay)) {
                    scheduled.markExecuted();
                    executed++;
                }
            }
        }
        return executed;
    }
    
    // Reset account for new simulation
    void reset(double initialBalance = 10000.0) {
        balance_ = initialBalance;
        transactionHistory_.clear();
        scheduledDeposits_.clear();
    }
    
private:
    std::string username_;
    std::string password_;
    double balance_;
    std::vector<Transaction> transactionHistory_;
    std::vector<ScheduledDeposit> scheduledDeposits_;
};

// --- Banking System (Singleton Pattern) ---
// Central system managing all user accounts and banking operations

class BankingSystem {
public:
    // Singleton getInstance method
    static BankingSystem& getInstance() {
        static BankingSystem instance;  // Created only once, thread-safe in C++11+
        return instance;
    }
    
    // Delete copy constructor and assignment operator
    BankingSystem(const BankingSystem&) = delete;
    BankingSystem& operator=(const BankingSystem&) = delete;
    
    // User Management
    
    // Register a new user
    bool registerUser(const std::string& username, const std::string& password, 
                     double initialBalance = 10000.0) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        // Check if username already exists
        if (accounts_.find(username) != accounts_.end()) {
            return false;  // Username taken
        }
        
        // Create new account
        accounts_[username] = std::make_unique<UserAccount>(username, password, initialBalance);
        return true;
    }
    
    // Login
    bool login(const std::string& username, const std::string& password) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        // Check if user exists
        auto it = accounts_.find(username);
        if (it == accounts_.end()) {
            return false;  // User not found
        }
        
        // Verify password
        if (!it->second->verifyPassword(password)) {
            return false;  // Wrong password
        }
        
        // Set as current user
        currentUser_ = username;
        return true;
    }
    
    // Logout
    void logout() {
        std::lock_guard<std::mutex> lock(mutex_);
        currentUser_ = "";
    }
    
    // Check if someone is logged in
    bool isLoggedIn() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return !currentUser_.empty();
    }
    
    // Get current logged-in username
    std::string getCurrentUser() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return currentUser_;
    }
    
    // Banking Operations (for currently logged-in user)
    
    // Deposit money
    bool deposit(double amount, const std::string& description, int currentDay) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        if (currentUser_.empty()) return false;  // Not logged in
        
        auto it = accounts_.find(currentUser_);
        if (it == accounts_.end()) return false;
        
        return it->second->deposit(amount, description, currentDay);
    }
    
    // Withdraw money
    bool withdraw(double amount, const std::string& description, int currentDay) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        if (currentUser_.empty()) return false;  // Not logged in
        
        auto it = accounts_.find(currentUser_);
        if (it == accounts_.end()) return false;
        
        return it->second->withdraw(amount, description, currentDay);
    }
    
    // Get balance of current user
    double getBalance() const {
        std::lock_guard<std::mutex> lock(mutex_);
        
        if (currentUser_.empty()) return 0.0;
        
        auto it = accounts_.find(currentUser_);
        if (it == accounts_.end()) return 0.0;
        
        return it->second->getBalance();
    }
    
    // Get transaction history of current user
    std::vector<Transaction> getTransactionHistory() const {
        std::lock_guard<std::mutex> lock(mutex_);
        
        if (currentUser_.empty()) return {};
        
        auto it = accounts_.find(currentUser_);
        if (it == accounts_.end()) return {};
        
        return it->second->getTransactionHistory();
    }
    
    // Reset current user's account
    void resetCurrentAccount(double initialBalance = 10000.0) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        if (currentUser_.empty()) return;
        
        auto it = accounts_.find(currentUser_);
        if (it != accounts_.end()) {
            it->second->reset(initialBalance);
        }
    }
    
    // Schedule a deposit for current user
    bool scheduleDeposit(int day, double amount, const std::string& description) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        if (currentUser_.empty()) return false;
        
        auto it = accounts_.find(currentUser_);
        if (it == accounts_.end()) return false;
        
        it->second->scheduleDeposit(day, amount, description);
        return true;
    }
    
    // Execute scheduled deposits for current day (for current user)
    int executeScheduledDeposits(int currentDay) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        if (currentUser_.empty()) return 0;
        
        auto it = accounts_.find(currentUser_);
        if (it == accounts_.end()) return 0;
        
        return it->second->executeScheduledDeposits(currentDay);
    }
    
    // Get scheduled deposits for current user
    std::vector<ScheduledDeposit> getScheduledDeposits() const {
        std::lock_guard<std::mutex> lock(mutex_);
        
        if (currentUser_.empty()) return {};
        
        auto it = accounts_.find(currentUser_);
        if (it == accounts_.end()) return {};
        
        return it->second->getScheduledDeposits();
    }
    
private:
    // Private constructor for singleton
    BankingSystem() : currentUser_("") {}
    
    // Private destructor
    ~BankingSystem() = default;
    
    // Member variables
    std::unordered_map<std::string, std::unique_ptr<UserAccount>> accounts_;
    std::string currentUser_;
    mutable std::mutex mutex_;  // For thread safety
};

#endif // BANKINGSYSTEM_H
