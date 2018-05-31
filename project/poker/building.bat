@echo off
for /f "delims=" %%s ('pkg-config --cflags --libs gtk+-3.0') do set pra=%%~s

gcc -Wall -c pokers.c %pra%
gcc -Wall -c GTK_GUI.c %pra%
gcc -Wall -o main.exe  GTK_GUI.o pokers.o %pra%
pause
