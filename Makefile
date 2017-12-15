#declare the variable
CC= g++
CFLAG =-c -Wall
all: db
db: $(CC) -o db main.c
clean: rm *.o *.txt *.sql

