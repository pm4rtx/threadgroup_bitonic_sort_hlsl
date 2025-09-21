@echo off

setlocal enabledelayedexpansion
cls

cd /D "%~dp0"

set SRC_DIR=%CD%
set DXC="dxc.exe"

set d3d_ver=1.615.0
set dxc_ver=1.8.2502.8
set pix_ver=1.0.240308001

set d3d_inc=Microsoft.Direct3D.D3D12.%d3d_ver%\build\native\include
set dxc_inc=Microsoft.Direct3D.DXC.%dxc_ver%\build\native\include
set pix_inc=WinPixEventRuntime.%pix_ver%\Include\WinPixEventRuntime

set d3d_bin=Microsoft.Direct3D.D3D12.%d3d_ver%\build\native\bin\x64
set dxc_bin=Microsoft.Direct3D.DXC.%dxc_ver%\build\native\bin\x64
set pix_bin=WinPixEventRuntime.%pix_ver%\bin\x64

set PREFERRED_INCLUDE_PATH=

set d3d_bin_path=
set dxc_bin_path=
set pix_bin_path=

for %%a in (%*) do set "%%a=1"

if not "%release%"=="1" if not "%debug%"=="1" echo [Configuration ("Debug" or "Release") was not set. Choosing "Release"] && set release=1

