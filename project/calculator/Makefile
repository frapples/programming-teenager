CPP = g++
CPPFLAGS_GTK = $(shell pkg-config gtkmm-3.0 --cflags --libs)
CPPFLAGS = -g -Wall -std=c++11  $(CPPFLAGS_GTK)


main: main.o MainWindow.o Integer.o Number.o expression.o CalculatorMainWidget.o voice.o
	$(CPP) $(CPPFLAGS) main.cpp MainWindow.cpp Integer.cpp Number.cpp expression.cpp CalculatorMainWidget.cpp voice.cpp  -o main

