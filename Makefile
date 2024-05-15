TARGET := app

SRC := $(wildcard src/*.cpp)
OBJ := $(patsubst src/%.cpp,build/%.o,$(SRC))

CC := g++
CC_FLAGS := -Wall -Wextra -pedantic
LD_FLAGS := -lSDL2 -lSDL2_image

all: $(TARGET)

$(TARGET): build $(OBJ)
	$(CC) $(CC_FLAGS) -o $(TARGET) $(OBJ) $(LD_FLAGS)

build:
	mkdir -p build

build/%.o: src/%.cpp
	$(CC) $(CC_FLAGS) $(LD_FLAGS) -c $< -o $@

clean:
	rm -rf build $(TARGET)

run: all
	./$(TARGET)
