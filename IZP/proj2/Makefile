#AUTHOR : Eduard Frlička


# =========================================================================
# Names and Directories

# Flags for compiling
CFLAGS:=-std=c99 -Wall -Wextra -pedantic -ggdb3
CFLAGS+= -Wno-unused-parameter
CFLAGS+= -D_ASCII_COLORS

LFLAGS:=

# Name of target binary file
BINARY_PATH:=setcal

# Name / Path to folder, where .o files will be stored
OBJ:=obj

# Name / Path to folder, where .dep files will be stored
DEP:=$(OBJ)

# Name / Path to folder, where source files are (.h .c .cpp ...)
SRC:=src

# Compiler
CC:=gcc

# Suffix of files to compile (E.x.: c, cpp, c++)
SUFFIX:=c

# rm command
RM:=rm 


# =========================================================================
# initializing global variables

SRC_DIRECTORIES=$(shell find $(SRC) -type d)

vpath %.$(SUFFIX) $(SRC_DIRECTORIES)
vpath %.h $(SRC_DIRECTORIES)

# Adding -I 
INCLUDE:=$(foreach DIR, $(SRC_DIRECTORIES),-I $(DIR))

SOURCES  := $(foreach DIR,$(SRC_DIRECTORIES),$(notdir $(wildcard $(DIR)/*.$(SUFFIX))))
OBJECTS  := $(patsubst %.$(SUFFIX),$(OBJ)/%.o,$(SOURCES))


# =========================================================================
# Targets
.PHONY:  all run clean cleanall source
.PRECIOUS: $(DEP)/%.dep


# =========================================================================
# Rules
all: $(BINARY_PATH) 

run: $(BINARY_PATH) 
	./$(BINARY_PATH) sets.txt

$(BINARY_PATH) : $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) $^ -o $@ $(LFLAGS) $(INCLUDE)

$(OBJ)/%.o: %.$(SUFFIX) $(DEP)/%.dep
	@mkdir -p $(@D)
	$(CC) $< -c -o $@ $(CFLAGS) $(INCLUDE)

$(DEP)/%.dep: %.$(SUFFIX)
	@mkdir -p $(@D)
	@$(CC) -MT "$(patsubst $(DEP)/%.dep,$(OBJ)/%.o,$@) $@" -MM $< -o $@ $(INCLUDE)

source:
	@echo "Sources: " $(SOURCES)
	@echo "Objects: " $(OBJECTS)
	@echo "Source_Folders: " $(SRC_DIRECTORIES)


# =========================================================================
# Cleaning rules
clean:
	$(RM) -f $(OBJ)/*.o $(DEP)/*.dep

cleanall: clean
	$(RM) -f $(BINARY_PATH)
	$(RM) -df $(DEP) $(OBJ)

-include $(wildcard $(DEP)/*.dep)
