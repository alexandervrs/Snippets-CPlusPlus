echo off
title Compile

chcp 65001
cls

echo.
echo  Compiling...
echo.

g++ %1 -static-libgcc -static-libstdc++ -lgdi32 -lcomdlg32 -D UNICODE -D _UNICODE -O3 -DNDEBUG -s -o "_Test.exe"
pause
echo.
echo  Running...
echo.
_Test.exe -fullscreen

exit