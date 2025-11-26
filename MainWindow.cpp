// MainWindow.cpp
#include "MainWindow.h"
#include <QGridLayout>
#include <QHeaderView>
#include <sstream>
#include <iomanip>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentDay(1) {
    setupUI();
    updateUIState();
}

MainWindow::~MainWindow() {
}

void MainWindow::setupUI() {
    // Create central widget
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    mainLayout = new QVBoxLayout(centralWidget);
    
    // Setup all pages
    setupLoginPage();
    setupBankingPage();
    setupTradingPage();
    
    // Create header (initially hidden)
    headerWidget = new QWidget();
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    
    welcomeLabel = new QLabel("Welcome!");
    welcomeLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    
    balanceLabel = new QLabel("Balance: $0.00");
    balanceLabel->setStyleSheet("font-size: 14px; color: green;");
    
    refreshBalanceButton = new QPushButton("Refresh Balance");
    connect(refreshBalanceButton, &QPushButton::clicked, this, &MainWindow::onRefreshBalanceClicked);
    
    logoutButton = new QPushButton("Logout");
    connect(logoutButton, &QPushButton::clicked, this, &MainWindow::onLogoutClicked);
    
    headerLayout->addWidget(welcomeLabel);
    headerLayout->addWidget(balanceLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(refreshBalanceButton);
    headerLayout->addWidget(logoutButton);
    
    headerWidget->setVisible(false);
    mainLayout->addWidget(headerWidget);
    
    // Create tab widget for Banking and Trading
    tabWidget = new QTabWidget();
    tabWidget->addTab(bankingPage, "Banking");
    tabWidget->addTab(tradingPage, "Trading");
    tabWidget->setVisible(false);
    
    mainLayout->addWidget(loginWidget);
    mainLayout->addWidget(tabWidget);
    
    setWindowTitle("Banking & Trading System");
    resize(900, 700);
}

void MainWindow::setupLoginPage() {
    loginWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(loginWidget);
    
    // Title
    QLabel *titleLabel = new QLabel("Banking System");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
    titleLabel->setAlignment(Qt::AlignCenter);
    
    // Login form
    QGroupBox *loginBox = new QGroupBox("Login / Register");
    QGridLayout *formLayout = new QGridLayout();
    
    formLayout->addWidget(new QLabel("Username:"), 0, 0);
    usernameInput = new QLineEdit();
    formLayout->addWidget(usernameInput, 0, 1);
    
    formLayout->addWidget(new QLabel("Password:"), 1, 0);
    passwordInput = new QLineEdit();
    passwordInput->setEchoMode(QLineEdit::Password);
    formLayout->addWidget(passwordInput, 1, 1);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    loginButton = new QPushButton("Login");
    registerButton = new QPushButton("Register");
    
    connect(loginButton, &QPushButton::clicked, this, &MainWindow::onLoginClicked);
    connect(registerButton, &QPushButton::clicked, this, &MainWindow::onRegisterClicked);
    
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(registerButton);
    
    formLayout->addLayout(buttonLayout, 2, 0, 1, 2);
    
    loginBox->setLayout(formLayout);
    
    statusLabel = new QLabel("");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("color: red;");
    
    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addSpacing(20);
    layout->addWidget(loginBox);
    layout->addWidget(statusLabel);
    layout->addStretch();
}

void MainWindow::setupBankingPage() {
    bankingPage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(bankingPage);
    
    // Day Tracker Section
    QGroupBox *dayBox = new QGroupBox("Simulation");
    QHBoxLayout *dayLayout = new QHBoxLayout();
    
    currentDayLabel = new QLabel(QString("Current Day: %1").arg(currentDay));
    currentDayLabel->setStyleSheet("font-size: 14px; font-weight: bold;");
    
    advanceDayButton = new QPushButton("Advance Day");
    advanceDayButton->setToolTip("Move to the next day and execute scheduled deposits");
    connect(advanceDayButton, &QPushButton::clicked, this, &MainWindow::onAdvanceDayClicked);
    
    dayLayout->addWidget(currentDayLabel);
    dayLayout->addStretch();
    dayLayout->addWidget(advanceDayButton);
    dayBox->setLayout(dayLayout);
    
    layout->addWidget(dayBox);
    
    // Deposit Section
    QGroupBox *depositBox = new QGroupBox("Deposit");
    QGridLayout *depositLayout = new QGridLayout();
    
    depositLayout->addWidget(new QLabel("Amount:"), 0, 0);
    depositAmount = new QDoubleSpinBox();
    depositAmount->setRange(0.01, 1000000.00);
    depositAmount->setDecimals(2);
    depositAmount->setValue(100.00);
    depositAmount->setPrefix("$");
    depositLayout->addWidget(depositAmount, 0, 1);
    
    depositLayout->addWidget(new QLabel("Description:"), 1, 0);
    depositDescription = new QLineEdit("Deposit");
    depositLayout->addWidget(depositDescription, 1, 1);
    
    depositButton = new QPushButton("Deposit Now");
    depositButton->setToolTip("Deposit immediately on current day");
    connect(depositButton, &QPushButton::clicked, this, &MainWindow::onDepositClicked);
    depositLayout->addWidget(depositButton, 2, 0, 1, 2);
    
    depositBox->setLayout(depositLayout);
    
    // Withdraw Section
    QGroupBox *withdrawBox = new QGroupBox("Withdraw");
    QGridLayout *withdrawLayout = new QGridLayout();
    
    withdrawLayout->addWidget(new QLabel("Amount:"), 0, 0);
    withdrawAmount = new QDoubleSpinBox();
    withdrawAmount->setRange(0.01, 1000000.00);
    withdrawAmount->setDecimals(2);
    withdrawAmount->setValue(50.00);
    withdrawAmount->setPrefix("$");
    withdrawLayout->addWidget(withdrawAmount, 0, 1);
    
    withdrawLayout->addWidget(new QLabel("Description:"), 1, 0);
    withdrawDescription = new QLineEdit("Withdrawal");
    withdrawLayout->addWidget(withdrawDescription, 1, 1);
    
    withdrawButton = new QPushButton("Withdraw Now");
    withdrawButton->setToolTip("Withdraw immediately on current day");
    connect(withdrawButton, &QPushButton::clicked, this, &MainWindow::onWithdrawClicked);
    withdrawLayout->addWidget(withdrawButton, 2, 0, 1, 2);
    
    withdrawBox->setLayout(withdrawLayout);
    
    // Scheduled Deposit Section
    QGroupBox *scheduledBox = new QGroupBox("Schedule Future Deposit");
    QGridLayout *scheduledLayout = new QGridLayout();
    
    scheduledLayout->addWidget(new QLabel("Amount:"), 0, 0);
    scheduledDepositAmount = new QDoubleSpinBox();
    scheduledDepositAmount->setRange(0.01, 1000000.00);
    scheduledDepositAmount->setDecimals(2);
    scheduledDepositAmount->setValue(100.00);
    scheduledDepositAmount->setPrefix("$");
    scheduledLayout->addWidget(scheduledDepositAmount, 0, 1);
    
    scheduledLayout->addWidget(new QLabel("Description:"), 1, 0);
    scheduledDepositDescription = new QLineEdit("Scheduled Deposit");
    scheduledLayout->addWidget(scheduledDepositDescription, 1, 1);
    
    scheduledLayout->addWidget(new QLabel("Execute on Day:"), 2, 0);
    scheduledDepositDay = new QSpinBox();
    scheduledDepositDay->setRange(2, 365);
    scheduledDepositDay->setValue(5);
    scheduledLayout->addWidget(scheduledDepositDay, 2, 1);
    
    QHBoxLayout *scheduledButtonLayout = new QHBoxLayout();
    scheduleDepositButton = new QPushButton("Schedule Deposit");
    connect(scheduleDepositButton, &QPushButton::clicked, this, &MainWindow::onScheduleDepositClicked);
    
    viewScheduledButton = new QPushButton("View Scheduled");
    connect(viewScheduledButton, &QPushButton::clicked, this, &MainWindow::onViewScheduledDepositsClicked);
    
    scheduledButtonLayout->addWidget(scheduleDepositButton);
    scheduledButtonLayout->addWidget(viewScheduledButton);
    scheduledLayout->addLayout(scheduledButtonLayout, 3, 0, 1, 2);
    
    scheduledBox->setLayout(scheduledLayout);
    
    // Transaction History Section
    QGroupBox *historyBox = new QGroupBox("Transaction History");
    QVBoxLayout *historyLayout = new QVBoxLayout();
    
    viewTransactionsButton = new QPushButton("View Transactions");
    connect(viewTransactionsButton, &QPushButton::clicked, this, &MainWindow::onViewTransactionsClicked);
    
    transactionDisplay = new QTextEdit();
    transactionDisplay->setReadOnly(true);
    transactionDisplay->setMaximumHeight(200);
    
    historyLayout->addWidget(viewTransactionsButton);
    historyLayout->addWidget(transactionDisplay);
    
    historyBox->setLayout(historyLayout);
    
    // Add to main layout
    QHBoxLayout *operationsLayout = new QHBoxLayout();
    operationsLayout->addWidget(depositBox);
    operationsLayout->addWidget(withdrawBox);
    
    layout->addLayout(operationsLayout);
    layout->addWidget(scheduledBox);
    layout->addWidget(historyBox);
    layout->addStretch();
}

void MainWindow::setupTradingPage() {
    tradingPage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tradingPage);
    
    // Stock Market Section
    QGroupBox *marketBox = new QGroupBox("Stock Market");
    QVBoxLayout *marketLayout = new QVBoxLayout();
    
    refreshMarketButton = new QPushButton("Refresh Market Data");
    marketLayout->addWidget(refreshMarketButton);
    
    stockMarketTable = new QTableWidget();
    stockMarketTable->setColumnCount(4);
    stockMarketTable->setHorizontalHeaderLabels({"Symbol", "Price", "Change %", "Trend"});
    stockMarketTable->horizontalHeader()->setStretchLastSection(true);
    stockMarketTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    marketLayout->addWidget(stockMarketTable);
    
    marketBox->setLayout(marketLayout);
    
    // Trading Bot Control Section
    QGroupBox *botBox = new QGroupBox("Trading Bot");
    QVBoxLayout *botLayout = new QVBoxLayout();
    
    botStatusLabel = new QLabel("Bot Status: INACTIVE");
    botStatusLabel->setStyleSheet("font-weight: bold; color: red;");
    botLayout->addWidget(botStatusLabel);
    
    QHBoxLayout *botButtonLayout = new QHBoxLayout();
    startBotButton = new QPushButton("Start Bot");
    stopBotButton = new QPushButton("Stop Bot");
    botButtonLayout->addWidget(startBotButton);
    botButtonLayout->addWidget(stopBotButton);
    botLayout->addLayout(botButtonLayout);
    
    botBox->setLayout(botLayout);
    
    // Portfolio Section
    QGroupBox *portfolioBox = new QGroupBox("Portfolio");
    QVBoxLayout *portfolioLayout = new QVBoxLayout();
    
    portfolioTable = new QTableWidget();
    portfolioTable->setColumnCount(4);
    portfolioTable->setHorizontalHeaderLabels({"Symbol", "Shares", "Avg Price", "Current Value"});
    portfolioTable->horizontalHeader()->setStretchLastSection(true);
    portfolioTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    portfolioLayout->addWidget(portfolioTable);
    
    portfolioBox->setLayout(portfolioLayout);
    
    // Performance Stats Section
    QGroupBox *statsBox = new QGroupBox("Performance");
    QGridLayout *statsLayout = new QGridLayout();
    
    statsLayout->addWidget(new QLabel("Total Profit:"), 0, 0);
    totalProfitLabel = new QLabel("$0.00");
    totalProfitLabel->setStyleSheet("font-weight: bold; color: green;");
    statsLayout->addWidget(totalProfitLabel, 0, 1);
    
    statsLayout->addWidget(new QLabel("Total Shares Owned:"), 1, 0);
    totalSharesLabel = new QLabel("0");
    statsLayout->addWidget(totalSharesLabel, 1, 1);
    
    statsLayout->addWidget(new QLabel("Days Elapsed:"), 2, 0);
    daysElapsedLabel = new QLabel("0");
    statsLayout->addWidget(daysElapsedLabel, 2, 1);
    
    statsBox->setLayout(statsLayout);
    
    // Add all sections to main layout
    layout->addWidget(marketBox);
    layout->addWidget(botBox);
    layout->addWidget(portfolioBox);
    layout->addWidget(statsBox);
    layout->addStretch();
}

