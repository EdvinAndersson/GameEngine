@echo off

if not exist build mkdir build
pushd build

set compile_flags=/W4 /Zi /c /MTd /MP /Od /FC /wd4201 /fsanitize=address -D_CRT_SECURE_NO_WARNINGS -D UNICODE -DDEBUG ^
-I../../Utility/include ^
-I../../Utility/include/Utility/vendor/assimp/include

set linker_flags=gdi32.lib user32.lib kernel32.lib shell32.lib ^
/out:CogwheelCore.lib ^
../../Utility/include/Utility/vendor/assimp/lib/assimp-vc142-mtd.lib

set source_files=^
../../Utility/include/Utility/*.cpp ^
../../Utility/include/Utility/Rendering/*.cpp ^
../../Utility/include/Utility/vendor/stb_image/*.c ^
../src/*.cpp ^
../src/event/*.cpp ^
../src/ecs/*.cpp ^
../src/project/*.cpp ^
../src/scene/*.cpp ^
../src/serialization/*.cpp 

cl %compile_flags% %source_files%
if %ERRORLEVEL% == 0 (
    lib *.obj %linker_flags%
) else (
    echo ---- Error was found. Not creating a .lib ----
)

del *.obj

popd

xcopy build\CogwheelCore.lib ..\Editor\build /y
