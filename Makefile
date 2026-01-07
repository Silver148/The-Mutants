#MAKEFILE POR JUAN YAGUARO :D

#NAME = The_Mutants
#CC = gcc
#CFLAGS = -Wall -Iinclude
#LIBS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lm
SOURCES = src/main.c src/player.c src/states/state_menu.c \
			src/states/state_game.c src/zombies.c src/delta_time.c src/anim_manager.c \
			src/music.c src/settings.c src/projectiles.c src/zombie_waves.c src/update_system.c \
			src/show_notifications.c src/system_cinematics.c
OBJECTS = $(SOURCES:src/%.c=obj/%.o)

# Windows x64 build settings
WIN_NAME = The_Mutants.exe
WIN_CC = x86_64-w64-mingw32-gcc
WIN_CFLAGS = -Iinclude -Icurl/include -Lcurl/lib/x64 -LSDL2-Mingw/x86_64-w64-mingw32/lib -ISDL2-Mingw/x86_64-w64-mingw32/include -Iffmpeg/include -Lffmpeg/lib/x64 \
			-march=x86-64 -O2 -msse2 -mfpmath=sse
WIN_LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lcurl -mwindows \
			-Wl,--dynamicbase -Wl,--nxcompat \
			-Wl,--high-entropy-va -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 \
			-lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid -lavcodec -lavformat -lavutil

# Windows x32 build settings
WIN32_NAME = The_Mutants_32.exe
WIN32_CC = i686-w64-mingw32-gcc
WIN32_CFLAGS = -Iinclude -Icurl/include -Lcurl/lib/x32 -LSDL2-Mingw/i686-w64-mingw32/lib -ISDL2-Mingw/i686-w64-mingw32/include -Iffmpeg/include -Lffmpeg/lib/x32 \
				-O2 -march=i686 -msse2 -mfpmath=sse
WIN32_LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lcurl -mwindows \
			-Wl,--dynamicbase -Wl,--nxcompat -Wl,-lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 \
			-lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid -lavcodec -lavformat -lavutil

OS := $(shell uname -s)
ARCH := $(shell uname -m)

CURRENT_CFLAGS = $(WIN_CFLAGS)

all: $(OS)-$(ARCH)

Windows_NT-x86_64:
	make $(WIN_NAME) CC=$(WIN_CC)
	cp SDL2-Mingw/x86_64-w64-mingw32/bin/SDL2.dll SDL2.dll
	cp SDL2-Mingw/x86_64-w64-mingw32/bin/libpng16-16.dll libpng16-16.dll
	cp SDL2-Mingw/x86_64-w64-mingw32/bin/zlib1.dll zlib1.dll
	cp SDL2-Mingw/x86_64-w64-mingw32/bin/libjpeg-9.dll libjpeg-9.dll
	cp SDL2-Mingw/x86_64-w64-mingw32/bin/SDL2_image.dll SDL2_image.dll
	cp SDL2-Mingw/x86_64-w64-mingw32/bin/libtiff-5.dll libtiff-5.dll
	cp SDL2-Mingw/x86_64-w64-mingw32/bin/SDL2_mixer.dll SDL2_mixer.dll
	cp SDL2-Mingw/x86_64-w64-mingw32/bin/SDL2_ttf.dll SDL2_ttf.dll
	cp ffmpeg/bin/x64/avcodec-62.dll avcodec-62.dll
	cp ffmpeg/bin/x64/avformat-62.dll avformat-62.dll
	cp ffmpeg/bin/x64/avutil-60.dll avutil-60.dll
	cp curl/bin/x64/libcurl-x64.dll libcurl-x64.dll
	zip -r The_Mutants_windows.zip $(WIN_NAME) SDL2.dll libpng16-16.dll zlib1.dll libjpeg-9.dll SDL2_image.dll libtiff-5.dll SDL2_mixer.dll SDL2_ttf.dll libcurl-x64.dll \
	sprites music fonts skins avcodec-62.dll avformat-62.dll avutil-60.dll cinematics
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
	cp SDL2-Mingw/i686-w64-mingw32/bin/SDL2_mixer.dll SDL2_mixer.dll
	cp SDL2-Mingw/i686-w64-mingw32/bin/SDL2_ttf.dll SDL2_ttf.dll
	cp ffmpeg/bin/x32/avcodec-62.dll avcodec-62.dll
	cp ffmpeg/bin/x32/avformat-62.dll avformat-62.dll
	cp ffmpeg/bin/x32/avutil-60.dll avutil-60.dll
	cp curl/bin/x32/libcurl.dll libcurl.dll
	zip -r The_Mutants_windows32.zip $(WIN32_NAME) SDL2.dll libpng16-16.dll zlib1.dll libjpeg-9.dll SDL2_image.dll libtiff-5.dll SDL2_mixer.dll SDL2_ttf.dll libcurl.dll \
	sprites music fonts skins avcodec-62.dll avformat-62.dll avutil-60.dll avdevice-62.dll avfilter-11.dll cinematics
	rm -f *.dll
	rm -f $(WIN32_NAME)
	@echo "BUILD COMPLETE... Compiled from the $(OS)-$(ARCH)"

