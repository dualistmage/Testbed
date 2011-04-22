CC = g++
RM = rm -f

INCLUDE_DIRS = -I.
CC_FLAGS = -c -O -Wall -g

OBJECT = Problem_B.o

TARGET = Problem_B

all:$(TARGET)

$(TARGET): $(OBJECT)
	$(CC) -o $(TARGET) $(OBJECT)
	$(RM) $(OBJECT)

%.o: %.cpp
	$(CC) $(CC_FLAGS) $(INCLUDE_DIRS) -o $@ $<

clean:
	$(RM) $(OBJECT) $(TARGET)
