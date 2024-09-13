@echo off
call conda activate QbertEnvironement
cd /d "%~dp0"
python generate_class.py LivesSystem System System.h
call conda deactivate