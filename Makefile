# Makefile for the smash program
CXX = g++
CPPFLAGS = -std=c++11 -Wall -pedantic -Wall -DNDEBUG -pthread
CCLINK = $(CXX)

SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))
DEPS = $(wildcard *.h)
TARGET = ttftps
RM = rm -f
# Creating the  executable
ttftps: $(OBJS)
	$(CXX) -o $@ $^ $(CPPFLAGS)

data_handler.o: data_handler.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CPPFLAGS)
wrq_handler.o: wrq_handler.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CPPFLAGS)
state.o: state.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CPPFLAGS)
server.o: server.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CPPFLAGS)
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*