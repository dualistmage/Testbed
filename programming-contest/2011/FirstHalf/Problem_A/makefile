CC = g++
RM = rm -f

INCLUDE_DIRS = -I.
#CC_FLAGS = -c -O1 -Wall -ansi -g
CC_FLAGS = -O1

EXE_OUT1 = Problem_A
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
