@echo off
%~d0
cd %~dp0

windres icon.rc -o icon.o
gcc starter.c icon.o -o starter.exe -mwindows

pause
