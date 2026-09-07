CC = clang

CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lm
SRC = src/*.c

default: .o $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) *.o -o main

debug: .o $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) *.o -o main_dbinfo -fsanitize=address -g 

.o: $(SRC)
	$(CC) $(CFLAGS) -c src/main.c -o main.o
	$(CC) $(CFLAGS) -c src/player.c -o player.o
	$(CC) $(CFLAGS) -c src/projectile.c -o projectile.o
	$(CC) $(CFLAGS) -c src/enemy.c -o enemy.o

clean: *
	-rm *.o main*
