@echo off
call conda activate QbertEnvironement
cd /d "%~dp0"
python generate_class.py DerivedClass BaseClass BaseClass.h
call conda deactivate