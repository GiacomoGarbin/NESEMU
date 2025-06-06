# libpng location
INC = -I/opt/homebrew/include
LIB = -L/opt/homebrew/lib

OPTIONS = -arch arm64 -std=c++17 -mmacosx-version-min=10.15 -Wall
LIBRARIES = -framework OpenGL -framework GLUT -framework Carbon -lpng
CPP = Bus.cpp CPU.cpp PPU.cpp Cartridge.cpp Mapper.cpp Mapper000.cpp demo.cpp
EXE = demo

all:
	clang++ $(OPTIONS) $(INC) $(LIB) $(LIBRARIES) $(CPP) -o $(EXE)
clean:
	-rm $(EXE)
