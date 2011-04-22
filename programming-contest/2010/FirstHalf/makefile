DIRS = Problem_A Problem_B

all:
	@for dir in $(DIRS); do \
		(cd $$dir && make -f makefile $@) || exit 1; \
	done

clean:
	@for dir in $(DIRS); do \
		(cd $$dir && make -f makefile $@) || exit 1; \
	done
