# Compiler Information
CC=gcc
CFLAGS=-g -std=gnu99
EXEC=.exe

# Directories
SRC_DIR=src

BIN_DIR=bin
BIN=$(BIN_DIR)/recall$(EXEC)

DOC_DIR=docs
DOCS=$(wildcard $(docs)/*)

INCLUDE_DIR=$(SRC_DIR)/deps
INCLUDES=$(wildcard $(INCLUDE_DIR)/*.c)
HEADERS=$(patsubst $(INCLUDE_DIR)/%.c,$(INCLUDE_DIR)/%.h,$(INCLUDES))

TEST_DIR=test
TEST_BIN_DIR=$(TEST_DIR)/bin
TEST_FRAMEWORK=$(TEST_DIR)/ceemply/ceemply.c

OBJ_DIR=obj
OBJS=$(patsubst $(INCLUDE_DIR)/%.c,$(OBJ_DIR)/%.o,$(INCLUDES))

MAIN_SRC=$(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/.*.c)

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -std=c99 $(MAIN_SRC) $? -o $(BIN) -lm

$(OBJ_DIR)/%.o: $(INCLUDE_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
ifneq (, $(wildcard $(BIN)))
	del$(subst /,\\, $(BIN))
else 
	$(info > Binary executable already deleted)
endif

ifneq (, $(wildcard $(OBJS)))
	del$(subst /,\\, $(OBJS))
else
	$(info > Object files already deleted)
endif

ifneq (, $(wildcard *.zip))
	del $(SUBMISSION)
else
	$(info > Submission already deleted)
endif

ifneq (, $(wildcard $(TEST_BIN_DIR)/*))
	del $(subst /,\\, $(wildcard $(TEST_BIN_DIR)/*))
else
	$(info > Submission already deleted)
endif


test: $(TEST_BINS)

test_%: $(OBJS)
	$(CC) $(CFLAGS) $(TEST_DIR)/$@.c $^ $(TEST_FRAMEWORK) -o $(TEST_BIN_DIR)/$@

submit:
	minizip -o $(SUBMISSION) $(BIN) $(INCLUDES) $(HEADERS) $(OBJS) $(MAIN_SRC) $(INCLUDED_MISC_FILES)
