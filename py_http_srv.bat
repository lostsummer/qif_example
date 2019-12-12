@echo off
FOR /F %%i IN ('python -c "import sys;print(sys.version_info.major)"') DO @set PY_VER=%%i

if "%PY_VER%" == "3"  (python -m http.server 80) else (python -m SimpleHTTPServer 80)