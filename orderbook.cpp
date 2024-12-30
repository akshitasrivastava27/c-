#include "orderbook.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>

// Comparator for buy orders
bool compareBuyOrders(const Order& a, const Order& b) {
    if (a.getIsMarketOrder() != b.getIsMarketOrder()) return a.getIsMarketOrder();
    if (a.getPrice() != b.getPrice()) return a.getPrice() > b.getPrice();
    return a.getTimestamp() < b.getTimestamp();
}

// Comparator for sell orders
bool compareSellOrders(const Order& a, const Order& b) {
    if (a.getIsMarketOrder() != b.getIsMarketOrder()) return a.getIsMarketOrder();
    if (a.getPrice() != b.getPrice()) return a.getPrice() < b.getPrice();
    return a.getTimestamp() < b.getTimestamp();
}

// Constructor
OrderBook::OrderBook(double initialPrice)
    : buyOrders(compareBuyOrders), sellOrders(compareSellOrders),
      lastTradedPrice(initialPrice), currentTime(0) {}

// Process a single order line
void OrderBook::processOrder(const std::string& orderLine) {
    std::istringstream stream(orderLine);
    std::string id;
    char type;
    int quantity;
    double price = 0.0;
    bool isMarketOrder = false;

    stream >> id >> type >> quantity;
    if (stream.peek() == std::istream::traits_type::eof()) {
        isMarketOrder = true;
    } else {
        stream >> price;
    }

    Order order(id, type, quantity, price, isMarketOrder, currentTime++);
    addOrder(order);
    matchOrders();
}

// Add an order to the appropriate set
void OrderBook::addOrder(const Order& order) {
    if (order.getType() == 'B') {
        buyOrders.insert(order);
    } else {
        sellOrders.insert(order);
    }
}

// Match buy and sell orders
void OrderBook::matchOrders() {
    while (!buyOrders.empty() && !sellOrders.empty()) {
        auto topBuy = buyOrders.begin();
        auto topSell = sellOrders.begin();  

        if (topBuy->getIsMarketOrder() || topSell->getIsMarketOrder() || topBuy->getPrice() >= topSell->getPrice()) {
            executeOrder(*topBuy, *topSell);
        } else {
            break;
        }
    }
}

// Execute a single trade and handle residual orders
void OrderBook::executeOrder(Order buy, Order sell) {
    int tradeQuantity = std::min(buy.getQuantity(), sell.getQuantity());
    double tradePrice = determineTradePrice(buy, sell);

    // Use a string stream for consistent formatting
    std::ostringstream buyStream, sellStream;
    buyStream << std::fixed << std::setprecision(2);
    sellStream << std::fixed << std::setprecision(2);

    // Log execution for the buyer
    buyStream << "order " << buy.getId() << " " << tradeQuantity
              << " shares purchased at price " << tradePrice;
    executionLog.push_back(buyStream.str());

    // Log execution for the seller
    sellStream << "order " << sell.getId() << " " << tradeQuantity
               << " shares sold at price " << tradePrice;
    executionLog.push_back(sellStream.str());

    // Update last traded price
    lastTradedPrice = tradePrice;

    // Handle quantities and residual orders
    buy.setQuantity(buy.getQuantity() - tradeQuantity);
    sell.setQuantity(sell.getQuantity() - tradeQuantity);

    if (buy.getQuantity() == 0) {
        buyOrders.erase(buyOrders.begin());
    } else {
        buyOrders.erase(buyOrders.begin());
        buyOrders.insert(buy);
    }

    if (sell.getQuantity() == 0) {
        sellOrders.erase(sellOrders.begin());
    } else {
        sellOrders.erase(sellOrders.begin());
        sellOrders.insert(sell);
    }
}

// Determine the trade price based on order types and priorities
double OrderBook::determineTradePrice(const Order& buy, const Order& sell) const {
    // Case 1: Both orders are limit orders
    if (!buy.getIsMarketOrder() && !sell.getIsMarketOrder()) {
        return (buy.getTimestamp() < sell.getTimestamp()) ? buy.getPrice() : sell.getPrice();
    }
    // Case 2: One is a market order
    if (buy.getIsMarketOrder() && !sell.getIsMarketOrder()) {
        return sell.getPrice(); // Use the limit price of the seller
    }
    if (!buy.getIsMarketOrder() && sell.getIsMarketOrder()) {
        return buy.getPrice(); // Use the limit price of the buyer
    }
    // Case 3: Both are market orders
    return lastTradedPrice; // Use the last traded price
}

// Print output to console
void OrderBook::printOutput() const {
    // Print executed trades
    for (const auto& execution : executionLog) {
        std::cout << execution << std::endl;
    }

    // Collect all unexecuted orders
    std::vector<Order> unexecutedOrders;
    unexecutedOrders.insert(unexecutedOrders.end(), buyOrders.begin(), buyOrders.end());
    unexecutedOrders.insert(unexecutedOrders.end(), sellOrders.begin(), sellOrders.end());

    // Sort unexecuted orders by their timestamp (or received index)
    std::sort(unexecutedOrders.begin(), unexecutedOrders.end(),
              [](const Order& a, const Order& b) {
                  return a.getTimestamp() < b.getTimestamp();
              });

    // Print unexecuted orders
    for (const auto& order : unexecutedOrders) {
        std::cout << "order " << order.getId() << " " << order.getQuantity()
                  << " shares unexecuted" << std::endl;
    }
}
