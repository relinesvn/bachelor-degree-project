@echo off
chcp 65001 >nul
echo === Бакалаврський проєкт (DEV ENV) ===
echo [1/2] Збірка у режимі Debug...

:: Переходимо в корінь проєкту і запускаємо компіляцію
msbuild ..\..\BachelorProject.slnx /p:Configuration=Debug /p:Platform=x64

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo [ПОМИЛКА] Помилка компіляції! Перевірте код.
    pause
    exit /b %ERRORLEVEL%
)

echo.
echo [2/2] Запуск програми...
..\..\x64\Debug\BachelorProject.exe

echo.
pause