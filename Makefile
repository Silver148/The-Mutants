NAME = The_Mutants
CC = gcc
CFLAGS = -Wall -Iinclude
LIBS = -lSDL2
SOURCES = src/main.c
OBJECTS = $(SOURCES:src/%.c=obj/%.o)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME) $(LIBS)

obj/%.o: src/%.c
	mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

clean:
	rm -rf obj $(NAME)