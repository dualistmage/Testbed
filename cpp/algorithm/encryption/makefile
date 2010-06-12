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

MY_HOME = $(HOME)/test/cpp/algorithm/encryption

SHA1_SRC = $(MY_HOME)/source/sha1

TARGET = main

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp \
		  									$(SHA1_SRC)/*.cpp ))

INCLUDE_PATH = \
			   -I $(BOOST)/include \

LIB_FLAGS = $(LIB_PATH) $(LIB)

LIB_PATH = \
		   -L $(BOOST)/lib \

LIB = \
	  -l boost_system-gcc41-mt \
	  -l boost_serialization-gcc41-mt \
	  -l boost_thread-gcc41-mt \
	  -l boost_filesystem-gcc41-mt \
	  -l boost_date_time-gcc41-mt \
	  -l boost_unit_test_framework-gcc41-mt \

%.o : %.cpp
	$(CXX) $(INCLUDE_PATH) $(CXXFLAGS) -c -o $@ $^ 

all: $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CXX) -o $@ $^ $(LIB_FLAGS)

clean:
	rm -rf $(OBJECTS) $(TARGET) 

