@echo off

if not exist build mkdir build
pushd build

set compile_flags=/Fe:Game.exe /W4 /MP /FC /wd4201 /fsanitize=address /MTd -D UNICODE ^
-I../src ^
-I../../src ^
-I../../src/vendor/assimp/include

set source_files=^
../src/*.cpp

set linker_flags=CogwheelCore.lib gdi32.lib user32.lib kernel32.lib shell32.lib -incremental:no

cl %compile_flags% %source_files% /link %linker_flags%

del *.obj

popd