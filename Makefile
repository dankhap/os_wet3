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

# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*