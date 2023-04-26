CC = gcc
CFLAGS = -Wall -std=c99 \
         -I/usr/include/libxml2 \
         -D_THREAD_SAFE \
         -I/opt/homebrew/include \
         -I/opt/homebrew/include/SDL2 \
         -I/usr/local/include \
         -I/usr/local/Homebrew/include \
         -I/opt/homebrew/include \
         -I/opt/homebrew/Cellar/sdl2_ttf/2.20.2/include/SDL2
LDFLAGS = -lxml2 \
          -L/usr/local/lib \
          -L/opt/homebrew/lib \
          -lproj \
          -L/opt/homebrew/lib \
          -lSDL2 \
          -lSDL2_ttf
TARGET = main
SRCS = readMap.c storeData.c main.c visualization.c findRoute.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c dataParser.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS) data.txt