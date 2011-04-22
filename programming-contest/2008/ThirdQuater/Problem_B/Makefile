CC = gcc
RM = rm -f

INCLUDE_DIRS = -I.
CC_FLAGS = -c -O -Wall -ansi
LINK_LIBS = -lstdc++

OBJS = $(CSR_OBJ) $(CSRDU_OBJ) $(CSRVI_OBJ)
CSR_OBJ = CSR.o
CSRDU_OBJ = CSR-DU.o
CSRVI_OBJ = CSR-VI.o

EXE_OUT = $(CSR_EXE) $(CSRDU_EXE) $(CSRVI_EXE)
CSR_EXE = CSR
CSRDU_EXE = CSR-DU
CSRVI_EXE = CSR-VI
all:$(EXE_OUT)

$(EXE_OUT): $(OBJS)
	$(CC) -o $(CSR_EXE) $(CSR_OBJ) $(LINK_LIBS)
	$(CC) -o $(CSRDU_EXE) $(CSRDU_OBJ) $(LINK_LIBS)
	$(CC) -o $(CSRVI_EXE) $(CSRVI_OBJ) $(LINK_LIBS)

%.o: %.cpp
	$(CC) $(CC_FLAGS) $(INCLUDE_DIRS) -o $@ $<

clean:
	$(RM) $(OBJS) $(EXE_OUT)
