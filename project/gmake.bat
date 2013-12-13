@ECHO OFF

SET REL_CODE_PATH=../code
SET REL_PREMAKE_PATH=../project

pushd .
cd %REL_CODE_PATH%

:: put premake on the path
pushd .
cd %REL_PREMAKE_PATH%
SET PREMAKE_PATH=%CD%
popd
SET ORIGINAL_ENV_PATH=%PATH%
SET PATH="%PREMAKE_PATH%"

:: run premake
premake4 gmake

:: restore the old path
SET PATH=%ORIGINAL_ENV_PATH%
popd