CC = g++
RM = rm -f

INCLUDE_DIRS = -I.
#CC_FLAGS = -c -Wall -ansi -g
CC_FLAGS = -O3

EXE_OUT1 = Problem_B
OBJ1 = $(EXE_OUT1).o

all:$(EXE_OUT1)

$(EXE_OUT1): $(OBJ1)
	$(CC) -o $(EXE_OUT1) $^

%.o: %.cpp
	$(CC) $(CC_FLAGS) $(INCLUDE_DIRS) -c -o $@ $?

clean:
	$(RM) *.o \
		$(OBJ1) \
		$(EXE_OUT1)
