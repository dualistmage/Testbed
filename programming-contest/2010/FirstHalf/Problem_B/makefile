CC = g++
RM = rm -f

INCLUDE_DIRS = -I.
CC_FLAGS = -O3
#CC_FLAGS = -c -Wall -ansi -g

EXE_OUT = Problem_B
OBJ = $(patsubst %.cpp, %.o, $(wildcard ./utility/*.cpp \
	  									*.cpp) )

all:$(EXE_OUT)

$(EXE_OUT): $(OBJ)
	$(CC) -o $(EXE_OUT) $^

%.o: %.cpp
	$(CC) $(CC_FLAGS) $(INCLUDE_DIRS) -c -o $@ $?

clean:
	$(RM) *.o \
		$(OBJ) \
		$(EXE_OUT)
