echo off
echo Running BoxBlur MipMap Downsampling algorithm:

set scale=0.5
set /p scale=Enter scale factor (0..1] (default %scale%): 

set srcDir=./img2
set /p srcDir=Enter source directory (default %srcDir%): 

set dstDir=./pslike
set /p dstDir=Enter destination directory (default %dstDir%): 

.\bin\host.exe --src=%srcDir% --dst=%dstDir% --method=mip-floor --scale=%scale% --gamma=1.0 --lod-bias=-1.0 --src-alpha=premultiply --dst-alpha=unpremultiply
