@echo off
cd /d "%~dp0"
py bangla_ide.py 2>nul || python bangla_ide.py
