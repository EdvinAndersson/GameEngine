@echo off

if not exist build mkdir build
pushd build

set compile_flags=/W4 /Zi /c /MTd /MP /Od /FC /wd4201 -D_CRT_SECURE_NO_WARNINGS -D UNICODE -DDEBUG ^
-I../src/vendor/assimp/include ^
-I../src/vendor/cglm-master/include ^
-I../src ^
-I../../

set linker_flags=gdi32.lib user32.lib kernel32.lib shell32.lib ole32.lib ^
/out:CogwheelCore.lib ^
../src/vendor/assimp/lib/assimp-vc142-mtd.lib

set source_files=^
../src/*.cpp ^
../src/Rendering/*.cpp ^
../src/vendor/stb_image/*.c ^
../src/event/*.cpp ^
../src/ecs/*.cpp ^
../src/project/*.cpp ^
../src/scene/*.cpp ^
../src/serialization/*.cpp ^
../src/assets/*.cpp 

cl %compile_flags% %source_files%
if %ERRORLEVEL% == 0 (
    lib *.obj %linker_flags%
) else (
    echo ---- Error was found. Not creating a .lib ----
)

del *.obj

popd

xcopy build\CogwheelCore.lib ..\Editor\build /y
xcopy build\CogwheelCore.lib ..\export\build /y
xcopy build\CogwheelCore.lib ..\tools\build_scripts /y
