CXX      = g++
CXXFLAGS = -Wall -Werror
CXXFILES = main.cpp

OBJECTS = $(CXXFILES:.cpp=.o)
TARGET  = course_work

all: $(TARGET)

course_work: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f lab *.o