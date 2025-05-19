CXX = g++
CXXFLAGS = -g -Wall -Wextra -pedantic -fdiagnostics-color=always -Iinclude -Iutils

SRCDIRS = src utils
OBJDIR = obj
BINDIR = bin

SOURCES = $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.cpp))
OBJECTS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(notdir $(SOURCES)))

EXEC = $(BINDIR)/main

all: $(EXEC)

$(EXEC): $(addprefix $(OBJDIR)/, $(notdir $(OBJECTS)))
	mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJDIR)/%.o: src/%.cpp
	mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: utils/%.cpp
	mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean