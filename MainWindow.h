// MainWindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QTableWidget>
#include <QTabWidget>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QMessageBox>
#include "BankingSystem.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Authentication slots
    void onLoginClicked();
    void onRegisterClicked();
    void onLogoutClicked();

    // Banking slots
    void onDepositClicked();
    void onWithdrawClicked();
    void onViewTransactionsClicked();
    void onRefreshBalanceClicked();
    void onScheduleDepositClicked();
    void onViewScheduledDepositsClicked();
    void onAdvanceDayClicked();

    // Trading slots
    void onStartBotClicked();
    void onStopBotClicked();
    void onRefreshMarketClicked();
    void onEndSimulationClicked();
    void onResetSimulationClicked();

private:
    // Setup methods
    void setupUI();
    void setupLoginPage();
    void setupBankingPage();
    void setupTradingPage();

    // Helper methods
    void updateUIState();
    void refreshBalance();
    void refreshTransactionHistory();
    void refreshMarketData();
    void refreshPortfolio();
    void refreshTradingStats();
    void refreshTradeHistory();

    // Main layout components
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;

    // Tab widget for Banking and Trading
    QTabWidget *tabWidget;

    // Login/Register UI
    QWidget *loginWidget;
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QPushButton *loginButton;
    QPushButton *registerButton;
    QLabel *statusLabel;

    // Logged-in header
    QWidget *headerWidget;
    QLabel *welcomeLabel;
    QLabel *balanceLabel;
    QPushButton *logoutButton;
    QPushButton *refreshBalanceButton;

    // Banking Page Components
    QWidget *bankingPage;
    QDoubleSpinBox *depositAmount;
    QLineEdit *depositDescription;
    QPushButton *depositButton;

    QDoubleSpinBox *withdrawAmount;
    QLineEdit *withdrawDescription;
    QPushButton *withdrawButton;

    QPushButton *viewTransactionsButton;
    QTextEdit *transactionDisplay;

    // Scheduled Deposits
    QDoubleSpinBox *scheduledDepositAmount;
    QLineEdit *scheduledDepositDescription;
    QSpinBox *scheduledDepositDay;
    QPushButton *scheduleDepositButton;
    QPushButton *viewScheduledButton;

    // Day tracker
    QLabel *currentDayLabel;
    QPushButton *advanceDayButton;

    // Trading Page Components
    QWidget *tradingPage;
    QTableWidget *stockMarketTable;
    QTableWidget *portfolioTable;
    QPushButton *startBotButton;
    QPushButton *stopBotButton;
    QPushButton *refreshMarketButton;
    QPushButton *endSimulationButton;
    QPushButton *resetSimulationButton;
    QLabel *botStatusLabel;
    QLabel *strategyLabel;
    QLabel *marketConditionLabel;
    QLabel *totalProfitLabel;
    QLabel *totalSharesLabel;
    QLabel *daysElapsedLabel;
    QTextEdit *tradeHistoryDisplay;

    // Current day tracker
    int currentDay;
};

#endif // MAINWINDOW_H