// ============================================================================
// AUTHENTICATION SLOTS
// ============================================================================

void MainWindow::onLoginClicked() {
    QString username = usernameInput->text();
    QString password = passwordInput->text();
    
    if (username.isEmpty() || password.isEmpty()) {
        statusLabel->setText("Please enter username and password");
        statusLabel->setStyleSheet("color: red;");
        return;
    }
    
    BankingSystem& bank = BankingSystem::getInstance();
    
    if (bank.login(username.toStdString(), password.toStdString())) {
        statusLabel->setText("Login successful!");
        statusLabel->setStyleSheet("color: green;");
        updateUIState();
        refreshBalance();
    } else {
        statusLabel->setText("Invalid username or password");
        statusLabel->setStyleSheet("color: red;");
    }
}

void MainWindow::onRegisterClicked() {
    QString username = usernameInput->text();
    QString password = passwordInput->text();
    
    if (username.isEmpty() || password.isEmpty()) {
        statusLabel->setText("Please enter username and password");
        statusLabel->setStyleSheet("color: red;");
        return;
    }
    
    BankingSystem& bank = BankingSystem::getInstance();
    
    if (bank.registerUser(username.toStdString(), password.toStdString(), 10000.0)) {
        statusLabel->setText("Registration successful! Please login.");
        statusLabel->setStyleSheet("color: green;");
        passwordInput->clear();
    } else {
        statusLabel->setText("Username already exists");
        statusLabel->setStyleSheet("color: red;");
    }
}

