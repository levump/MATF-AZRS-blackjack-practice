CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = blackjack

$(TARGET) : main.c 
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -rf $(TARGET) *.o *core
