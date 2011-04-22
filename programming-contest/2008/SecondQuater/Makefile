DIRS = Problem_A Problem_B Problem_C

all:
	@for dir in $(DIRS); do \
		(cd $$dir && make -f Makefile $@) || exit 1; \
	done

clean:
	@for dir in $(DIRS); do \
		(cd $$dir && make -f Makefile $@) || exit 1; \
	done