void MainWindow::onLogoutClicked() {
    BankingSystem& bank = BankingSystem::getInstance();
    bank.logout();
    
    usernameInput->clear();
    passwordInput->clear();
    statusLabel->clear();
    transactionDisplay->clear();
    
    updateUIState();
}

// ============================================================================
// BANKING SLOTS
// ============================================================================

void MainWindow::onDepositClicked() {
    BankingSystem& bank = BankingSystem::getInstance();
    
    double amount = depositAmount->value();
    QString description = depositDescription->text();
    
    if (bank.deposit(amount, description.toStdString(), currentDay)) {
        QMessageBox::information(this, "Success", 
            QString("Deposited $%1 on day %2!").arg(amount, 0, 'f', 2).arg(currentDay));
        refreshBalance();
    } else {
        QMessageBox::warning(this, "Error", "Deposit failed!");
    }
}

void MainWindow::onWithdrawClicked() {
    BankingSystem& bank = BankingSystem::getInstance();
    
    double amount = withdrawAmount->value();
    QString description = withdrawDescription->text();
    
    if (bank.withdraw(amount, description.toStdString(), currentDay)) {
        QMessageBox::information(this, "Success", 
            QString("Withdrew $%1 on day %2!").arg(amount, 0, 'f', 2).arg(currentDay));
        refreshBalance();
    } else {
        QMessageBox::warning(this, "Error", "Withdrawal failed! Insufficient balance or invalid amount.");
    }
}

