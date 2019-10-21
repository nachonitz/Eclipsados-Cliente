# Source: https://hiltmon.com/blog/2013/07/03/a-simple-c-plus-plus-project-structure/

CC := g++ # This is the main compiler
SRCDIR := src
BUILDDIR := build
TARGET := bin/cliente
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g # -Wall
LIB := -pthread -lSDL2 -lSDL2_image -lSDL2_mixer
INC := -I include

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	$(shell for d in `find $(SRCDIR)/* -type d | cut -b 5-`; do mkdir -p $(BUILDDIR)/$$d; done)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

# Tests
# tester:
#   $(CC) $(CFLAGS) test/tester.cpp $(INC) $(LIB) -o bin/tester

.PHONY: clean
