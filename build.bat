@echo off

echo =====================================
echo DIPAL LIBRATY BUILD SCRIPT
echo =====================================

REM Check if CMake exists
cmake --version >nul 2>&1
if %errorlevel% neq 0 (
  echo ERROR: CMake not found! please install CMake and add iy to PATH
  pause
  exit /b 1
)

echo .
echo ==================================
echo Cleaning previous build files ...
echo ==================================

REM Remove build directories
if exist build (
  echo Removing build directory ...
  rmdir /s /q build
  echo Build directory removed.
) else (
  echo Build directory not found.
)

if exist build-debug (
  echo Removing build-debug directory ...
  rmdir /s /q build-debug
  echo build-debug directory removed.
) else (
  echo build-debug directory not found.
)

REM Remove other generated files

if exist compile_commands.json (
  echo Removing compile_commands.json ...
  del compile_commands.json
)

echo Clear completed!

echo .
echo =============================
echo Building DIPAL library
echo =============================

REM Create fresh build allow-unrelated-historiesrectory
echo Configuring CMake ...
mkdir build
cd build
cmake ..

if %errorlevel% neq 0 (
  echo ERROR: CMake configuration failed !
  pause 
  cd ..
  exit /b 1
)

REM Build the project
cmake --build .

if %errorlevel% neq 0 (
  echo ERROR: Build failed!
  pause 
  cd ..
  exit /b 1
)

echo.
echo ==================================
echo Build completed successfully!
echo ==================================
echo.
echo Executable location: build\examples\Release\
echo Library location: build\Release\
echo.

REM Go back to project root
cd ..

echo Press any key to continue ...
pause
