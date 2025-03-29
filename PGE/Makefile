# libpng location
INC = -I/opt/homebrew/include
LIB = -L/opt/homebrew/lib

LIBRARIES = -framework OpenGL -framework GLUT -framework Carbon -lpng

EXE = example

all:
	clang++ -arch arm64 -std=c++17 -mmacosx-version-min=10.15 -Wall $(INC) $(LIB) $(LIBRARIES) example.cpp -o $(EXE)
clean:
	-rm $(EXE)
