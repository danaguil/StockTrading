#ifndef STOCKABSTRACTFACTORY_H
#define STOCKABSTRACTFACTORY_H
#include <iostream>
using namespace std;

// Abstract Factory
class StockAbstractFactory {
public:
    virtual ~StockAbstractFactory() {}

    virtual class Stock* createStock(const string& ticker) = 0;
    virtual class StockPriceGenerator* createPriceGenerator() = 0;
};

class Stock {
private:
    string ticker;
public:
    Stock(const string& t) : ticker(t) {}

    void display() {
        cout << "Stock: " << ticker << endl;
    }
};

class StockPriceGenerator {
private:
    double drift;       // Slow upward movement (helps show profit in demo)
    double volatility;  // How much it wiggles

public:
    StockPriceGenerator(double d, double v)
        : drift(d), volatility(v) {}

    // random percent change = drift + random(-volatility, +volatility)
    /*
    *  "(rand() % 2001 - 1000)" -> rand num between -1000 to +1000
    *  "1000.0 -> divide so num becomes between -1.0 and +1.0
    *
    *  e.g. of volatility. if v = 0.4%
    *  -> price moves between -0.4 and +0.4
    */
    double generate(double price) {
        double randomMove = (rand() % 2001 - 1000) / 1000.0; // -1 to +1
        double percentChange = drift + (randomMove * volatility);
        return price * (1.0 + percentChange);
    }
};

// Use function in main
class SimpleStockFactory : public StockAbstractFactory {
public:
    Stock* createStock(const std::string& ticker) override {
        return new Stock(ticker);
    }

    StockPriceGenerator* createPriceGenerator() override {
        return new StockPriceGenerator(0.0008, 0.012);
        //     ↑ drift (upwards bias)  ↑ volatility (+/-1.2%)
    }
};

/*
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include "StockAbstractFactory.h"
using namespace std;


int main() {
    srand(time(NULL));

    SimpleStockFactory factory;

    Stock* tesla = factory.createStock("TSLA");
    tesla->display();

    StockPriceGenerator* gen = factory.createPriceGenerator();

    double price = 170.00;
    std::cout << "Start: $" << price << "\n";

    for (int i = 0; i < 10; i++) {
        price = gen->generate(price);
        price = floor(price * 100) / 100.0; // round to 2 decimals
        std::cout << "Price " << i+1 << ": $" << price << "\n";
    }

    delete tesla;
    delete gen;
}

 */

#endif