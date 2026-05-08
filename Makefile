# Fordító és alapvető flag-ek
CC = gcc
CFLAGS = -Wall -std=c99 -Iinclude -O3

# Alapértelmezett LDFLAGS (Minden platformra közös)
# Itt adjuk hozzá az enet-et
COMMON_LDFLAGS = -lenet

# Rendszerfüggő beállítások
ifeq ($(OS),Windows_NT)
    LDFLAGS = -Llib -lraylib -lopengl32 -lgdi32 -lwinmm $(COMMON_LDFLAGS)
    TARGET = chaos_rts.exe
else
    # Linux/Unix esetén az enet-et is bele kell fűzni
    LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 $(COMMON_LDFLAGS)
    TARGET = chaos_rts
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
	rm -f src/*.o src/modules/*.o $(TARGET)
