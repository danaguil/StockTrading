// MainWindow.cpp
#include "MainWindow.h"
#include "TradingBot.h"
#include <QGridLayout>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentDay(1) {
    setupUI();
    updateUIState();
}

MainWindow::~MainWindow() {
}

void MainWindow::setupUI() {
    // Apply professional stylesheet
    setStyleSheet(
        "QMainWindow {"
        "   background-color: #2b2b2b;"
        "}"
        "QWidget {"
        "   font-family: 'Segoe UI', Arial, sans-serif;"
        "   font-size: 13px;"
        "}"
        "QPushButton {"
        "   background-color: #0078d4;"
        "   color: white;"
        "   border: none;"
        "   padding: 8px 16px;"
        "   border-radius: 4px;"
        "   font-weight: bold;"
        "   min-width: 80px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #106ebe;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #005a9e;"
        "}"
        "QPushButton:disabled {"
        "   background-color: #cccccc;"
        "   color: #666666;"
        "}"
        "QLineEdit {"
        "   padding: 6px;"
        "   border: 2px solid #555555;"
        "   border-radius: 4px;"
        "   background-color: #3c3c3c;"
        "   color: #ffffff;"
        "}"
        "QLineEdit:focus {"
        "   border: 2px solid #0078d4;"
        "}"
        "QGroupBox {"
        "   font-weight: bold;"
        "   border: 2px solid #555555;"
        "   border-radius: 6px;"
        "   margin-top: 12px;"
        "   padding-top: 16px;"
        "   background-color: #353535;"
        "   color: #ffffff;"
        "}"
        "QGroupBox::title {"
        "   subcontrol-origin: margin;"
        "   subcontrol-position: top left;"
        "   padding: 4px 8px;"
        "   color: #0078d4;"
        "   font-size: 14px;"
        "}"
        "QTableWidget {"
        "   border: 1px solid #555555;"
        "   border-radius: 4px;"
        "   background-color: #3c3c3c;"
        "   gridline-color: #555555;"
        "   selection-background-color: #0078d4;"
        "   color: #ffffff;"
        "}"
        "QTableWidget::item {"
        "   padding: 4px;"
        "}"
        "QHeaderView::section {"
        "   background-color: #2b2b2b;"
        "   padding: 6px;"
        "   border: none;"
        "   border-bottom: 2px solid #0078d4;"
        "   font-weight: bold;"
        "   color: #ffffff;"
        "}"
        "QTabWidget::pane {"
        "   border: 2px solid #555555;"
        "   border-radius: 4px;"
        "   background-color: #353535;"
        "   top: -1px;"
        "}"
        "QTabBar::tab {"
        "   background-color: #2b2b2b;"
        "   color: #cccccc;"
        "   padding: 10px 20px;"
        "   margin-right: 2px;"
        "   border-top-left-radius: 4px;"
        "   border-top-right-radius: 4px;"
        "   font-weight: bold;"
        "}"
        "QTabBar::tab:selected {"
        "   background-color: #353535;"
        "   color: #0078d4;"
        "   border-bottom: 3px solid #0078d4;"
        "}"
        "QTabBar::tab:hover:!selected {"
        "   background-color: #404040;"
        "}"
        "QTextEdit {"
        "   border: 1px solid #555555;"
        "   border-radius: 4px;"
        "   background-color: #3c3c3c;"
        "   padding: 4px;"
        "   color: #ffffff;"
        "}"
        "QLabel {"
        "   color: #ffffff;"
        "}"
        "QDoubleSpinBox {"
        "   padding: 6px;"
        "   border: 2px solid #555555;"
        "   border-radius: 4px;"
        "   background-color: #3c3c3c;"
        "   color: #ffffff;"
        "}"
        "QDoubleSpinBox:focus {"
        "   border: 2px solid #0078d4;"
        "}"
    );

    centralWidget = new QWidget(this);
    centralWidget->setStyleSheet("background-color: #2b2b2b;");
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    setupLoginPage();
    setupBankingPage();
    setupTradingPage();

    // Header (initially hidden)
    headerWidget = new QWidget();
    headerWidget->setStyleSheet(
        "background-color: #353535;"
        "border-bottom: 2px solid #0078d4;"
        "padding: 10px;"
    );
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);

    welcomeLabel = new QLabel("Welcome!");
    welcomeLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #0078d4;");

    balanceLabel = new QLabel("Balance: $0.00");
    balanceLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #107c10;");

    logoutButton = new QPushButton("Logout");
    logoutButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #d13438;"
        "}"
        "QPushButton:hover {"
        "   background-color: #a52a2a;"
        "}"
    );
    connect(logoutButton, &QPushButton::clicked, this, &MainWindow::onLogoutClicked);

    headerLayout->addWidget(welcomeLabel);
    headerLayout->addWidget(balanceLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(logoutButton);

    headerWidget->setVisible(false);
    mainLayout->addWidget(headerWidget);

    tabWidget = new QTabWidget();
    tabWidget->addTab(bankingPage, "Banking");
    tabWidget->addTab(tradingPage, "Trading");
    tabWidget->setVisible(false);

    mainLayout->addWidget(loginWidget);
    mainLayout->addWidget(tabWidget);

    setWindowTitle("StockTrading Application");
    resize(900, 700);
}

