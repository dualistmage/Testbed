OUT= t_svd 
all: $(OUT)

GPP=/usr/bin/g++
GCC=/usr/bin/gcc

CC_OPTIONS= -DHAVE_INLINE -O3 -ftree-vectorize -Wall -c -g

INC_PATH= -I. 

.cpp.o:
	$(GPP) $(INC_PATH) $(CC_OPTIONS) $<
.cc.o:
	$(GPP) $(INC_PATH) $(CC_OPTIONS) $<
.c.o:
	$(GCC) $(INC_PATH) $(CC_OPTIONS) $<

t_svd_OBJ = svd.o t_svd.o d-mat2d.o
t_svd: $(t_svd_OBJ)
	$(GPP) $(t_svd_OBJ) -o $@

clean:
	rm -rf *.o *~
	rm $(OUT)

