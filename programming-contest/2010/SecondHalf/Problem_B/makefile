CXX = g++
CXXFLAGS = -Wall -ansi -g
#CXXFLAGS = -O1

TARGET = Problem_B
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
