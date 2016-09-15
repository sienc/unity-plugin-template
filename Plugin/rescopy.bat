@echo off

::Define newline variable
(SET LF=^
%=this line is empty=%
)

IF NOT "%RES_FILES%" == "" (
echo Copying DLLs and resources to output folder...

::
IF NOT "%ADD_OUT_DIR%" == "" (
echo Additional output directory: %ADD_OUT_DIR%
xcopy "%TARGET_PATH%" "%ADD_OUT_DIR%" /D /Y /S
)

:: Save file list to a temporary file, replacing ; with newline
echo %RES_FILES:;=!LF!% > rescopy.tmp

:: Loop through items in the file
FOR /F %%I IN (rescopy.tmp) DO (
echo Copying %%I
xcopy %%I "%OUT_DIR%" /D /Y /S

IF NOT "%ADD_OUT_DIR%" == "" (
echo Copying to additional output directory
xcopy %%I "%ADD_OUT_DIR%" /D /Y /S
)

)

:: delete temp file
del rescopy.tmp

)