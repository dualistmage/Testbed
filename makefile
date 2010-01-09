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
			   -I $(BOOST)/include \
			   -I $(WISELIB)/include \
			   -I $(IZENELIB)/include \
			   -I $(GLOG_PATH)/include \
			   -I $(SF1V5_ROOT)/source/core/include \

LIB_FLAGS = $(LIB_PATH) $(LIB)

LIB_PATH = \
		   -L $(BOOST)/lib \
		   -L $(WISELIB)/lib \
		   -L $(IZENELIB)/lib \
		   -L $(GLOG_PATH)/lib \

LIB = \
	  -l boost_system-gcc41-mt \
	  -l boost_serialization-gcc41-mt \
	  -l boost_thread-gcc41-mt \
	  -l boost_filesystem-gcc41-mt \
	  -l boost_date_time-gcc41-mt \
	  -l boost_unit_test_framework-gcc41-mt \
	  -l wiselib \
	  -l procmeminfo \
	  -l glog \

%.o : %.cpp
	$(CXX) $(INCLUDE_PATH) $(CXXFLAGS) -c -o $@ $^ 

all: $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CXX) -o $@ $^ $(LIB_FLAGS)

clean:
	rm -rf *.o $(TARGET) 

