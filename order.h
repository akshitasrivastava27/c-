#ifndef ORDER_H
#define ORDER_H

#include <string>

class Order {
private:
    std::string id;
    char type; // 'B' for buy, 'S' for sell
    int quantity;
    double price;
    bool isMarketOrder;
    int timestamp;

public:
    Order(std::string id, char type, int quantity, double price, bool isMarketOrder, int timestamp);

    // Getters
    const std::string& getId() const { return id; }
    char getType() const { return type; }
    int getQuantity() const { return quantity; }
    double getPrice() const { return price; }
    bool getIsMarketOrder() const { return isMarketOrder; }
    int getTimestamp() const { return timestamp; }

    // Setters
    void setQuantity(int newQuantity) { quantity = newQuantity; }
};

#endif
