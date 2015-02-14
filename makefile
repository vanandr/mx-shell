CC = clang

CC_BASE_DIR = .

CC_INCDIRS :=\
               -I$(CC_BASE_DIR)

CC_FILE_LIST :=\
               $(CC_BASE_DIR)/mx-shell.c \
               $(CC_BASE_DIR)/mx-utils.c

CC_BUILD_FLAGS = -DDEBUG

mx-shell: $(CC_FILE_LIST)
	$(CC) -o $@ $(CC_FILE_LIST) $(CC_BUILD_FLAGS) $(CC_INCDIRS) 
