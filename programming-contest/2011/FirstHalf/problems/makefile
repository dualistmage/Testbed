OUT=  t_grade
all: $(OUT)

GPP=/usr/bin/g++
GCC=/usr/bin/gcc

CC_OPTIONS= -DHAVE_INLINE -DGSL_RANGE_CHECK_OF -Wall -c -O3

INC_PATH= -I. 

.cc.o:
	$(GPP) $(INC_PATH) $(CC_OPTIONS) $<
.c.o:
	$(GCC) $(INC_PATH) $(CC_OPTIONS) $<

t_grade_OBJ = t_grade.o
t_grade: $(t_grade_OBJ)
	$(GPP) $(t_grade_OBJ) $(LIBS) -o $@

clean:
	rm -rf *.o *~ 
	rm $(OUT)

