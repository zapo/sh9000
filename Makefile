all: shell

shell: shell.c
	clang shell.c -o shell

clean:
	-rm shell


