CC = clang

mx-shell: mx-shell.o
	$(CC) -o mx-shell mx-shell.o
