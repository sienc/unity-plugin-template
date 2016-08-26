rem ===========
rem rescopy.bat
rem Copys resource files into output dir 
rem ===========

@echo off

if defined RES_FILES (
	echo Copying resources to output folder...
	for %%I in (%RES_FILES%) do (
		xcopy %%I "%OUT_DIR%" /D /Y /S
	)
)

if defined COPY_DIR (
	echo Additional output folder: %COPY_DIR%
	xcopy "%TARGET_PATH%" "%COPY_DIR%" /D /Y /S
	if defined RES_FILES (
		echo Copying resources to additional output folder...
		for %%I in (%RES_FILES%) do (
			xcopy %%I "%COPY_DIR%" /D /Y /S
		)
	)
)
