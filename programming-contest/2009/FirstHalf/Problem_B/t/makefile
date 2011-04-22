or: all
CC= g++
CC_OPTIONS = -DHAVE_INLINE -g -Wall -c

INC_PATH= -I. -I/home/yeogirl/projects/ylib
LIB_PATH= -L/home/yeogirl/projects/ylib
LIBS= -ly 

.cc.o:
	$(CC) $(INC_PATH) $(CC_OPTIONS) $<

evalb_OBJ = evalb.o
evalb: $(evalb_OBJ) 
	$(CC) $(evalb_OBJ) $(LIB_PATH) $(LIBS) -o $@

all:
	make evalb

clean:
	rm *.o *~
