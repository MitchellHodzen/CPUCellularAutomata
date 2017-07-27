CC = g++
CLIBS = `sdl2-config --cflags --libs` -lSDL2_image
OUTPUT = test
DEBUGOUTPUTDIR = bin/debug
RELEASEOUTPUTDIR = bin/release
OBJDIR = bin
SRCDIR = .

SOURCES = $(wildcard *.cpp)
POBJECTS = $(SOURCES:.cpp=.o)
OBJECTS = $(addprefix $(OBJDIR)/, $(POBJECTS))

$(OUTPUT): $(OBJECTS)
	$(CC) $(OBJECTS) $(CLIBS) -o $(DEBUGOUTPUTDIR)/$(OUTPUT)
	@echo "\n"Debug build completed\; Enter $(DEBUGOUTPUTDIR)/$(OUTPUT) to run."\n"

$(OBJDIR)/%.o: %.cpp
	$(CC) -c $< -o $@ $(CLIBS)

clean:
	rm $(OBJDIR)/*.o $(DEBUGOUTPUTDIR)/$(OUTPUT) #$(RELEASEOUTPUTDIR)/$(OUTPUT)

#release: $(OBJECTS)
#	$(CC) $(OBJECTS) $(CLIBS) -O3 -o $(RELEASEOUTPUTDIR)/$(OUTPUT)
#	@echo "\n"Release build completed\; Enter $(RELEASEOUTPUTDIR)/$(OUTPUT) to run."\n"
