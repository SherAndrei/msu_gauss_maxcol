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
		  -Wmissing-declarations -O3 -g -pg
LDFLAGS := # -fsanitize=address -fno-omit-frame-pointer -static-libasan
LIBS    := -lm

#Variables
EXE   := ./a.out
HDRS  := $(wildcard $(HDR)/*.h)
SRCS  := $(wildcard $(SRC)/*.c)
OBJS  := $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))	

# $@ - the left side of the :
# $^ - the right side of the :
# $< - the first item in the dependencies list
# -c flag says to generate the object file

$(EXE): $(OBJS)
	$(CC) $^ -o $@ $(LIBS) $(LDFLAGS) 

$(OBJ)/main.o: $(SRC)/main.c $(HDRS) | $(OBJ)
	$(CC) -c $< -o $@ $(CFLAGS)  

$(OBJ)/fill.o: $(SRC)/fill.c $(HDR)/fill.h $(HDR)/matrix.h $(HDR)/error.h | $(OBJ)
	$(CC) -c $< -o $@ $(CFLAGS)  

$(OBJ)/gauss_inverse.o: $(SRC)/gauss_inverse.c $(HDR)/gauss_inverse.h | $(OBJ)
	$(CC) -c $< -o $@ $(CFLAGS)  

$(OBJ)/matrix.o : $(SRC)/matrix.c $(HDR)/matrix.h | $(OBJ)
	$(CC) -c $< -o $@ $(CFLAGS)  

$(OBJ)/print.o : $(SRC)/print.c $(HDR)/print.h | $(OBJ)
	$(CC) -c $< -o $@ $(CFLAGS)  

$(OBJ)/error.o : $(SRC)/error.c $(HDR)/error.h | $(OBJ)
	$(CC) -c $< -o $@ $(CFLAGS)  

$(OBJ)/solve.o : $(SRC)/solve.c $(HDR)/matrix.h $(HDR)/gauss_inverse.h $(HDR)/error.h $(HDR)/operations.h | $(OBJ)
	$(CC) -c $< -o $@ $(CFLAGS)

${OBJ}/operations.o : ${SRC}/operations.c $(HDR)/operations.h | ${OBJ}
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJ):
	$(MKDIR) $@

.PHONY: clean debug release

clean:
	$(RMDIR) $(OBJ) $(EXE)

# debug: CFLAGS += -g -O0 -D DEBUG
# debug: $(EXE) 

# release: CFLAGS += -O3 -D RELEASE
# release: $(EXE)