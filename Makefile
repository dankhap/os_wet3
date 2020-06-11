# Makefile for the smash program
CXX = g++
CPPFLAGS = -std=c++11 -Wall -Werror -pedantic-errors -DNDEBUG -pthread
CCLINK = $(CXX)

SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))
DEPS = $(wildcard *.h)
TARGET = smash
RM = rm -f
# Creating the  executable
ttftps: $(OBJS)
	$(CXX) -o $@ $^ $(CPPFLAGS)
logger.o: logger.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CPPFLAGS)

# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*