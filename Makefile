PROJECT_NAME := c-kanban

COMPILER := gcc
COMPILER_FLAGS := `pkg-config --cflags gtk+-3.0` -Wall
COMPILER_END_FLAGS := `pkg-config --libs gtk+-3.0` # se nao for no fim nao funciona

OUTPUT_FOLDER := bin
TESTS_FOLDER := tests

SOURCE_FOLDER := src
LIB_FOLDER := lib

SOURCE_FILES := $(wildcard $(SOURCE_FOLDER)/*.c) $(wildcard $(LIB_FOLDER)/*.c)
OBJ_FILES := $(patsubst %.c, %.o, $(SOURCE_FILES))

.PHONY: all compile clean run test

all: compile clean .clear run

compile:
	@$(COMPILER) $(COMPILER_FLAGS) -I$(LIB_FOLDER) -o $(OUTPUT_FOLDER)/$(PROJECT_NAME).out $(SOURCE_FILES) $(COMPILER_END_FLAGS)

.clear:
	clear

run:
	@./$(OUTPUT_FOLDER)/$(PROJECT_NAME).out

test:
	cd $(TESTS_FOLDER) && $(MAKE)

clean:
	rm -f $(OBJ_FILES)

clean-tests:
	rm -F $(wildcard $(TESTS_FOLDER)/*.o)
