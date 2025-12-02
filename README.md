# StockTrading Application using Qt GUI

A Qt-based GUI application for a banking/trading system with user authentication using Facade, Singleton, Strategy, and Abstract Factory Design Batterns. This was coded with C++ and is our submission for our project for CS 542. 

## Contributers
Max, Jose, and Daniel. 

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
```

3. **Build the application:**
```bash
make
```

4. **Run the application:**
```bash
open BankingTradingSystem.app
```