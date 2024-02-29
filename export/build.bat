@echo off

if not exist build mkdir build
pushd build

xcopy "..\..\Editor\build\CogwheelCore.lib" /y

set compile_flags=/Fe:Game.exe /W4 /MP /FC /wd4201 -D_CRT_SECURE_NO_WARNINGS /MTd -D UNICODE ^
-I../.. ^
-I../src ^
-I../../Core/src ^
-I../../Core/src/vendor/assimp/include ^
-I../../Core/src/vendor/cglm-master/include ^
-I../src/vendor

set source_files=^
../src/*.cpp

set linker_flags=CogwheelCore.lib gdi32.lib user32.lib kernel32.lib shell32.lib ole32.lib -incremental:no

cl %compile_flags% %source_files% /link %linker_flags%

del *.obj

xcopy "..\..\Editor\res\projects\Project1\Assets" Assets /s /e /i /y
xcopy "..\..\Editor\res\projects\Project1\Unnamed Project.proj" /y
xcopy "..\..\Editor\build\ScriptsDLL.dll" /y
xcopy "..\..\Editor\build\assimp-vc142-mtd.dll" /y

popd