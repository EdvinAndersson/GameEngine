@echo off

if not exist build mkdir build
pushd build

set compile_flags=/W4 /Zi /MP /Od /FC /wd4201 /fsanitize=address /Fe:CogwheelEditor.exe /MTd -D UNICODE ^
-I../.. ^
-I../../Utility/include ^
-I../../Utility/include/Utility/vendor/assimp/include

set linker_flags=CogwheelCore.lib gdi32.lib user32.lib kernel32.lib shell32.lib -incremental:no

set source_files=^
../src/*.cpp ^
../src/vendor/imgui/*.cpp ^
../src/vendor/imgui/backends/*.cpp

cl %compile_flags% %source_files% /link %linker_flags%

popd