NAME = The_Mutants
CC = gcc
CFLAGS = -Wall -Iinclude
LIBS = -lSDL2
SOURCES = src/main.c
OBJECTS = $(SOURCES:src/%.c=obj/%.o)

# Windows build settings
WIN_NAME = The_Mutants.exe
WIN_CC = x86_64-w64-mingw32-gcc
WIN_CFLAGS = -Iinclude -LSDL2-Mingw/x86_64-w64-mingw32/lib
WIN_LIBS = -lmingw32 -lwinmm -luser32 -lgdi32 -ldxguid -mwindows -lSDL2main -lSDL2

#RULE LINUX
$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME) $(LIBS)

#RULE WINDOWS
$(WIN_NAME): $(OBJECTS)
	$(WIN_CC) $(WIN_CFLAGS) $(OBJECTS) -o $(WIN_NAME) $(WIN_LIBS)

obj/%.o: src/%.c
	mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

windows: $(WIN_NAME)

clean:
	rm -rf obj $(NAME) $(WIN_NAME)