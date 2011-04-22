CC = g++
RM = rm -f

INCLUDE_DIRS = -I.
#CC_FLAGS = -c -O1 -Wall -ansi -g
CC_FLAGS = -O1

EXE_OUT1 = Problem_A_1
OBJ1 = $(EXE_OUT1).o Profile.o

EXE_OUT2 = Problem_A_2
OBJ2 = $(EXE_OUT2).o Profile.o

all:$(EXE_OUT1) $(EXE_OUT2)

$(EXE_OUT1): $(OBJ1)
	$(CC) -o $(EXE_OUT1) $^

$(EXE_OUT2): $(OBJ2)
	$(CC) -o $(EXE_OUT2) $^

%.o: %.cpp
	$(CC) $(CC_FLAGS) $(INCLUDE_DIRS) -c -o $@ $?

clean:
	$(RM) *.o \
		$(OBJ1) \
		$(OBJ2) \
		$(EXE_OUT1) \
		$(EXE_OUT2) \
		./profile/* \
		sample/your_sample.profile
