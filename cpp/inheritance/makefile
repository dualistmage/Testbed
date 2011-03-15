CXX = g++

######################################################
# if $(COMPILE_FLAGS) is given by upper makefile 
# else if no $(COMPILE_FLAGS) is given 
ifneq ($(COMPILE_FLAGS),)
CXXFLAGS += $(COMPILE_FLAGS) 
else
#CXXFLAGS = -Wall -O3 -g
CXXFLAGS = -Wall -O3 -g
endif


TARGET = main

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))

INCLUDE_PATH = \

LIB_FLAGS = $(LIB_PATH) $(LIB)

LIB_PATH = \

LIB = \

%.o : %.cpp %.h
	$(CXX) $(INCLUDE_PATH) $(CXXFLAGS) -c -o $@ $^ 

all: $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CXX) -o $@ $^ $(LIB_FLAGS)

clean:
	rm -rf *.o $(TARGET) 

