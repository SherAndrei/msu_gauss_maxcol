#Commands
MKDIR := mkdir
RMDIR := rm -rf

#Folders to make life easier
BIN 	:= ./bin
OBJ 	:= ./obj
HDR 	:= ./hdr
SRC 	:= ./src
BENCH   := ./bench

#Compiler features
CC      := gcc
CFLAGS  := -mfpmath=sse -fstack-protector-all -W -Wall -Wextra -Wunused -Wcast-align \
		  -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security \
		  -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long  \
		  -Wcast-qual -Wno-suggest-attribute=format-Werror -Wpedantic -I$(HDR)
LDFLAGS := -fsanitize=address -fno-omit-frame-pointer
LIBS    := -lm

#Variables
EXE   := $(BIN)/main
SRCS  := $(wildcard $(SRC)/*.c)
OBJS  := $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))	
BNCHS := $(wildcard $(BENCH)/*.txt)

# $@ - the left side of the :
# $^ - the right side of the :
# $< - the first item in the dependencies list
# -c flag says to generate the object file

$(EXE): $(OBJS) | $(BIN)
	 $(DVARS) $(CC) $^ -o $@ $(LIBS) $(LDFLAGS) 

$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	 $(CC) -c $< -o $@ $(CFLAGS)  

$(BIN) $(OBJ) $(BENCH):
	$(MKDIR) $@

.PHONY: clear debug release

clear:
	$(RMDIR) $(OBJ) $(BIN) $(BNCHS)

debug: CFLAGS += -g -O0 -DDEBUG
debug: $(EXE) 

release: CFLAGS += -O3 -DRELEASE
release: $(EXE)