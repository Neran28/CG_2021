CC=g++
AR=ar
ARFLAGS= rcs
CFLAGS=-c -Wall -std=c++17 -Wunreachable-code -Wunreachable-code -Xclang -fopenmp
LFLAGS=-lglfw -lGLEW -framework OpenGL -L../Utils -lutils 
# for linux use this line LFLAGS=-lglfw -lGLEW -lGL -L../Utils -lutils
LIBS=-lomp -L ../../openmp/lib
INCLUDES=-I. -I../Utils -I ../../openmp/include
SRC = Mat3.cpp GLApp.cpp GLDebug.cpp Vec4.cpp Mat4.cpp Vec3.cpp Rand.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = libutils.a

all: $(TARGET)

release: CFLAGS += -O3 -DNDEBUG
release: $(TARGET)

$(TARGET): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

clean:
	-rm -rf $(OBJ) $(TARGET) core

