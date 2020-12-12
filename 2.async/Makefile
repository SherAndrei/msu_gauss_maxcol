#Commands
MKDIR := mkdir
RMDIR := rm -rf

#Folders to make life easier
BIN 	:= ./bin
OBJ 	:= ./obj
HDR     := ./hdr
SRC 	:= ./src

#Compiler features
CC      := gcc
CFLAGS  := -mfpmath=sse -fstack-protector-all -W -Wall -Wextra -Wunused -Wcast-align \
		  -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security \
		  -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long  \
		  -Wcast-qual -Wno-suggest-attribute=format -Wpedantic -Werror=declaration-after-statement -I$(HDR) \
		  -Wmissing-declarations
LDFLAGS := # -fsanitize=address -fno-omit-frame-pointer -static-libasan
LIBS    := -lm -lpthread

#Variables
EXE   := ./a.out
HDRS  := $(wildcard $(HDR)/*.h)
SRCS  := $(wildcard $(SRC)/*.c)
OBJS  := $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))	

all: debug

$(EXE): $(OBJS)
	$(CC) $^ -o $@ $(LIBS) $(LDFLAGS) 

$(OBJ)/main.o: $(SRC)/main.c $(HDRS) | $(OBJ)
	$(CC) -c $< -o $@ $(CFLAGS)  

$(OBJ)/thread_func.o: $(SRC)/thread_func.c $(HDR)/thread_func.h $(HDR)/fill.h $(HDR)/args.h $(HDR)/solve.h $(HDR)/matrix.h $(HDR)/gauss_inverse.h $(HDR)/synchronise.h | $(OBJ)
	$(CC) -c $< -o $@ $(CFLAGS)  

$(OBJ)/fill.o: $(SRC)/fill.c $(HDR)/fill.h $(HDR)/args.h $(HDR)/error.h $(HDR)/synchronise.h | $(OBJ)
	$(CC) -c $< -o $@ $(CFLAGS)  

$(OBJ)/error.o : $(SRC)/error.c $(HDR)/error.h | $(OBJ)
	$(CC) -c $< -o $@ $(CFLAGS)  

# $(OBJ)/solve.o : $(SRC)/solve.c $(HDR)/matrix.h $(HDR)/gauss_inverse.h $(HDR)/operations.h | $(OBJ)
# 	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJ):
	$(MKDIR) $@

.PHONY: all clean debug release

clean:
	$(RMDIR) $(OBJ) $(EXE)

debug: CFLAGS += -g -O0 -DDEBUG
debug: $(EXE) 

release: CFLAGS += -O3 -DRELEASE
release: $(EXE)