void MainWindow::setupLoginPage() {
    loginWidget = new QWidget();
    loginWidget->setStyleSheet("background-color: #2b2b2b;");
    QVBoxLayout *layout = new QVBoxLayout(loginWidget);
    layout->setSpacing(20);
    layout->setContentsMargins(50, 50, 50, 50);

    QLabel *titleLabel = new QLabel("StockTrading Application");
    titleLabel->setStyleSheet(
        "font-size: 32px;"
        "font-weight: bold;"
        "color: #ffffff;"
        "padding: 20px;"
    );
    titleLabel->setAlignment(Qt::AlignCenter);

    QGroupBox *loginBox = new QGroupBox("Login / Register");
    loginBox->setStyleSheet(
        "QGroupBox {"
        "   font-size: 16px;"
        "   padding: 20px;"
        "   margin-top: 20px;"
        "}"
        "QGroupBox::title {"
        "   font-size: 18px;"
        "   color: #ffffff;"
        "}"
    );
    QGridLayout *formLayout = new QGridLayout();
    formLayout->setSpacing(15);
    formLayout->setContentsMargins(20, 30, 20, 20);

    QLabel *userLabel = new QLabel("Username:");
    userLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #ffffff;");
    formLayout->addWidget(userLabel, 0, 0);
    usernameInput = new QLineEdit();
    usernameInput->setPlaceholderText("Enter your username");
    usernameInput->setMinimumHeight(35);
    formLayout->addWidget(usernameInput, 0, 1);

    QLabel *passLabel = new QLabel("Password:");
    passLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #ffffff;");
    formLayout->addWidget(passLabel, 1, 0);
    passwordInput = new QLineEdit();
    passwordInput->setPlaceholderText("Enter your password");
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setMinimumHeight(35);
    formLayout->addWidget(passwordInput, 1, 1);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);
    loginButton = new QPushButton("Login");
    loginButton->setMinimumHeight(40);
    loginButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #107c10;"
        "   font-size: 14px;"
        "   min-width: 120px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #0e6b0e;"
        "}"
    );
    registerButton = new QPushButton("Register");
    registerButton->setMinimumHeight(40);
    registerButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #107c10;"
        "   font-size: 14px;"
        "   min-width: 120px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #0e6b0e;"
        "}"
    );

    connect(loginButton, &QPushButton::clicked, this, &MainWindow::onLoginClicked);
    connect(registerButton, &QPushButton::clicked, this, &MainWindow::onRegisterClicked);

    buttonLayout->addStretch();
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(registerButton);
    buttonLayout->addStretch();

    formLayout->addLayout(buttonLayout, 2, 0, 1, 2);
    loginBox->setLayout(formLayout);

    statusLabel = new QLabel("");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("color: #d13438; font-size: 14px; font-weight: bold;");

    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addWidget(loginBox);
    layout->addWidget(statusLabel);
    layout->addStretch();
}

