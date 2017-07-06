TARGET=$(shell basename `pwd`)
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:%.c=%.o)


all: $(TARGET)

$(OBJECTS): $(SOURCES)

$(TARGET): $(OBJECTS) -lncursesw -lpthread

clean:
	$(RM) $(OBJECTS) $(TARGET)

.PHONY: all clean



