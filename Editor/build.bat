@echo off

if not exist build mkdir build
pushd build

set compile_flags=/W4 /Zi /MP /Od /FC /wd4201 /Fe:EngineEditor.exe /MTd -D UNICODE -I../../
set linker_flags=EngineCore.lib gdi32.lib user32.lib kernel32.lib shell32.lib

cl %compile_flags% ../src/*.cpp /link %linker_flags%

popd