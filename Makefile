CFLAGS = -Wall -Wextra -Wpedantic -Werror -std=c99 -g -lm
CC = gcc $(CFLAGS)

.PHONY: linux mac clean error

.PHONY	:	all
all	:	linux

linux: lex.yy.o	banhammer.o
	$(CC)  -o banhammer banhammer.o lex.yy.o -lfl || make error

mac: lex.yy.o flextutorial.o	banhammer.o
	$(CC)  -o banhammer banhammer.o lex.yy.o -ll

banhammer.o	:	banhammer.c
	$(CC) -c banhammer.c

lex.yy.o : lex.yy.c
	cc -c lex.yy.c


lex.yy.c: words.l
	flex words.l


platformerror := " \n Unable to compile. \n If you are on a mac, and the above error message contains \n  'ld: library not found for -lfl'\n use\n   make mac \n instead of just \n   make\n "
error:
	echo $(platformerror)

clean:
	rm -f banhammer banhammer.o lex.yy.o lex.yy.c
