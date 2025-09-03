CXX = g++
CXXFLAGS = -std=c++17 -pthread -Wall -Wextra -g -O0 -I../include
LDFLAGS = -L.. -ljagoMQ

all: demo_subscriber demo_publisher

demo_subscriber: demo_subscriber.cpp 
	$(CXX) $(CXXFLAGS) demo_subscriber.cpp -o demo_subscriber $(LDFLAGS)

demo_publisher: demo_publisher.cpp
	$(CXX) $(CXXFLAGS) demo_publisher.cpp -o demo_publisher $(LDFLAGS)

clean:
	rm -f demo_subscriber demo_publisher

.PHONY: all clean