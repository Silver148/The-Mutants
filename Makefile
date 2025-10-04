NAME = The_Mutants
CC = gcc
CFLAGS = -Wall -Iinclude
LIBS = -lSDL2
SOURCES = src/main.c
OBJECTS = $(SOURCES:src/%.c=obj/%.o)

# Windows x64 build settings
WIN_NAME = The_Mutants.exe
WIN_CC = x86_64-w64-mingw32-gcc
WIN_CFLAGS = -Iinclude -LSDL2-Mingw/x86_64-w64-mingw32/lib -ISDL2-Mingw/x86_64-w64-mingw32/include
WIN_LIBS = -lmingw32 -lSDL2main -lSDL2 -mwindows  -Wl,--dynamicbase -Wl,--nxcompat -Wl,--high-entropy-va -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid

WIN32_NAME = The_Mutants_32.exe
WIN32_CC = i686-w64-mingw32-gcc
WIN32_CFLAGS = -Iinclude -LSDL2-Mingw/i686-w64-mingw32/lib -ISDL2-Mingw/i686-w64-mingw32/include
WIN32_LIBS = -lmingw32 -lSDL2main -lSDL2 -mwindows -Wl,--dynamicbase -Wl,--nxcompat -Wl,-lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid

#RULE LINUX
$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME) $(LIBS)

#RULE WINDOWS X64
$(WIN_NAME): $(OBJECTS)
	$(WIN_CC) $(WIN_CFLAGS) $(OBJECTS) -o $(WIN_NAME) $(WIN_LIBS)

# RULE WINDOWS 32-BIT
$(WIN32_NAME): $(OBJECTS)
	$(WIN32_CC) $(WIN32_CFLAGS) $(OBJECTS) -o $(WIN32_NAME) $(WIN32_LIBS)

windows32:

obj/%.o: src/%.c
	mkdir -p obj
ifeq ($(CC), x86_64-w64-mingw32-gcc)
	$(CC) $(WIN_CFLAGS) -c $< -o $@
else ifeq ($(CC), i686-w64-mingw32-gcc)
	$(CC) $(WIN32_CFLAGS) -c $< -o $@
else
	$(CC) $(CFLAGS) -c $< -o $@
endif

all: $(NAME)

windows: 
	make $(WIN_NAME) CC=$(WIN_CC)
	cp SDL2-Mingw/x86_64-w64-mingw32/bin/SDL2.dll SDL2.dll
	zip -r The_Mutants.zip $(WIN_NAME) SDL2.dll
	rm -f SDL2.dll
	rm -f $(WIN_NAME)

windows32:
	make $(WIN32_NAME) CC=$(WIN32_CC)
	cp SDL2-Mingw/i686-w64-mingw32/bin/SDL2.dll SDL2.dll
	zip -r The_Mutants_32.zip $(WIN32_NAME) SDL2.dll
	rm -f SDL2.dll
	rm -f $(WIN32_NAME)

clean:
	rm -rf obj $(NAME) $(WIN_NAME) *.zip