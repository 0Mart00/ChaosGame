# Fordító és alapvető flag-ek
CC = gcc
CFLAGS = -Wall -std=c99 -Iinclude -O3

# Raylib linkelése (Rendszerfüggő)
ifeq ($(OS),Windows_NT)
    LDFLAGS = -Llib -lraylib -lopengl32 -lgdi32 -lwinmm
    TARGET = chaos_rts.exe
else
    LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    TARGET = chaos_rts
endif

# Forrásfájlok
SRC = $(wildcard src/*.c) $(wildcard src/modules/*.c)

OBJ = $(SRC:.c=.o)

# Alapértelmezett szabály
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f src/*.o $(TARGET)
	rm -f src/modules/*.o $(TARGET)
