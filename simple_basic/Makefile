CXX      = g++
CXXFLAGS = -Wall -Werror
CXXFILES = main.cpp lib_simple_basic.cpp
HFILES   = lib_simple_basic.h

OBJECTS = $(CXXFILES:.cpp=.o)
TARGET  = lab

all: $(TARGET)

lab: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

include deps.make
deps.make: $(CXXFILES) $(HFILES)
	$(CXX) -MM $(CXXFILES) > deps.make

clean:
	rm -f lab *.o