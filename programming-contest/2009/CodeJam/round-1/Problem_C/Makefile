CC = gcc
RM = rm -f

INCLUDE_DIRS = -I.
CC_FLAGS = -c -O1 -Wall -ansi -g
LINK_LIBS = -lstdc++

EXE_OUT = Problem_C
OBJS = Problem_C.o

all:$(EXE_OUT)

$(EXE_OUT): $(OBJS)
	$(CC) -o $(EXE_OUT) $(OBJS) $(LINK_LIBS)
	$(RM) $(OBJS)

%.o: %.cpp
	$(CC) $(CC_FLAGS) $(INCLUDE_DIRS) -o $@ $<

clean:
	$(RM) $(OBJS) $(EXE_OUT)
