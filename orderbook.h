#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <set>
#include <vector>
#include "order.h"

// Comparator for buy orders
bool compareBuyOrders(const Order& a, const Order& b);

// Comparator for sell orders
bool compareSellOrders(const Order& a, const Order& b);

class OrderBook {
private:
    std::multiset<Order, decltype(&compareBuyOrders)> buyOrders;
    std::multiset<Order, decltype(&compareSellOrders)> sellOrders;
    double lastTradedPrice;
    int currentTime;
    std::vector<std::string> executionLog;

public:
    OrderBook(double initialPrice);
    void processOrder(const std::string& orderLine);
    void printOutput()const;

private:
    void addOrder(const Order& order);
    void matchOrders();
    void executeOrder(Order buy, Order sell);
    double determineTradePrice(const Order& buy, const Order& sell) const;
};

#endif
