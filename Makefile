CXX=g++
IDIR=./include
BDIR=./build
SDIR=./src
LDIR=./lib
CXXFLAGS=-I$(IDIR)
_DEPS=template.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))
_OBJ=main.o template.o
OBJ=$(patsubst %,$(BDIR)/%,$(_OBJ))
$(BDIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)
$(SDIR)/main: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)