debug-windows-x64:
	make $(WIN_NAME) CC=$(WIN_CC) "WIN_CFLAGS=$(WIN_CFLAGS) -g -ggdb3 -Og -DDEBUG -DANIM_DEBUG"
	cp SDL2-Mingw/x86_64-w64-mingw32/bin/SDL2.dll SDL2.dll
	cp SDL2-Mingw/x86_64-w64-mingw32/bin/libpng16-16.dll libpng16-16.dll
	cp SDL2-Mingw/x86_64-w64-mingw32/bin/zlib1.dll zlib1.dll
	cp SDL2-Mingw/x86_64-w64-mingw32/bin/libjpeg-9.dll libjpeg-9.dll
	cp SDL2-Mingw/x86_64-w64-mingw32/bin/SDL2_image.dll SDL2_image.dll
	cp SDL2-Mingw/x86_64-w64-mingw32/bin/libtiff-5.dll libtiff-5.dll
	cp SDL2-Mingw/x86_64-w64-mingw32/bin/SDL2_mixer.dll SDL2_mixer.dll
	cp SDL2-Mingw/x86_64-w64-mingw32/bin/SDL2_ttf.dll SDL2_ttf.dll
	cp ffmpeg/bin/avcodec-62.dll avcodec-62.dll
	cp ffmpeg/bin/avformat-62.dll avformat-62.dll
	cp ffmpeg/bin/avutil-60.dll avutil-60.dll
	cp curl/bin/libcurl-x64.dll libcurl-x64.dll
	zip -r The_Mutants_windows-debug.zip $(WIN_NAME) SDL2.dll libpng16-16.dll zlib1.dll libjpeg-9.dll SDL2_image.dll libtiff-5.dll SDL2_mixer.dll SDL2_ttf.dll libcurl-x64.dll \
	sprites music fonts skins avcodec-62.dll avformat-62.dll avutil-60.dll cinematics
	rm -f *.dll
	rm -f $(WIN_NAME)
	@echo "DEBUG BUILD COMPLETE... Compiled from the $(OS)-$(ARCH)"

debug-windows-x32:
	make $(WIN32_NAME) CC=$(WIN32_CC) "WIN32_CFLAGS=$(WIN32_CFLAGS) -g -ggdb3 -Og -DDEBUG -DANIM_DEBUG"
	cp SDL2-Mingw/i686-w64-mingw32/bin/SDL2.dll SDL2.dll
	cp SDL2-Mingw/i686-w64-mingw32/bin/libpng16-16.dll libpng16-16.dll
	cp SDL2-Mingw/i686-w64-mingw32/bin/zlib1.dll zlib1.dll
	cp SDL2-Mingw/i686-w64-mingw32/bin/libjpeg-9.dll libjpeg-9.dll
	cp SDL2-Mingw/i686-w64-mingw32/bin/SDL2_image.dll SDL2_image.dll
	cp SDL2-Mingw/i686-w64-mingw32/bin/libtiff-5.dll libtiff-5.dll
	cp SDL2-Mingw/i686-w64-mingw32/bin/SDL2_mixer.dll SDL2_mixer.dll
	cp SDL2-Mingw/i686-w64-mingw32/bin/SDL2_ttf.dll SDL2_ttf.dll
	cp ffmpeg/bin/x32/avcodec-62.dll avcodec-62.dll
	cp ffmpeg/bin/x32/avformat-62.dll avformat-62.dll
	cp ffmpeg/bin/x32/avutil-60.dll avutil-60.dll
	cp ffmpeg/bin/x32/swresample-6.dll swresample-6.dll
#	cp ffmpeg/bin/x32/avdevice-62.dll avdevice-62.dll
#	cp ffmpeg/bin/x32/avfilter-11.dll avfilter-11.dll
#	cp ffmpeg/bin/x32/swresample-6.dll swresample-6.dll
	cp curl/bin/x32/libcurl.dll libcurl.dll
	zip -r The_Mutants_windows32-debug.zip $(WIN32_NAME) SDL2.dll libpng16-16.dll zlib1.dll libjpeg-9.dll SDL2_image.dll libtiff-5.dll SDL2_mixer.dll SDL2_ttf.dll libcurl.dll \
	sprites music fonts skins avcodec-62.dll avformat-62.dll avutil-60.dll swresample-6.dll avdevice-62.dll avfilter-11.dll cinematics
	rm -f *.dll
	rm -f $(WIN32_NAME)
	@echo "DEBUG BUILD COMPLETE... Compiled from the $(OS)-$(ARCH)"

tools_dev:
#	gcc new_commit.c -o new_commit
#	gcc convert_gif_to_sequencePNG.c -o convert_gif_to_sequencePNG
	$(WIN_CC) new_commit.c -o new_commit.exe
	$(WIN_CC) convert_gif_to_sequencePNG.c -o convert_gif_to_sequencePNG.exe
	$(WIN32_CC) new_commit.c -o new_commit_32.exe
	$(WIN32_CC) convert_gif_to_sequencePNG.c -o convert_gif_to_sequencePNG_32.exe

create_icon_exe:
	windres resource.rc -o resource.o

clean:
	rm -rf obj $(NAME) $(WIN_NAME) $(WIN32_NAME) *.zip new_commit *.exe

#RULE LINUX
#$(NAME): $(OBJECTS)
#	$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME) $(LIBS)

#RULE WINDOWS X64
$(WIN_NAME): CURRENT_CFLAGS = $(WIN_CFLAGS)
$(WIN_NAME): $(OBJECTS)
	$(WIN_CC) $(WIN_CFLAGS) $(OBJECTS) resource.o -o $(WIN_NAME) $(WIN_LIBS)

# RULE WINDOWS 32-BIT
$(WIN32_NAME): CURRENT_CFLAGS = $(WIN32_CFLAGS)
$(WIN32_NAME): $(OBJECTS)
	$(WIN32_CC) $(WIN32_CFLAGS) $(OBJECTS) resource32.o -o $(WIN32_NAME) $(WIN32_LIBS)

obj/%.o: src/%.c
	mkdir -p obj
	mkdir -p obj/states
	$(CC) $(CURRENT_CFLAGS) -c $< -o $@
