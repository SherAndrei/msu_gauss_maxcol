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
# OBJS  := $(wildcard $(OBJ)/*.o)
OBJS  := $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))	
BNCHS := $(wildcard $(BENCH)/*.txt)

# $@ - the left side of the :
# $^ - the right side of the :
# $< - the first item in the dependencies list
# -c flag says to generate the object file

$(EXE): $(OBJS) | $(BIN)
	$(CC) $^ -o $@ $(LIBS) $(LDFLAGS) 

$(OBJ)/main.o: $(SRC)/main.c $(SRCS) | $(OBJ)
	$(CC) -c $< -o $@ $(CFLAGS)  

$(OBJ)/fill.o: $(SRC)/fill.c $(SRC)/matrix.c $(HDR)/fill.h $(HDR)/error.h $(HDR)/matrix.h | $(OBJ)
	$(CC) -c $< -o $@ $(CFLAGS)  

$(OBJ)/gauss_inverse.o: $(SRC)/gauss_inverse.c $(HDR)/gauss_inverse.h | $(OBJ)
	$(CC) -c $< -o $@ $(CFLAGS)  

$(OBJ)/matrix.o : $(SRC)/matrix.c $(HDR)/matrix.h | $(OBJ)
	$(CC) -c $< -o $@ $(CFLAGS)  

$(OBJ)/print.o : $(SRC)/print.c $(HDR)/print.h | $(OBJ)
	$(CC) -c $< -o $@ $(CFLAGS)  

$(OBJ)/error.o : $(SRC)/error.c $(HDR)/error.h | $(OBJ)
	$(CC) -c $< -o $@ $(CFLAGS)  

$(OBJ)/solve.o : $(SRC)/solve.c $(SRC)/matrix.c $(SRC)/gauss_inverse.c $(HDR)/error.h $(HDR)/multiply.h $(HDR)/extract.h | $(OBJ)
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