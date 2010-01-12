###########################################################
# Makefile - Dohyun yun
###########################################################

JAVAC = javac
JAVAC_FLAGS = -Xlint -classpath . -d .
JAVA_FLAGS = -classpath .
JAR = jar
TARGET = test.jar

OBJS = $(patsubst %.java, %.class, $(wildcard 	./testPackage/*.java ))

all: $(TARGET)

test:
	javac $(JAVAC_FLAGS) test.java
	java $(JAVA_FLAGS):$(TARGET):$(CLASSPATH) test

$(TARGET): $(OBJS)
	$(JAR) cf $(TARGET) $(OBJS)

%.class : %.java
	$(JAVAC) $(JAVAC_FLAGS) $<

clean:
	rm -rf *.class $(OBJS) $(TARGET)
