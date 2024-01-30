@echo off

pushd build

set compile_flags=/Fe:Game.exe

set source_files=^
../src/*.cpp

set linker_flags=CogwheelCore.lib gdi32.lib user32.lib kernel32.lib shell32.lib -incremental:no

cl %compile_flags% %source_files% /link %linker_flags%

popd