void MainWindow::onViewTransactionsClicked() {
    refreshTransactionHistory();
}

void MainWindow::onRefreshBalanceClicked() {
    refreshBalance();
}

void MainWindow::onScheduleDepositClicked() {
    BankingSystem& bank = BankingSystem::getInstance();
    
    double amount = scheduledDepositAmount->value();
    QString description = scheduledDepositDescription->text();
    int day = scheduledDepositDay->value();
    
    if (day <= currentDay) {
        QMessageBox::warning(this, "Error", 
            QString("Scheduled day must be in the future (current day is %1)").arg(currentDay));
        return;
    }
    
    if (bank.scheduleDeposit(day, amount, description.toStdString())) {
        QMessageBox::information(this, "Success", 
            QString("Scheduled $%1 deposit for day %2").arg(amount, 0, 'f', 2).arg(day));
    } else {
        QMessageBox::warning(this, "Error", "Failed to schedule deposit!");
    }
}

void MainWindow::onViewScheduledDepositsClicked() {
    BankingSystem& bank = BankingSystem::getInstance();
    std::vector<ScheduledDeposit> scheduled = bank.getScheduledDeposits();
    
    QString message;
    if (scheduled.empty()) {
        message = "No scheduled deposits.";
    } else {
        message = QString("Scheduled Deposits (Current Day: %1):\n\n").arg(currentDay);
        for (const auto& dep : scheduled) {
            QString status = dep.isExecuted() ? "✓ EXECUTED" : "⏳ PENDING";
            message += QString("Day %1: $%2 - %3 [%4]\n")
                        .arg(dep.getScheduledDay())
                        .arg(dep.getAmount(), 0, 'f', 2)
                        .arg(QString::fromStdString(dep.getDescription()))
                        .arg(status);
        }
    }
    
    QMessageBox::information(this, "Scheduled Deposits", message);
}

