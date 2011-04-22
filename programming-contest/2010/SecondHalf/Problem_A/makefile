CXX = g++
#CXXFLAGS = -Wall -ansi -O3 -g
CXXFLAGS = -O1

TARGET = Problem_A
OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))

BOOST_INCLUDE = ../
INCLUDE = -I $(BOOST_INCLUDE)

%.o : %.cpp
	$(CXX) $(INCLUDE) $(CXXFLAGS) -c -o $@ $^

all:$(TARGET)

$(TARGET) : $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -rf $(OBJECTS) $(TARGET)
