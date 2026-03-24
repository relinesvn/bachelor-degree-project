@echo off
echo === Запуск комплексної перевірки проєкту ===

echo [1/2] Перевірка типів та компіляція (Static Type Checking)...
msbuild BachelorProject.sln /p:Configuration=Debug /p:WarningLevel=4 /p:TreatWarningsAsErrors=true

echo [2/2] Запуск статичного аналізатора (Linting)...
clang-tidy BachelorProject\BachelorProject.cpp -config="" --

echo === Перевірку завершено! ===
pause