void MainWindow::onAdvanceDayClicked() {
    BankingSystem& bank = BankingSystem::getInstance();
    
    currentDay++;
    currentDayLabel->setText(QString("Current Day: %1").arg(currentDay));
    
    // Execute any scheduled deposits for this day
    int executed = bank.executeScheduledDeposits(currentDay);
    
    if (executed > 0) {
        QMessageBox::information(this, "Deposits Executed", 
            QString("Executed %1 scheduled deposit(s) on day %2!").arg(executed).arg(currentDay));
        refreshBalance();
        refreshTransactionHistory();
    }
}

// ============================================================================
// HELPER METHODS
// ============================================================================

void MainWindow::updateUIState() {
    BankingSystem& bank = BankingSystem::getInstance();
    bool loggedIn = bank.isLoggedIn();
    
    loginWidget->setVisible(!loggedIn);
    headerWidget->setVisible(loggedIn);
    tabWidget->setVisible(loggedIn);
    
    if (loggedIn) {
        std::string username = bank.getCurrentUser();
        welcomeLabel->setText(QString("Welcome, %1!").arg(QString::fromStdString(username)));
    }
}

void MainWindow::refreshBalance() {
    BankingSystem& bank = BankingSystem::getInstance();
    double balance = bank.getBalance();
    
    balanceLabel->setText(QString("Balance: $%1").arg(balance, 0, 'f', 2));
}

void MainWindow::refreshTransactionHistory() {
    BankingSystem& bank = BankingSystem::getInstance();
    std::vector<Transaction> transactions = bank.getTransactionHistory();
    
    transactionDisplay->clear();
    
    if (transactions.empty()) {
        transactionDisplay->setPlainText("No transactions yet.");
        return;
    }
    
    QString history;
    history += QString("=== TRANSACTIONS (Total: %1) ===\n\n").arg(transactions.size());
    
    for (const auto& trans : transactions) {
        QString type = QString::fromStdString(trans.getTypeString());
        int day = trans.getDay();
        double amount = trans.getAmount();
        QString description = QString::fromStdString(trans.getDescription());
        
        history += QString("%1 | Day %2 | $%3 | %4\n")
                    .arg(type, -15)
                    .arg(day, 3)
                    .arg(amount, 10, 'f', 2)
                    .arg(description);
    }
    
    transactionDisplay->setPlainText(history);
}
