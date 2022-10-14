CXX      = g++
CXXFLAGS = -Wall -Werror
CXXFILES = main.cpp lib_simple_assembler.cpp
HFILES   = lib_simple_assembler.h

OBJECTS = $(CXXFILES:.cpp=.o)
TARGET  = sat

all: $(TARGET)

sat: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

include deps.make
deps.make: $(CXXFILES) $(HFILES)
	$(CXX) -MM $(CXXFILES) > deps.make

clean:
	rm -f sat *.o