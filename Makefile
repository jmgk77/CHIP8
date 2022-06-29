EXE = chip8

SOURCES = main.cpp chip8.cpp sdl1_chip8.cpp
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))

CXXFLAGS = -I./include -g -Wall -Wformat
LIBS = `sdl-config --libs` -lSDL_ttf

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(EXE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(OBJS) $(EXE)
