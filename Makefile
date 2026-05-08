# Fordító és alapvető flag-ek
CC = gcc
CFLAGS = -Wall -std=c99 -Iinclude -O3

# Alapértelmezett LDFLAGS
COMMON_LDFLAGS = -lenet

# Rendszerfüggő beállítások
ifeq ($(OS),Windows_NT)
    # Windowson kell a ws2_32 az ENet-hez
    LDFLAGS = -Llib -lraylib -lopengl32 -lgdi32 -lwinmm -lws2_32 $(COMMON_LDFLAGS)
    TARGET = chaos_rts.exe
    CLEAN_CMD = rm -f src/*.o src/modules/*.o $(TARGET)
else
    # Linux/Unix/macOS
    # macOS esetén gyakran kell az extra framework-ök, de a brew-s raylib kezeli
    LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 $(COMMON_LDFLAGS)
    TARGET = chaos_rts
    CLEAN_CMD = rm -f src/*.o src/modules/*.o $(TARGET)
endif

# Forrásfájlok automatikus keresése
SRC = $(wildcard src/*.c) $(wildcard src/modules/*.c)
OBJ = $(SRC:.c=.o)

# Fő szabály
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Objektum fájlok generálása
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

# Takarítás
clean:
	$(CLEAN_CMD)

