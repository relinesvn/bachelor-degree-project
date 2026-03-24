@echo off
chcp 65001 >nul
echo === Бакалаврський проєкт (PROD ENV) ===
echo Запуск робочої версії програми...
echo.

:: Перевіряємо, чи існує скомпільований файл у поточній директорії
if not exist "BachelorProject.exe" (
    echo [ПОМИЛКА] Файл BachelorProject.exe не знайдено! 
    echo Переконайтеся, що ви скопіювали сюди Release-версію програми.
    pause
    exit /b 1
)

:: Запуск програми
BachelorProject.exe

echo.
echo [УСПІХ] Обчислення завершено. Логи збережено у файл tpar.rez.
pause