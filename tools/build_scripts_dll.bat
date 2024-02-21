@echo off

pushd tools
if not exist build_scripts mkdir build_scripts
pushd build_scripts

set compile_flags=/c -D UNICODE ^
-I../../ ^
-I../../Core/src ^
-I../../Core/src/vendor/assimp/include ^
-I../../Core/src/vendor/cglm-master/include

set linker_flags=gdi32.lib user32.lib kernel32.lib shell32.lib /out:ScriptsLib.lib

cl %compile_flags% ../../Editor/res/projects/Project1/Assets/scripts/TestComponent.cpp ../../Editor/res/projects/Project1/Assets/scripts/TestComponent2.cpp

link %linker_flags% /DLL /OUT:ScriptsDLL.dll *.obj

popd
popd