void MainWindow::setupBankingPage() {
    // Create a scroll area for the banking page
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    
    QWidget *scrollWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(scrollWidget);
    layout->setSpacing(15);
    layout->setContentsMargins(15, 15, 15, 15);

    // Deposit
    QGroupBox *depositBox = new QGroupBox("💰 Deposit");
    QGridLayout *depositLayout = new QGridLayout();
    depositLayout->setSpacing(12);

    QLabel *depAmtLabel = new QLabel("Amount:");
    depAmtLabel->setStyleSheet("font-weight: bold; color: #ffffff;");
    depositLayout->addWidget(depAmtLabel, 0, 0);
    depositAmount = new QDoubleSpinBox();
    depositAmount->setRange(0.01, 1000000.00);
    depositAmount->setDecimals(2);
    depositAmount->setValue(100.00);
    depositAmount->setPrefix("$");
    depositAmount->setMinimumHeight(30);
    depositLayout->addWidget(depositAmount, 0, 1);

    QLabel *depDescLabel = new QLabel("Description:");
    depDescLabel->setStyleSheet("font-weight: bold; color: #ffffff;");
    depositLayout->addWidget(depDescLabel, 1, 0);
    depositDescription = new QLineEdit("Deposit");
    depositDescription->setMinimumHeight(30);
    depositLayout->addWidget(depositDescription, 1, 1);

    depositButton = new QPushButton("💵 Deposit Now");
    depositButton->setMinimumHeight(35);
    depositButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #107c10;"
        "   font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #0e6b0e;"
        "}"
    );
    connect(depositButton, &QPushButton::clicked, this, &MainWindow::onDepositClicked);
    depositLayout->addWidget(depositButton, 2, 0, 1, 2);
    depositBox->setLayout(depositLayout);

    // Withdraw
    QGroupBox *withdrawBox = new QGroupBox("💸 Withdraw");
    QGridLayout *withdrawLayout = new QGridLayout();
    withdrawLayout->setSpacing(12);

    QLabel *withAmtLabel = new QLabel("Amount:");
    withAmtLabel->setStyleSheet("font-weight: bold; color: #ffffff;");
    withdrawLayout->addWidget(withAmtLabel, 0, 0);
    withdrawAmount = new QDoubleSpinBox();
    withdrawAmount->setRange(0.01, 1000000.00);
    withdrawAmount->setDecimals(2);
    withdrawAmount->setValue(50.00);
    withdrawAmount->setPrefix("$");
    withdrawAmount->setMinimumHeight(30);
    withdrawLayout->addWidget(withdrawAmount, 0, 1);

    QLabel *withDescLabel = new QLabel("Description:");
    withDescLabel->setStyleSheet("font-weight: bold; color: #ffffff;");
    withdrawLayout->addWidget(withDescLabel, 1, 0);
    withdrawDescription = new QLineEdit("Withdrawal");
    withdrawDescription->setMinimumHeight(30);
    withdrawLayout->addWidget(withdrawDescription, 1, 1);

    withdrawButton = new QPushButton("💳 Withdraw Now");
    withdrawButton->setMinimumHeight(35);
    withdrawButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #d13438;"
        "}"
        "QPushButton:hover {"
        "   background-color: #a52a2a;"
        "}"
    );
    connect(withdrawButton, &QPushButton::clicked, this, &MainWindow::onWithdrawClicked);
    withdrawLayout->addWidget(withdrawButton, 2, 0, 1, 2);
    withdrawBox->setLayout(withdrawLayout);

    // Scheduled Deposits
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
    scheduleDepositButton->setMinimumHeight(35);
    scheduleDepositButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #8a8a8a;"
        "   font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #6a6a6a;"
        "}"
    );
    connect(scheduleDepositButton, &QPushButton::clicked, this, &MainWindow::onScheduleDepositClicked);

    viewScheduledButton = new QPushButton("View Scheduled");
    viewScheduledButton->setMinimumHeight(35);
    viewScheduledButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #8a8a8a;"
        "   font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #6a6a6a;"
        "}"
    );
    connect(viewScheduledButton, &QPushButton::clicked, this, &MainWindow::onViewScheduledDepositsClicked);

    scheduledButtonLayout->addWidget(scheduleDepositButton);
    scheduledButtonLayout->addWidget(viewScheduledButton);
    scheduledLayout->addLayout(scheduledButtonLayout, 3, 0, 1, 2);
    scheduledBox->setLayout(scheduledLayout);

    // Display for the transaction History
    QGroupBox *historyBox = new QGroupBox("Transaction History");
    QVBoxLayout *historyLayout = new QVBoxLayout();

    viewTransactionsButton = new QPushButton("View Transactions");
    viewTransactionsButton->setMinimumHeight(35);
    viewTransactionsButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #8a8a8a;"
        "   font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #6a6a6a;"
        "}"
    );
    connect(viewTransactionsButton, &QPushButton::clicked, this, &MainWindow::onViewTransactionsClicked);

    transactionDisplay = new QTextEdit();
    transactionDisplay->setReadOnly(true);
    transactionDisplay->setMaximumHeight(200);

    historyLayout->addWidget(viewTransactionsButton);
    historyLayout->addWidget(transactionDisplay);
    historyBox->setLayout(historyLayout);

    QHBoxLayout *operationsLayout = new QHBoxLayout();
    operationsLayout->addWidget(depositBox);
    operationsLayout->addWidget(withdrawBox);

    layout->addLayout(operationsLayout);
    layout->addWidget(scheduledBox);
    layout->addWidget(historyBox);
    layout->addStretch();
    
    // Set the scroll widget and create the banking page with scroll area
    scrollArea->setWidget(scrollWidget);
    bankingPage = scrollArea;
}

