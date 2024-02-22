@echo off

pushd tools
if not exist build_scripts mkdir build_scripts
pushd build_scripts

del *.obj
del ScriptsDLL.dll
del ScriptsDLL.lib

set compile_flags=/c /W4 /Zi /MP /Od /FC /wd4201 /MTd -D UNICODE -D_CRT_SECURE_NO_WARNINGS ^
-I../../ ^
-I../../Core/src ^
-I../../Core/src/vendor/assimp/include ^
-I../../Core/src/vendor/cglm-master/include

set linker_flags=gdi32.lib user32.lib kernel32.lib shell32.lib -incremental:no CogwheelCore.lib /out:ScriptsLib.lib

cl %compile_flags% ../../Editor/res/projects/Project1/Assets/scripts/*.cpp ../GeneratedComponentsUtility.cpp

link %linker_flags% /DLL /OUT:ScriptsDLL.dll *.obj

popd
popd