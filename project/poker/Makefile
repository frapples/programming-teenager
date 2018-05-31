CC = gcc
CFLAGS_GTK = $(shell pkg-config --cflags --libs gtk+-3.0)
CFLAGS = -g -Wall $(CFLAGS_GTK)

main:GTK_GUI.o pokers.o
	$(CC) $(CFLAGS_GTK) -o main  GTK_GUI.o pokers.o