void MainWindow::setupTradingPage() {
    // Create a scroll area for the trading page
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    
    QWidget *scrollWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(scrollWidget);
    layout->setSpacing(12);
    layout->setContentsMargins(12, 12, 12, 12);

    // Keeps a count of the days elapsed in the sim
    QGroupBox *dayBox = new QGroupBox("📅 Simulation Day");
    QHBoxLayout *dayLayout = new QHBoxLayout();

    currentDayLabel = new QLabel(QString("Current Day: %1").arg(currentDay));
    currentDayLabel->setStyleSheet(
        "font-size: 16px;"
        "font-weight: bold;"
        "color: #ffffff;"
    );

    advanceDayButton = new QPushButton("⏭️ Advance Day");
    advanceDayButton->setMinimumHeight(35);
    advanceDayButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #8a8a8a;"
        "   font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #6a6a6a;"
        "}"
    );

    connect(advanceDayButton, &QPushButton::clicked, this, &MainWindow::onAdvanceDayClicked);

    dayLayout->addWidget(currentDayLabel);
    dayLayout->addStretch();
    dayLayout->addWidget(advanceDayButton);
    dayBox->setLayout(dayLayout);
    layout->addWidget(dayBox);

    // Displays the Stock Market stats
    QGroupBox *marketBox = new QGroupBox("📈 Stock Market");
    QVBoxLayout *marketLayout = new QVBoxLayout();

    stockMarketTable = new QTableWidget();
    stockMarketTable->setColumnCount(5);
    stockMarketTable->setHorizontalHeaderLabels({"Symbol", "Company", "Price", "Change %", "Trend"});
    stockMarketTable->horizontalHeader()->setStretchLastSection(true);
    stockMarketTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    stockMarketTable->setMinimumHeight(200);
    marketLayout->addWidget(stockMarketTable);
    marketBox->setLayout(marketLayout);

    // Control for the trading bot
    QGroupBox *botBox = new QGroupBox("🤖 Trading Bot");
    QVBoxLayout *botLayout = new QVBoxLayout();

    QHBoxLayout *botStatusLayout = new QHBoxLayout();
    botStatusLabel = new QLabel("Bot Status: INACTIVE");
    botStatusLabel->setStyleSheet(
        "font-weight: bold;"
        "color: #d13438;"
        "font-size: 14px;"
        "padding: 6px 12px;"
        "background-color: #fef0f0;"
        "border-radius: 4px;"
    );

    strategyLabel = new QLabel("Strategy: Conservative");
    strategyLabel->setStyleSheet(
        "font-weight: bold;"
        "color: #0078d4;"
        "font-size: 14px;"
        "padding: 6px 12px;"
        "background-color: #e6f2ff;"
        "border-radius: 4px;"
    );

    marketConditionLabel = new QLabel("Market: UNKNOWN");
    marketConditionLabel->setStyleSheet(
        "font-weight: bold;"
        "color: #8a8a8a;"
        "font-size: 14px;"
        "padding: 6px 12px;"
        "background-color: #f0f0f0;"
        "border-radius: 4px;"
    );

    botStatusLayout->addWidget(botStatusLabel);
    botStatusLayout->addWidget(strategyLabel);
    botStatusLayout->addWidget(marketConditionLabel);
    botStatusLayout->addStretch();
    botLayout->addLayout(botStatusLayout);

    QHBoxLayout *botButtonLayout = new QHBoxLayout();
    botButtonLayout->setSpacing(8);
    
    startBotButton = new QPushButton("▶️ Start Bot");
    startBotButton->setMinimumHeight(38);
    startBotButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #107c10;"
        "   font-size: 13px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #0e6b0e;"
        "}"
    );
    
    stopBotButton = new QPushButton("⏸️ Stop Bot");
    stopBotButton->setMinimumHeight(38);
    stopBotButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #d13438;"
        "   font-size: 13px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #a52a2a;"
        "}"
    );
    
    endSimulationButton = new QPushButton("🏁 End Simulation");
    endSimulationButton->setMinimumHeight(38);
    endSimulationButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #8a8a8a;"
        "   font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #6a6a6a;"
        "}"
    );
    
    resetSimulationButton = new QPushButton("🔄 Reset");
    resetSimulationButton->setMinimumHeight(38);
    resetSimulationButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #ff8c00;"
        "   font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #e67e00;"
        "}"
    );

    connect(startBotButton, &QPushButton::clicked, this, &MainWindow::onStartBotClicked);
    connect(stopBotButton, &QPushButton::clicked, this, &MainWindow::onStopBotClicked);
    connect(endSimulationButton, &QPushButton::clicked, this, &MainWindow::onEndSimulationClicked);
    connect(resetSimulationButton, &QPushButton::clicked, this, &MainWindow::onResetSimulationClicked);

    botButtonLayout->addWidget(startBotButton);
    botButtonLayout->addWidget(stopBotButton);
    botButtonLayout->addWidget(endSimulationButton);
    botButtonLayout->addWidget(resetSimulationButton);
    botLayout->addLayout(botButtonLayout);
    botBox->setLayout(botLayout);

    // Portfolio of current shares held
    QGroupBox *portfolioBox = new QGroupBox("💼 Portfolio");
    QVBoxLayout *portfolioLayout = new QVBoxLayout();

    portfolioTable = new QTableWidget();
    portfolioTable->setColumnCount(5);
    portfolioTable->setHorizontalHeaderLabels({"Symbol", "Shares", "Avg Price", "Current Value", "Profit"});
    portfolioTable->horizontalHeader()->setStretchLastSection(true);
    portfolioTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    portfolioTable->setMinimumHeight(150);
    portfolioTable->setAlternatingRowColors(true);
    portfolioTable->setStyleSheet(
        "QTableWidget {"
        "   alternate-background-color: #404040;"
        "}"
    );
    portfolioLayout->addWidget(portfolioTable);
    portfolioBox->setLayout(portfolioLayout);

    // Performance Stats after running the simulations
    QGroupBox *statsBox = new QGroupBox("📊 Performance Summary");
    statsBox->setStyleSheet(
        "QGroupBox {"
        "   background-color: #353535;"
        "   border: 2px solid #0078d4;"
        "}"
    );
    QGridLayout *statsLayout = new QGridLayout();
    statsLayout->setSpacing(12);

    QLabel *profitLabel = new QLabel("Total Profit:");
    profitLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #ffffff;");
    statsLayout->addWidget(profitLabel, 0, 0);
    totalProfitLabel = new QLabel("$0.00");
    totalProfitLabel->setStyleSheet(
        "font-weight: bold;"
        "color: #107c10;"
        "font-size: 16px;"
        "padding: 4px 8px;"
        "background-color: #1a3a1a;"
        "border-radius: 4px;"
    );
    statsLayout->addWidget(totalProfitLabel, 0, 1);

    QLabel *sharesLabel = new QLabel("Total Shares Owned:");
    sharesLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #ffffff;");
    statsLayout->addWidget(sharesLabel, 1, 0);
    totalSharesLabel = new QLabel("0");
    totalSharesLabel->setStyleSheet(
        "font-weight: bold;"
        "font-size: 16px;"
        "color: #ffffff;"
        "padding: 4px 8px;"
        "background-color: #3a3a1a;"
        "border-radius: 4px;"
    );
    statsLayout->addWidget(totalSharesLabel, 1, 1);

    QLabel *daysLabel = new QLabel("Days Elapsed:");
    daysLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #ffffff;");
    statsLayout->addWidget(daysLabel, 2, 0);
    daysElapsedLabel = new QLabel("0");
    daysElapsedLabel->setStyleSheet(
        "font-weight: bold;"
        "font-size: 16px;"
        "color: #ffffff;"
        "padding: 4px 8px;"
        "background-color: #1a2a3a;"
        "border-radius: 4px;"
    );
    statsLayout->addWidget(daysElapsedLabel, 2, 1);
    statsBox->setLayout(statsLayout);

    // Displays the Trading  History of the bots
    QGroupBox *tradeHistoryBox = new QGroupBox("📜 Trade History");
    QVBoxLayout *tradeHistoryLayout = new QVBoxLayout();

    tradeHistoryDisplay = new QTextEdit();
    tradeHistoryDisplay->setReadOnly(true);
    tradeHistoryDisplay->setMaximumHeight(150);
    tradeHistoryDisplay->setStyleSheet(
        "QTextEdit {"
        "   font-family: 'Courier New', monospace;"
        "   font-size: 12px;"
        "   background-color: #3c3c3c;"
        "   color: #ffffff;"
        "}"
    );
    tradeHistoryLayout->addWidget(tradeHistoryDisplay);
    tradeHistoryBox->setLayout(tradeHistoryLayout);

    layout->addWidget(marketBox);
    layout->addWidget(botBox);
    layout->addWidget(portfolioBox);
    layout->addWidget(statsBox);
    layout->addWidget(tradeHistoryBox);
    
    // Set the scroll widget and create the trading page with scroll area
    scrollArea->setWidget(scrollWidget);
    tradingPage = scrollArea;
}

