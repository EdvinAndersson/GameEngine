@echo off

pushd Core
pushd export

for /f "usebackq delims=" %%i in (`vswhere.exe -prerelease -latest -property installationPath`) do (
  if exist "%%i\VC\Auxiliary\Build\vcvars64.bat" (
    call "%%i\VC\Auxiliary\Build\vcvars64.bat" %*
    call build.bat
    popd
    popd
    exit /b
  )
)

rem Instance or command prompt not found
popd
popd
exit /b 2