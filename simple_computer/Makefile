CXX      = g++
CXXFLAGS = -Wall -Werror
CXXFILES = main.cpp simple_computer.cpp terminal.cpp big_chars.cpp UI.cpp read_key.cpp CU.cpp ALU.cpp
HFILES   = simple_computer.h terminal.h big_chars.h UI.h read_key.h CU.h ALU.h

OBJECTS = $(CXXFILES:.cpp=.o)
TARGET  = computer

all: $(TARGET)

computer: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

include deps.make
deps.make: $(CXXFILES) $(HFILES)
	$(CXX) -MM $(CXXFILES) > deps.make

clean:
	rm -f lab *.o