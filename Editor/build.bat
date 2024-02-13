@echo off

if not exist build mkdir build
pushd build

set compile_flags=/W4 /Zi /MP /Od /FC /wd4201 /Fe:CogwheelEditor.exe /MTd -D UNICODE -D_CRT_SECURE_NO_WARNINGS ^
-I../.. ^
-I../../Core/src ^
-I../../Core/src/vendor/assimp/include ^
-I../../Core/src/vendor/cglm-master/include

set source_files=^
../src/*.cpp ^
../src/assets/*.cpp ^
../src/vendor/imgui/*.cpp ^
../src/vendor/imgui/backends/*.cpp

set linker_flags=CogwheelCore.lib gdi32.lib user32.lib kernel32.lib shell32.lib -incremental:no

cl %compile_flags% %source_files% /link %linker_flags%

popd
