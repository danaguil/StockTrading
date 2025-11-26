# Banking System - Qt GUI

A Qt-based GUI application for a banking system with user authentication using the Singleton pattern.

## Features

### Banking System (Singleton Pattern)
- User registration and login
- Deposit and withdrawal operations
- Transaction history tracking
- Balance management
- Thread-safe singleton implementation

## Project Structure

```
GUI/
├── BankingSystem.h       # Core banking logic (Singleton)
├── MainWindow.h          # Main window header
├── MainWindow.cpp        # Main window implementation
├── main.cpp              # Application entry point
├── BankingTradingSystem.pro  # Qt project file
└── README.md             # This file
```

## Building the Project


### Installing Qt on macOS

1. **Install Qt via Homebrew:**
```bash
brew install qt@6
```

2. **Add Qt to your PATH** (add to your `~/.zshrc` or `~/.bash_profile`):
```bash
# For Intel Macs:
export PATH="/usr/local/opt/qt@6/bin:$PATH"

# For Apple Silicon (M1/M2/M3):
export PATH="/opt/homebrew/opt/qt@6/bin:$PATH"
```

3. **Apply the changes:**
```bash
source ~/.zshrc
```

### Build Instructions

1. **Navigate to the project directory:**
```bash
cd StockTrading
```

2. **Generate Makefile with qmake:**
```bash
qmake BankingTradingSystem.pro
# Or if qmake is not in PATH:
/usr/local/opt/qt@6/bin/qmake BankingTradingSystem.pro
```

3. **Build the application:**
```bash
make
```

4. **Run the application:**
```bash
open BankingTradingSystem.app
```