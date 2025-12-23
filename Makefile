CC = gcc

CFLAGS = $(shell pkg-config --cflags gtk4)
LDLIBS = $(shell pkg-config --libs gtk4)
WARNINGFLAGS = -Wall -Wextra

SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:.c=.o)
DEPS = $(OBJS:.o=.d)

calc: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDLIBS)

run: calc
	./calc

src/%.o: src/%.c
	$(CC) $(CFLAGS) $(WARNINGFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

clean:
	rm -f calc $(OBJS) $(DEPS)
