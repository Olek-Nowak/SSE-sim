CXX=g++
IDIR=./include
BDIR=./build
SDIR=./src
LDIR=./lib
SFINC=C:\Users\ANowa\Documents\SFML-2.5.1\include
SFLIB=C:\Users\ANowa\Documents\SFML-2.5.1\lib
COMPILEFLAGS=-I$(IDIR) -I$(SFINC) -DSFML_STATIC -std=c++11
LINKFLAGS=-L$(SFLIB) -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -mwindows
_DEPS=register.h windowManager.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))
_OBJ=main.o register.o windowManager.o
OBJ=$(patsubst %,$(BDIR)/%,$(_OBJ))
$(BDIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(COMPILEFLAGS)
$(SDIR)/main: $(OBJ)
	$(CXX) -o $@ $^ $(LINKFLAGS)