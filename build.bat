@echo off
echo Derleniyor...
g++ -std=c++17 -Wall -Wextra -static src\main.cpp src\Device.cpp -o IoTGuard.exe
if %errorlevel% neq 0 (
    echo Derleme Hatasi!
    pause
    exit /b %errorlevel%
)
echo Derleme Basarili! Uygulama Baslatiliyor...
echo.
IoTGuard.exe
pause
