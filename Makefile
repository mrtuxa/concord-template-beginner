all: *.c

build:
	gcc main.c -o ovn -pthread -ldiscord -lcurl