CXX = g++
CXXFLAGS = -g -Wall -Wextra -pedantic -fdiagnostics-color=always -Iinclude -Iutils

SRCDIRS = src utils
OBJDIR = obj
BINDIR = bin

SOURCES = $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.cpp))
OBJECTS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(notdir $(SOURCES)))

EXEC = $(BINDIR)/main.exe

all: $(EXEC)

$(EXEC): $(addprefix $(OBJDIR)/, $(notdir $(OBJECTS)))
	if not exist $(BINDIR) mkdir $(BINDIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJDIR)/%.o: src/%.cpp
	if not exist $(OBJDIR) mkdir $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: utils/%.cpp
	if not exist $(OBJDIR) mkdir $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	if exist $(OBJDIR) rmdir /s /q $(OBJDIR)
	if exist $(BINDIR) rmdir /s /q $(BINDIR)

.PHONY: all clean