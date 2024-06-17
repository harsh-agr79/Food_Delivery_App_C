@echo off
gcc backend.c -o backend
if %errorlevel% equ 0 (
  npm run watch
) else (
  echo Compilation failed. Not running npm script.
)