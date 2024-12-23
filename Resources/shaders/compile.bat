SET BIN=../../bin/Resources/shaders
echo %VULKAN_SDK%
if not exist "%BIN%" (
    mkdir "%BIN%"
)
%VULKAN_SDK%\Bin\glslc.exe shader.vert -o %BIN%\vertex.spv
%VULKAN_SDK%\Bin\glslc.exe shader.frag -o %BIN%\fragment.spv
