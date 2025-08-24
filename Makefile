CXX = g++
CXXFLAGS = -std=c++17 -pthread -Wall -Wextra -O2 -Iinclude
AR = ar
ARFLAGS = rcs


LIBNAME = libjagoMQ.a
SRCDIR = src
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=%.o)

# Default target builds the library
$(LIBNAME): $(OBJECTS)
	$(AR) $(ARFLAGS) $(LIBNAME) $(OBJECTS)

%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(LIBNAME)

.PHONY: clean