// --- User Authentication ---
// Handle login, registration, and logout

void MainWindow::onLoginClicked() {
    QString username = usernameInput->text();
    QString password = passwordInput->text();

    if (username.isEmpty() || password.isEmpty()) {
        statusLabel->setText("Please enter username and password");
        statusLabel->setStyleSheet("color: red;");
        return;
    }

    BankingTradingFacade& facade = BankingTradingFacade::getInstance();

    if (facade.login(username.toStdString(), password.toStdString())) {
        statusLabel->setText("Login successful!");
        statusLabel->setStyleSheet("color: green;");
        updateUIState();
        refreshBalance();
        refreshMarketData();
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

    BankingTradingFacade& facade = BankingTradingFacade::getInstance();

    if (facade.registerUser(username.toStdString(), password.toStdString(), 10000.0)) {
        statusLabel->setText("Registration successful! Please login.");
        statusLabel->setStyleSheet("color: green;");
        passwordInput->clear();
    } else {
        statusLabel->setText("Username already exists");
        statusLabel->setStyleSheet("color: red;");
    }
}

void MainWindow::onLogoutClicked() {
    BankingTradingFacade::getInstance().logout();

    usernameInput->clear();
    passwordInput->clear();
    statusLabel->clear();
    transactionDisplay->clear();

    updateUIState();
}

// --- Banking Operations ---
// Deposits, withdrawals, and scheduled transactions

void MainWindow::onDepositClicked() {
    BankingTradingFacade& facade = BankingTradingFacade::getInstance();

    double amount = depositAmount->value();
    QString description = depositDescription->text();

    if (facade.deposit(amount, description.toStdString(), currentDay)) {
        QMessageBox::information(this, "Success",
                                 QString("Deposited $%1 on day %2!").arg(amount, 0, 'f', 2).arg(currentDay));
        refreshBalance();
        refreshTransactionHistory();  // Automatically refresh transaction display
    } else {
        QMessageBox::warning(this, "Error", "Deposit failed!");
    }
}

void MainWindow::onWithdrawClicked() {
    BankingTradingFacade& facade = BankingTradingFacade::getInstance();

    double amount = withdrawAmount->value();
    QString description = withdrawDescription->text();

    if (facade.withdraw(amount, description.toStdString(), currentDay)) {
        QMessageBox::information(this, "Success",
                                 QString("Withdrew $%1 on day %2!").arg(amount, 0, 'f', 2).arg(currentDay));
        refreshBalance();
        refreshTransactionHistory();  // Automatically refresh transaction display
    } else {
        QMessageBox::warning(this, "Error", "Withdrawal failed! Insufficient balance.");
    }
}

void MainWindow::onViewTransactionsClicked() {
    refreshTransactionHistory();
}

void MainWindow::onScheduleDepositClicked() {
    BankingTradingFacade& facade = BankingTradingFacade::getInstance();

    double amount = scheduledDepositAmount->value();
    QString description = scheduledDepositDescription->text();
    int day = scheduledDepositDay->value();

    if (day <= currentDay) {
        QMessageBox::warning(this, "Error",
                             QString("Scheduled day must be in the future (current day is %1)").arg(currentDay));
        return;
    }

    if (facade.scheduleDeposit(day, amount, description.toStdString())) {
        QMessageBox::information(this, "Success",
                                 QString("Scheduled $%1 deposit for day %2").arg(amount, 0, 'f', 2).arg(day));
    } else {
        QMessageBox::warning(this, "Error", "Failed to schedule deposit!");
    }
}

void MainWindow::onViewScheduledDepositsClicked() {
    BankingTradingFacade& facade = BankingTradingFacade::getInstance();
    std::vector<BankingTradingFacade::SimpleScheduledDeposit> scheduled = facade.getScheduledDeposits();

    QString message;
    if (scheduled.empty()) {
        message = "No scheduled deposits.";
    } else {
        message = QString("Scheduled Deposits (Current Day: %1):\n\n").arg(currentDay);
        for (const auto& dep : scheduled) {
            QString status = dep.executed ? "EXECUTED" : "PENDING";
            message += QString("Day %1: $%2 - %3 [%4]\n")
                           .arg(dep.scheduledDay)
                           .arg(dep.amount, 0, 'f', 2)
                           .arg(QString::fromStdString(dep.description))
                           .arg(status);
        }
    }

    QMessageBox::information(this, "Scheduled Deposits", message);
}

void MainWindow::onAdvanceDayClicked() {
    BankingTradingFacade& facade = BankingTradingFacade::getInstance();
    TradingBot& bot = TradingBot::getInstance();

    currentDay++;
    currentDayLabel->setText(QString("Current Day: %1").arg(currentDay));

    // Execute scheduled deposits
    int executed = facade.executeScheduledDeposits(currentDay);
    if (executed > 0) {
        QMessageBox::information(this, "Deposits Executed",
                                 QString("Executed %1 scheduled deposit(s) on day %2!").arg(executed).arg(currentDay));
    }

    // Advance market and run bot
    bot.advanceDay();
    if (bot.isRunning()) {
        bot.executeTradingCycle();
    }

    // Update UI
    refreshBalance();
    refreshTransactionHistory();
    refreshMarketData();
    refreshPortfolio();
    refreshTradingStats();
}

// --- Trading Bot Controls ---
// Start, stop, and manage the automated trading bot

void MainWindow::onStartBotClicked() {
    BankingTradingFacade& facade = BankingTradingFacade::getInstance();
    facade.startBot();

    botStatusLabel->setText("Bot Status: ACTIVE");
    botStatusLabel->setStyleSheet(
        "font-weight: bold;"
        "color: #107c10;"
        "font-size: 14px;"
        "padding: 6px 12px;"
        "background-color: #e6ffe6;"
        "border-radius: 4px;"
    );

    QMessageBox::information(this, "Trading Bot", "Trading bot started!");
}

void MainWindow::onStopBotClicked() {
    BankingTradingFacade& facade = BankingTradingFacade::getInstance();
    facade.stopBot();

    botStatusLabel->setText("Bot Status: INACTIVE");
    botStatusLabel->setStyleSheet(
        "font-weight: bold;"
        "color: #d13438;"
        "font-size: 14px;"
        "padding: 6px 12px;"
        "background-color: #fef0f0;"
        "border-radius: 4px;"
    );

    QMessageBox::information(this, "Trading Bot", "Trading bot stopped.");
}

void MainWindow::onRefreshMarketClicked() {
    refreshMarketData();
}

void MainWindow::onEndSimulationClicked() {
    TradingBot& bot = TradingBot::getInstance();
    bot.stopBot();

    // Try to end with profit
    int waitDays = 0;
    int maxWaitDays = 10;

    while (!bot.tryEndWithProfit(maxWaitDays, waitDays)) {
        waitDays++;
        currentDay++;
        bot.advanceDay();
        bot.executeTradingCycle();
    }

    currentDayLabel->setText(QString("Current Day: %1").arg(currentDay));

    double profit = bot.getProfit();
    int totalTrades = bot.getHistory().size();

    QString resultMsg = QString(
                            "=== SIMULATION ENDED ===\n\n"
                            "Final Profit: $%1\n"
                            "Total Trades: %2\n"
                            "Days Elapsed: %3\n"
                            "Extra Days Waited: %4\n\n"
                            "%5"
                            ).arg(profit, 0, 'f', 2)
                            .arg(totalTrades)
                            .arg(currentDay)
                            .arg(waitDays)
                            .arg(profit > 0 ? "SUCCESS: Ended with profit!" : "Note: Had to cut some losses.");

    QMessageBox::information(this, "Simulation Results", resultMsg);

    refreshBalance();
    refreshPortfolio();
    refreshTradingStats();
    refreshTradeHistory();

    botStatusLabel->setText("Bot Status: SIMULATION ENDED");
    botStatusLabel->setStyleSheet("font-weight: bold; color: blue;");
}

void MainWindow::onResetSimulationClicked() {
    BankingTradingFacade& facade = BankingTradingFacade::getInstance();

    // Confirm before resetting everything
    QMessageBox::StandardButton reply = QMessageBox::question(this, "START OVER",
                                                              "Are you sure you want to reset the simulation?\n\n"
                                                              "Wiping trading bot history ok?",

                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) return;

    // Reset everything using Facade
    currentDay = 1;
    facade.resetSimulation();

    // Update UI
    currentDayLabel->setText(QString("Current Day: %1").arg(currentDay));
    botStatusLabel->setText("Bot Status: INACTIVE");
    botStatusLabel->setStyleSheet("font-weight: bold; color: red;");

    refreshBalance();
    refreshMarketData();
    refreshPortfolio();
    refreshTradingStats();
    transactionDisplay->clear();
    tradeHistoryDisplay->clear();

    QMessageBox::information(this, "Reset Complete",
                             "Simulation has been reset.\n\n"
                             "Day: 1\n"
                             "Balance: $10,000\n"
                             "Stock prices reset to opening values.");
}

// --- UI Helper Functions ---
// Update displays and refresh data across the interface

void MainWindow::updateUIState() {
    BankingTradingFacade& facade = BankingTradingFacade::getInstance();
    bool loggedIn = facade.isLoggedIn();

    loginWidget->setVisible(!loggedIn);
    headerWidget->setVisible(loggedIn);
    tabWidget->setVisible(loggedIn);

    if (loggedIn) {
        string username = facade.getCurrentUser();
        welcomeLabel->setText(QString("Welcome, %1!").arg(QString::fromStdString(username)));
    }
}

void MainWindow::refreshBalance() {
    BankingTradingFacade& facade = BankingTradingFacade::getInstance();
    double balance = facade.getBalance();
    balanceLabel->setText(QString("Balance: $%1").arg(balance, 0, 'f', 2));
}

void MainWindow::refreshTransactionHistory() {
    BankingTradingFacade& facade = BankingTradingFacade::getInstance();
    vector<BankingTradingFacade::SimpleTransaction> transactions = facade.getTransactionHistory();

    transactionDisplay->clear();

    if (transactions.empty()) {
        transactionDisplay->setPlainText("No transactions yet.");
        return;
    }

    QString history = QString("=== TRANSACTIONS (%1) ===\n\n").arg(transactions.size());

    for (const auto& trans : transactions) {
        history += QString("%1 | Day %2 | $%3 | %4\n")
        .arg(QString::fromStdString(trans.type), -15)
            .arg(trans.day, 3)
            .arg(trans.amount, 10, 'f', 2)
            .arg(QString::fromStdString(trans.description));
    }

    transactionDisplay->setPlainText(history);
}

void MainWindow::refreshMarketData() {
    BankingTradingFacade& facade = BankingTradingFacade::getInstance();
    vector<BankingTradingFacade::SimpleStockInfo> stocks = facade.getMarketData();

    stockMarketTable->setRowCount(stocks.size());

    int row = 0;
    for (const auto& stock : stocks) {
        stockMarketTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(stock.symbol)));
        stockMarketTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(stock.name)));
        stockMarketTable->setItem(row, 2, new QTableWidgetItem(QString("$%1").arg(stock.currentPrice, 0, 'f', 2)));



        row++;
    }

    stockMarketTable->resizeColumnsToContents();
}

