CC=g++
CFLAGS=-g -Wall
BIN=main

OBJDIR=obj

SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))

all: $(BIN)

release: CFLAGS=-O2 -DNDEBUG
release: clean $(BIN)

# Link .o files and sfml lib files
$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

# Compile .cpp files
$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

# Run executable
run:
	./$(BIN)

# Clean obj dir

#Clean obj dir
clean:
	powershell rm -r $(OBJDIR)/*.o
