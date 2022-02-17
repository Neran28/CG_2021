CC=g++
CFLAGS=-c -Wall -std=c++17 -Wunreachable-code
LFLAGS=-lglfw -lGLEW -framework OpenGL 
# for linux use this line LFLAGS=-lglfw -lGLEW -lGL
LIBS=
INCLUDES=-I. 
SRC = main.cpp GLDebug.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = hello_opengl

all: $(TARGET)

release: CFLAGS += -O3 -DNDEBUG
release: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(INCLUDES) $^ $(LFLAGS) $(LIBS) -o $@


%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

clean:
	-rm -rf $(OBJ) $(TARGET) core
