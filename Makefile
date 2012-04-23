OBJS = main.o log.o startup.o monster.o turret.o draw.o decal.o bullet.o bitmap.o sound.o candy.o input.o

CC = gcc
CFLAGS = -Wall
LDFLAGS = `allegro-config --libs`

LDFLAGS_WIN32 = -Wl,--subsystem,windows -lalleg

OUTPUT = window
OUTPUT_WIN32 = window.exe

ifeq ($(WIN32_STATIC),1)
	CFLAGS := -DALLEGRO_STATICLINK -O2
	LDFLAGS_WIN32 = -DALLEGRO_STATICLINK -Wl,--subsystem,windows -lalleg_s -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lole32 -ldinput -lddraw -ldxguid -lwinmm -ldsound
endif


all:
	@echo "make linux"
	@echo "make win32"
	@echo "make win32_static"

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

linux: $(OBJS)
	$(CC) $^ -o $(OUTPUT) $(LDFLAGS)

win32: $(OBJS)
	$(CC) $^ -o $(OUTPUT_WIN32) $(LDFLAGS_WIN32)

win32_static:
	WIN32_STATIC=1 mingw32-make win32

clean:
	rm -f *.o window

clean_win32:
	del /F *.o window.exe
