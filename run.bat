@echo off
cls
cmake -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -B ./bin
cmake --build ./bin
cd bin
IF EXIST "compile_commands.json" (
  move ./compile_commands.json ../
)
cd ../