:: Populate NuGet cache
if not exist .nuget_cache (
    where /q curl
    if errorlevel 1 (
        echo [Can't find curl.exe in PATH... skipping .nuget_cache population]
    ) else (
        echo [Found curl.exe in PATH... populating .nuget_cache]
        mkdir .nuget_cache
        pushd .nuget_cache
            call :fetch_nuget_package Microsoft.Direct3D.D3D12 %d3d_ver%
            call :fetch_nuget_package Microsoft.Direct3D.DXC %dxc_ver%
            call :fetch_nuget_package WinPixEventRuntime %pix_ver%
        popd
    )
)

:: setup NuGet include/lib paths
if exist .nuget_cache (
    set "PREFERRED_INCLUDE_PATH=!PREFERRED_INCLUDE_PATH! /I %SRC_DIR%\.nuget_cache\%d3d_inc%"
    set "PREFERRED_INCLUDE_PATH=!PREFERRED_INCLUDE_PATH! /I %SRC_DIR%\.nuget_cache\%dxc_inc%"
    set "PREFERRED_INCLUDE_PATH=!PREFERRED_INCLUDE_PATH! /I %SRC_DIR%\.nuget_cache\%pix_inc%"

    set "d3d_bin_path=%SRC_DIR%\.nuget_cache\%d3d_bin%"
    set "dxc_bin_path=%SRC_DIR%\.nuget_cache\%dxc_bin%"
    set "pix_bin_path=%SRC_DIR%\.nuget_cache\%pix_bin%"
    set "DXC=!dxc_bin_path!\dxc.exe"
)

if "%no_shaders%"=="1" (
    echo [Skipping rebuilding shaders]
) else (
    :: We rebuild shaders when any of the configs compiled
    if not exist .build_shaders mkdir .build_shaders
    pushd .build_shaders

        rem for %%i in (%SRC_DIR%\*.hlsl) do call :compile_shader %SRC_DIR% %%~ni

        for /l %%i in (1,1,10) do (
            echo [Compiling Shader Permutations %%i0%%]
            for %%j in (0,1,2) do (
                for %%k in (0,1) do (
                    set /a kernel_size_log2=%%i+%%j
                    call :compile_shader_bitonic_sort_variant %SRC_DIR% threadgroup_bitonic_sort !kernel_size_log2! %%i %%k
                )
            )
        )
    popd
)

if "%release%"=="1" (
    echo [building Release]
    call :build_config release
)

if "%debug%"=="1" (
    echo [building Debug]
    call :build_config debug
)

goto :eof

:build_config
    if not exist .build_%1 mkdir .build_%1
    pushd .build_%1
        :: copy the DLLs if NuGet packages exist or were acquired
        if exist %d3d_bin_path%\D3D12Core.dll           copy /v /y /b %d3d_bin_path%\D3D12Core.dll .
        if exist %d3d_bin_path%\d3d12SDKLayers.dll      copy /v /y /b %d3d_bin_path%\d3d12SDKLayers.dll .
        if exist %pix_bin_path%\WinPixEventRuntime.dll  copy /v /y /b %pix_bin_path%\WinPixEventRuntime.dll .

        :: compile C++
        set objlst=
        call :compile_%1 %SRC_DIR%
        for %%a in (*.obj) do (
            set objlst=%objlst% %%a
        )
        call :link_%1_exe /out:demo_%1.exe %objlst%
    popd
exit /b 0

:compile_shader_bitonic_sort_variant
    if "%5"=="1" (
        %DXC% -E main -T cs_6_0 -HV 2021 -DKERNEL_SIZE_LOG2=%3 -DTGROUP_SIZE_LOG2=%4 -DNO_WAVE_INTRINSICS=1 -O3 -Zs -Qstrip_debug -Fo shader_%2_%3_%4_%5.cso -Fc shader_%2_%3_%4_%5.asm -Fh shader_%2_%3_%4_%5.h -Vn shader_%2_%3_%4_%5 -Fd shader_%2_%3_%4_%5.pdb "%1\%2.hlsl"
    ) else (
        %DXC% -E main -T cs_6_0 -HV 2021 -DKERNEL_SIZE_LOG2=%3 -DTGROUP_SIZE_LOG2=%4 -O3 -Zs -Qstrip_debug -Fo shader_%2_%3_%4_%5.cso -Fc shader_%2_%3_%4_%5.asm -Fh shader_%2_%3_%4_%5.h -Vn shader_%2_%3_%4_%5 -Fd shader_%2_%3_%4_%5.pdb "%1\%2.hlsl"
    )
exit /b 0

:compile_shader
    %DXC% -E main -T cs_6_0 -HV 2021 -O3 -Zs -Qstrip_debug -Fo shader_%2.cso -Fc shader_%2.asm -Fh shader_%2.h -Vn shader_%2 -Fd shader_%2.pdb "%1\%2.hlsl"
exit /b 0

:compile_debug
    for %%a in (%1\*.cpp) do (cl /c /nologo /I %1\.build_shaders /I %1\external %PREFERRED_INCLUDE_PATH% /Od /Zi /Fo%%~na.obj /Fd%%~na.pdb %%a)
exit /b 0

:compile_release
    rem for %%a in (%1\*.cpp) do (cl /c /nologo /I %1\.build_shaders /I %1\external %PREFERRED_INCLUDE_PATH% /O2 /Zi /Fo%%~na.obj %%a)

    for %%a in (%1\*.cpp) do (cl /c /nologo /I %1\.build_shaders /I %1\external %PREFERRED_INCLUDE_PATH% /O2 /GS- /GL /wd4710 /wd4711 /Zi /c /nologo /EHsc /GR- /Zl /Wall /WX /WL /wd4514 /Fo%%~na.obj %%a)
exit /b 0

:link_debug_exe
    link /nologo /incremental:no /nodefaultlib /subsystem:console /machine:x64 /debug %* vcruntimed.lib ucrtd.lib msvcrtd.lib kernel32.lib ole32.lib shell32.lib
exit /b 0

:link_release_exe
    link /ltcg /nologo /incremental:no /nodefaultlib /subsystem:console /machine:x64 /debug %* vcruntime.lib ucrt.lib msvcrt.lib kernel32.lib ole32.lib shell32.lib
exit /b 0

:fetch_nuget_package
    start /wait /b curl -L -s -o %1.%2.zip https://www.nuget.org/api/v2/package/%1/%2
    mkdir %1.%2
    start /wait /b tar -xf %1.%2.zip -C %1.%2
exit /b 0

:eof
