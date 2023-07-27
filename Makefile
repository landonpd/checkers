TARGET = FP

SRC_FILES = main.cpp Square.cpp Square.h Pos.h Pos.cpp Piece.cpp Piece.h 

CXX = g++
CFLAGS = -Wall -g -std=c++11

# For OS X using an Intel chip, use the following line
#CXX_FLAGS = 
# For OS X using an M1 chip, use the following line
#CXX_FLAGS = -arch x86_64

INC_PATH = /usr/local/include/
LIB_PATH = /usr/local/lib/
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network

# do not modify anything below this line

OBJECTS = $(SRC_FILES:.cpp=.o)

ifeq ($(shell echo "Windows"), "Windows")
	TARGET := $(TARGET).exe
	DEL = del
else
	DEL = rm
endif

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) ${CXX_FLAGS} -o $@ $^ -L${LIB_PATH} ${LIBS}

.cpp.o:
	$(CXX) ${CXX_FLAGS} $(CFLAGS) -o $@ -c $< -I${INC_PATH} 

clean:
	$(DEL) $(TARGET) $(OBJECTS)

# DEPENDENCIES
main.o:Pos.h Piece.h Square.h 
Piece.o: Piece.h Pos.h 
Square.o: Square.h Pos.h Piece.h
Pos.o:pos.h
