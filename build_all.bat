@echo off

pushd Core
call build.bat
popd

pushd Editor
call build.bat
popd