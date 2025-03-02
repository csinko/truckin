CFLAGS  = -O3 -pthread -Wall -Wextra -std=c99
TARGET  = tkn
SRC     = main.c
PREFIX  ?= /usr/local

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

install: $(TARGET)
	mkdir -p $(PREFIX)/bin
	cp $(TARGET) $(PREFIX)/bin/

clean:
	rm -f $(TARGET)

