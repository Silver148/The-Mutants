NAME = The_Mutants
CC = gcc
CFLAGS = -Wall -Iinclude
LIBS = -lSDL2 -lSDL2_image
SOURCES = src/main.c src/player.c src/states/state_menu.c \
			src/states/state_game.c
OBJECTS = $(SOURCES:src/%.c=obj/%.o)

# Windows x64 build settings
WIN_NAME = The_Mutants.exe
WIN_CC = x86_64-w64-mingw32-gcc
WIN_CFLAGS = -Iinclude -LSDL2-Mingw/x86_64-w64-mingw32/lib -ISDL2-Mingw/x86_64-w64-mingw32/include
WIN_LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -mwindows  \
			-Wl,--dynamicbase -Wl,--nxcompat \
			-Wl,--high-entropy-va -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 \
			-lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid

# Windows x32 build settings
WIN32_NAME = The_Mutants_32.exe
WIN32_CC = i686-w64-mingw32-gcc
WIN32_CFLAGS = -Iinclude -LSDL2-Mingw/i686-w64-mingw32/lib -ISDL2-Mingw/i686-w64-mingw32/include
WIN32_LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -mwindows \
			-Wl,--dynamicbase -Wl,--nxcompat -Wl,-lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 \
			-lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid

OS := $(shell uname -s)
ARCH := $(shell uname -m)

all: $(OS)-$(ARCH)

Linux-x86_64:
	make $(NAME)
	zip -r The_Mutants_Linux.zip $(NAME) sprites
	@echo "BUILD COMPLETE... Compiled from the $(OS)-$(ARCH)"

Linux-i686:
	make $(NAME) "CFLAGS=$(CFLAGS) -m32"
	zip -r The_Mutants_Linux_32.zip $(NAME) sprites
	@echo "BUILD COMPLETE... Compiled from the $(OS)-$(ARCH)"

Windows_NT-x86_64:
	make $(WIN_NAME) CC=$(WIN_CC)
	cp SDL2-Mingw/x86_64-w64-mingw32/bin/SDL2.dll SDL2.dll
	cp SDL2-Mingw/x86_64-w64-mingw32/bin/libpng16-16.dll libpng16-16.dll
	cp SDL2-Mingw/x86_64-w64-mingw32/bin/zlib1.dll zlib1.dll
	cp SDL2-Mingw/x86_64-w64-mingw32/bin/libjpeg-9.dll libjpeg-9.dll
	cp SDL2-Mingw/x86_64-w64-mingw32/bin/SDL2_image.dll SDL2_image.dll
	cp SDL2-Mingw/x86_64-w64-mingw32/bin/libtiff-5.dll libtiff-5.dll
	zip -r The_Mutants_windows.zip $(WIN_NAME) SDL2.dll libpng16-16.dll zlib1.dll libjpeg-9.dll SDL2_image.dll libtiff-5.dll sprites
	rm -f *.dll
	rm -f $(WIN_NAME)
	@echo "BUILD COMPLETE... Compiled from the $(OS)-$(ARCH)"

Windows_NT-i686:
	make $(WIN32_NAME) CC=$(WIN32_CC)
	cp SDL2-Mingw/i686-w64-mingw32/bin/SDL2.dll SDL2.dll
	cp SDL2-Mingw/i686-w64-mingw32/bin/libpng16-16.dll libpng16-16.dll
	cp SDL2-Mingw/i686-w64-mingw32/bin/zlib1.dll zlib1.dll
	cp SDL2-Mingw/i686-w64-mingw32/bin/libjpeg-9.dll libjpeg-9.dll
	cp SDL2-Mingw/i686-w64-mingw32/bin/SDL2_image.dll SDL2_image.dll
	cp SDL2-Mingw/i686-w64-mingw32/bin/libtiff-5.dll libtiff-5.dll
	zip -r The_Mutants_windows32.zip $(WIN32_NAME) SDL2.dll libpng16-16.dll zlib1.dll libjpeg-9.dll SDL2_image.dll libtiff-5.dll sprites
	rm -f *.dll
	rm -f $(WIN32_NAME)
	@echo "BUILD COMPLETE... Compiled from the $(OS)-$(ARCH)"

clean:
	rm -rf obj $(NAME) $(WIN_NAME) $(WIN32_NAME) *.zip

#RULE LINUX
$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME) $(LIBS)

#RULE WINDOWS X64
$(WIN_NAME): $(OBJECTS)
	$(WIN_CC) $(WIN_CFLAGS) $(OBJECTS) -o $(WIN_NAME) $(WIN_LIBS)

# RULE WINDOWS 32-BIT
$(WIN32_NAME): $(OBJECTS)
	$(WIN32_CC) $(WIN32_CFLAGS) $(OBJECTS) -o $(WIN32_NAME) $(WIN32_LIBS)

obj/%.o: src/%.c
	mkdir -p obj
	mkdir -p obj/states
ifeq ($(CC), x86_64-w64-mingw32-gcc)
	$(CC) $(WIN_CFLAGS) -c $< -o $@
else ifeq ($(CC), i686-w64-mingw32-gcc)
	$(CC) $(WIN32_CFLAGS) -c $< -o $@
else
	$(CC) $(CFLAGS) -c $< -o $@
endif
