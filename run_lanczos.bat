echo off
echo Running Lanczos Subsampling algorithm:

set scale=0.5
set /p scale=Enter scale factor (0..1] (default %scale%): 

set srcDir=./img
set /p srcDir=Enter source directory (default %srcDir%): 

set dstDir=./lanczos
set /p dstDir=Enter destination directory (default %dstDir%): 

set gamma=2.2
set /p gamma=Enter gamma (default %gamma%): 

.\bin\host.exe --src=%srcDir% --dst=%dstDir% --method=lanczos --scale=%scale% --gamma=%gamma%
