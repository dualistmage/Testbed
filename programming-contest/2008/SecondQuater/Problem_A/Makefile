CC = gcc
RM = rm -f

INCLUDE_DIRS = -I.
CC_FLAGS = -c -O -Wall -ansi
LINK_LIBS = -lstdc++

EXE_OUT = Problem_A 
OBJS = Problem_A.o

all:$(EXE_OUT)

$(EXE_OUT): $(OBJS)
	$(CC) -o $(EXE_OUT) $(OBJS) $(LINK_LIBS)

%.o: %.cpp
	$(CC) $(CC_FLAGS) $(INCLUDE_DIRS) -o $@ $<

clean:
	$(RM) $(OBJS) $(EXE_OUT)
