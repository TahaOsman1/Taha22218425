CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = cpe351

SRCS = cpe351.c scheduler.c linked_list.c utils.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

test: $(TARGET)
	./$(TARGET) input.txt output.txt
	@echo "\nOutput file contents:"
	@cat output.txt

debug: CFLAGS += -DDEBUG -O0
debug: clean all

.PHONY: all clean test debug
