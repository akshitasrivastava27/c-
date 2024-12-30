#include <iostream>
#include <fstream>
#include <string>
#include "orderbook.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile) {
        std::cerr << "Error opening input file." << std::endl;
        return 1;
    }

    double initialPrice;
    inputFile >> initialPrice; // Read last traded price
    OrderBook orderBook(initialPrice);

    std::string orderLine;
    while (std::getline(inputFile, orderLine)) {
        if (!orderLine.empty()) {
            orderBook.processOrder(orderLine);
        }
    }

    orderBook.printOutput(); // Write execution results
    return 0;
}
