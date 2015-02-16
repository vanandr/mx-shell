CC = clang

CC_BASE_DIR = .

CC_INCDIRS :=\
               -I$(CC_BASE_DIR)

CC_FILE_LIST :=\
               $(CC_BASE_DIR)/mx_shell.c \
               $(CC_BASE_DIR)/mx_utils.c \
               $(CC_BASE_DIR)/mx_linkedlist.c \
               $(CC_BASE_DIR)/mx_cmd_parser.c \
               $(CC_BASE_DIR)/mx_parser_pipes.c

#CC_BUILD_FLAGS = -DDEBUG
CC_BUILD_FLAGS:=\

mx-shell: $(CC_FILE_LIST)
	$(CC) -o $@ $(CC_FILE_LIST) $(CC_BUILD_FLAGS) $(CC_INCDIRS) 
