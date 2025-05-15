NES = ..

# libpng location
INC = -I/opt/homebrew/include -I$(NES)
LIB = -L/opt/homebrew/lib

OPTIONS = -arch arm64 -std=c++17 -mmacosx-version-min=10.15 -Wall
LIBRARIES = -framework OpenGL -framework GLUT -framework Carbon -lpng
CPP = $(NES)/Bus.cpp $(NES)/CPU.cpp $(NES)/PPU.cpp $(NES)/Cartridge.cpp $(NES)/Mapper.cpp $(NES)/Mapper000.cpp demo2.cpp
EXE = demo2

all:
	clang++ $(OPTIONS) $(INC) $(LIB) $(LIBRARIES) $(CPP) -o $(EXE)
clean:
	-rm $(EXE)