void MainWindow::refreshPortfolio() {
    BankingTradingFacade& facade = BankingTradingFacade::getInstance();
    vector<BankingTradingFacade::SimplePortfolioItem> holdings = facade.getPortfolio();

    portfolioTable->setRowCount(holdings.size());

    int row = 0;
    for (const auto& h : holdings) {
        portfolioTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(h.symbol)));
        portfolioTable->setItem(row, 1, new QTableWidgetItem(QString::number(h.shares)));
        portfolioTable->setItem(row, 2, new QTableWidgetItem(QString("$%1").arg(h.averagePrice, 0, 'f', 2)));
        portfolioTable->setItem(row, 3, new QTableWidgetItem(QString("$%1").arg(h.currentValue, 0, 'f', 2)));

        QTableWidgetItem* profitItem = new QTableWidgetItem(QString("$%1").arg(h.profit, 0, 'f', 2));
        profitItem->setForeground(h.profit >= 0 ? Qt::darkGreen : Qt::red);
        portfolioTable->setItem(row, 4, profitItem);

        row++;
    }

    portfolioTable->resizeColumnsToContents();
}

void MainWindow::refreshTradingStats() {
    BankingTradingFacade& facade = BankingTradingFacade::getInstance();
    BankingTradingFacade::PerformanceSummary performance = facade.getPerformance();

    totalProfitLabel->setText(QString("$%1").arg(performance.totalProfit, 0, 'f', 2));
    totalProfitLabel->setStyleSheet(performance.totalProfit >= 0 ? "font-weight: bold; color: green;" : "font-weight: bold; color: red;");

    totalSharesLabel->setText(QString::number(performance.totalShares));
    daysElapsedLabel->setText(QString::number(currentDay));

    strategyLabel->setText(QString("Strategy: %1").arg(QString::fromStdString(facade.getBotStatus())));
    // Market condition accessed directly from TradingBot
    TradingBot& bot = TradingBot::getInstance();
    marketConditionLabel->setText(QString("Market: %1").arg(QString::fromStdString(bot.getMarketCondition())));

    refreshTradeHistory();
}

void MainWindow::refreshTradeHistory() {
    BankingTradingFacade& facade = BankingTradingFacade::getInstance();
    vector<BankingTradingFacade::SimpleTradeRecord> trades = facade.getTradeHistory();

    QString history;
    if (trades.empty()) {
        history = "No trades yet.";
    } else {
        for (const auto& t : trades) {
            history += QString("[Day %1] %2 %3 x%4 @ $%5 = $%6 (%7)\n")
            .arg(t.day)
                .arg(QString::fromStdString(t.type))
                .arg(QString::fromStdString(t.symbol))
                .arg(t.shares)
                .arg(t.price, 0, 'f', 2)
                .arg(t.total, 0, 'f', 2)
                .arg(QString::fromStdString(t.reason));
        }
    }

    tradeHistoryDisplay->setPlainText(history);
}
