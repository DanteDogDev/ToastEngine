@echo off
cls
cmake -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -B ./bin -DCMAKE_BUILD_TYPE=Debug
cmake --build ./bin
echo compiling shaders
SET BIN=.\bin\Resources\shaders
if not exist "%BIN%" (
    mkdir "%BIN%"
)
%VULKAN_SDK%\Bin\glslc.exe Resources\shaders\shader.vert -o %BIN%\vertex.spv
%VULKAN_SDK%\Bin\glslc.exe Resources\shaders\shader.frag -o %BIN%\fragment.spv
echo Moving Compile Commands
cd bin
IF EXIST "compile_commands.json" (
  move ./compile_commands.json ../
)
cd ../
