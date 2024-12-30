#include "order.h"

Order::Order(std::string id, char type, int quantity, double price, bool isMarketOrder, int timestamp)
    : id(std::move(id)), type(type), quantity(quantity), price(price),
      isMarketOrder(isMarketOrder), timestamp(timestamp) {}
