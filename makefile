
CXX = g++
CXXFLAGS = -std=c++11 -Wall

all: main

main: main.o order.o orderbook.o
	$(CXX) $(CXXFLAGS) -o main main.o order.o orderbook.o

main.o: main.cpp orderbook.h
	$(CXX) $(CXXFLAGS) -c main.cpp

order.o: order.cpp orderbook.h
	$(CXX) $(CXXFLAGS) -c order.cpp

orderbook.o: orderbook.cpp orderbook.h
	$(CXX) $(CXXFLAGS) -c orderbook.cpp

clean:
	rm -f *.o main
