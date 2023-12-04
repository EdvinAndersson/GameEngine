@echo off

if not exist build mkdir build
pushd build

set compile_flags=/W4 /Zi /c /MTd /MP /Od /FC /wd4201 -D_CRT_SECURE_NO_WARNINGS -D UNICODE -I../src/vendor/assimp/include
set linker_flags=gdi32.lib user32.lib kernel32.lib shell32.lib /out:EngineCore.lib ../src/vendor/assimp/lib/assimp-vc142-mtd.lib
set source_files=../src/*.cpp ../src/rendering/*.cpp ../src/vendor/stb_image/*.c

cl %compile_flags% %source_files%
if %ERRORLEVEL% == 0 (
    lib *.obj %linker_flags%
) else (
    echo ---- Error was found. Not creating a .lib ----
)

del *.obj

popd

xcopy build\EngineCore.lib ..\Editor\build /y
