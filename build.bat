@echo off

setlocal enabledelayedexpansion
cls

cd /D "%~dp0"

set SRC_DIR=%CD%
set DXC=

set d3d_ver=1.615.0
set dxc_ver=1.8.2502.8
set pix_ver=1.0.240308001

if not defined INCLUDE  echo [`INCLUDE` environment variable doesn't exist. Make sure to setup it or run `vcvarsall.all`]   && goto :eof
if not defined LIB      echo [`LIB` environment variable doesn't exist. Make sure to setup it or run `vcvarsall.all`]       && goto :eof

for %%a in (%*) do set "%%a=1"

if not "%release%"=="1" if not "%debug%"=="1" echo [Configuration ("Debug" or "Release") was not set. Choosing "Release"] && set release=1

if not "%no_nuget%"=="1" set no_nuget=0
if not "%no_shaders%"=="1" set no_shaders=0

:: Populate NuGet cache
if not exist .nuget_cache if not "%no_nuget%"=="1" (
    where /q curl
    if errorlevel 1 (
        echo [Can't find `curl.exe` in PATH... skipping `.nuget_cache` population]
    ) else (
        echo [Found `curl.exe` in PATH... populating `.nuget_cache`]
        mkdir .nuget_cache
        pushd .nuget_cache
            call :fetch_nuget_package Microsoft.Direct3D.D3D12 %d3d_ver%
            call :fetch_nuget_package Microsoft.Direct3D.DXC %dxc_ver%
            call :fetch_nuget_package WinPixEventRuntime %pix_ver%
        popd
    )
)
:: setup NuGet paths
if not exist .nuget_cache set no_nuget=1

if not "%no_nuget%"=="1" (
    :: prepend D3D/DXC/PIX include paths to make sure headers in those directories are prioritised
    set "INCLUDE=%SRC_DIR%\.nuget_cache\Microsoft.Direct3D.D3D12.%d3d_ver%\build\native\include;!INCLUDE!"
    set "INCLUDE=%SRC_DIR%\.nuget_cache\Microsoft.Direct3D.DXC.%dxc_ver%\build\native\include;!INCLUDE!"
    set "INCLUDE=%SRC_DIR%\.nuget_cache\WinPixEventRuntime.%pix_ver%\Include\WinPixEventRuntime;!INCLUDE!"

    set "d3d_bin_path=%SRC_DIR%\.nuget_cache\Microsoft.Direct3D.D3D12.%d3d_ver%\build\native\bin\x64"
    set "dxc_bin_path=%SRC_DIR%\.nuget_cache\Microsoft.Direct3D.DXC.%dxc_ver%\build\native\bin\x64"
    set "pix_bin_path=%SRC_DIR%\.nuget_cache\WinPixEventRuntime.%pix_ver%\bin\x64"
    set "DXC=!dxc_bin_path!\dxc.exe"
) else (
    if not exist .nuget_cache (
        echo [`.nuget_cache` doesn't exist. Using `PATH` to find `dxc.exe`]
    ) else (
        echo [NuGet disabled. Using `PATH` to find `dxc.exe`]
    )
    where /q dxc
    if errorlevel 1 (
        echo [Can't find `dxc.exe` in `PATH`...]
    ) else (
        echo [Found `dxc.exe` in `PATH` at the following locations:]
        for /f "tokens=* delims=" %%i in ('^"dxc.exe -help ^| find "Version:"^"') do echo [    %%i]
        for /f "tokens=* delims=" %%i in ('where dxc') do echo [    %%i]
        set DXC="dxc.exe"
    )
)

if "%no_shaders%"=="1" (
    echo [Skipping building shaders, `no_shaders` was specified.]
) else if not defined DXC (
    echo [Skipping building shaders, `dxc.exe` wasn't found.]
) else (
    :: We rebuild shaders when any of the configs compiled
    if not exist .build_shaders mkdir .build_shaders
    pushd .build_shaders
        set dxc_flags_std=-HV 2021 -Zs -Qstrip_debug
        set dxc_flags_dbg=-Od !dxc_flags_std!
        set dxc_flags_opt=-O3 !dxc_flags_std!
        set dxc_compile_cs=call %DXC% -E main -T cs_6_0 !dxc_flags_opt!

        for /l %%i in (1,1,10) do (
            echo [Compiling Shader Permutations %%i0%%]
            for %%j in (0,1,2) do (
                set /a kernel_size_log2=%%i+%%j
                call :compile_cs threadgroup_bitonic_sort threadgroup_bitonic_sort_!kernel_size_log2!_%%i_0 -DKERNEL_SIZE_LOG2=!kernel_size_log2! -DTGROUP_SIZE_LOG2=%%i
                call :compile_cs threadgroup_bitonic_sort threadgroup_bitonic_sort_!kernel_size_log2!_%%i_1 -DKERNEL_SIZE_LOG2=!kernel_size_log2! -DTGROUP_SIZE_LOG2=%%i -DNO_WAVE_INTRINSICS=1
            )
        )
    popd
)

:: for a set of 'standard' ('std') flags that are shared between Debug/Release build for Microsoft Visual C++ compiler, we disable only
::      -4514 - 'function' : unreferenced inline function has been removed
::      -4505 - 'function' : unreferenced local function has been removed
set msvc_compile_flags_std=/nologo /Zi /GR- /EHsc /Zl /permissive- /Wall /WX /WL /wd4514 /wd4505 /I %SRC_DIR%\.build_shaders
:: for a set of flags for Debug build for Microsoft Visual C++ compiler, we disable
::      - 5045 - Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
set msvc_compile_flags_dbg=/Od /GS /RTCscu /D_ALLOW_RTCc_IN_STL=1 /D_DEBUG=1 /wd5045 %msvc_compile_flags_std%
:: for a set of flags for Release/Optimised build for Microsoft Visual C++ compiler, we disable
::      - 4710 - 'function' : function not inlined
::      - 4711 - function 'function' selected for inline expansion
set msvc_compile_flags_opt=/O2 /GL /GS- /DNDEBUG=1 /wd4710 /wd4711 %msvc_compile_flags_std%

set msvc_link_flags_std=/nologo /incremental:no /nodefaultlib /subsystem:console /machine:x64 /debug
set msvc_link_flags_opt=/ltcg %msvc_link_flags_std%
set msvc_link_libs_std=kernel32.lib shell32.lib ole32.lib

:: According to https://learn.microsoft.com/en-us/cpp/c-runtime-library/crt-library-features?view=msvc-170
::      - msvcrt.lib        - static library for the native CRT startup for use with UCRT and vcruntime DLLs
::      - ucrt.lib          - DLL import library for the UCRT
::      - vcruntime.lib     - DLL import library for the vcruntime
::
::      - libcmt.lib        - statically links the native CRT start into your code
::      - libucrt.lib       - statically links the UCRT into your code
::      - libvcruntime.lib  - VCRuntime statically linked into your code
:: VCRuntime - contains Visual C++ CRT implemtation-specific code: exception handling and debugging support,
:: runtime checks and type information, implementation details, and certain extended library functions.
:: The VCRuntime library version needs to match the version of the compiler you're using.
::
:: set msvc_link_libs_opt=msvcrt.lib ucrt.lib vcruntime.lib
:: set msvc_link_libs_opt=libcmt.lib libucrt.lib libvcruntime.lib

set msvc_link_libs_opt=libcmt.lib libucrt.lib libvcruntime.lib
set msvc_link_libs_dbg=msvcrtd.lib ucrtd.lib vcruntimed.lib
::libcmtd.lib libucrtd.lib libvcruntimed.lib

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
        :: copy the required DLLs if NuGet packages exist or were acquired
        if defined d3d_bin_path if exist %d3d_bin_path%\D3D12Core.dll           copy /v /y /b %d3d_bin_path%\D3D12Core.dll . >nul
        if defined d3d_bin_path if exist %d3d_bin_path%\d3d12SDKLayers.dll      copy /v /y /b %d3d_bin_path%\d3d12SDKLayers.dll . >nul
        if defined pix_bin_path if exist %pix_bin_path%\WinPixEventRuntime.dll  copy /v /y /b %pix_bin_path%\WinPixEventRuntime.dll . >nul

        if "%1"=="debug" (
            call cl.exe %msvc_compile_flags_dbg% ..\demo.cpp /link %msvc_link_flags_std% %msvc_link_libs_std% %msvc_link_libs_dbg% /out:demo_%1.exe
        ) else if "%1"=="release" (
            call cl.exe %msvc_compile_flags_opt% ..\demo.cpp /link %msvc_link_flags_opt% %msvc_link_libs_std% %msvc_link_libs_opt% /out:demo_%1.exe
        )
    popd
exit /b 0

:: this function requries
::      - the first argument to be the name of source file
::      - the second argument to be the name of the destination file
:compile_cs
    setlocal
    :: split input arguments into 1st, 2nd and the rest.
    :: The 1st and 2nd used as source and destination names while everything else is passed into `dxc.exe`
    for /f "tokens=1,2,* delims= " %%g in ("%*") do (
        set "src=%%g"
        set "dst=%%h"
        set "args=%%i"
    )
    %dxc_compile_cs% %args% -Fo shader_%dst%.cso -Fc shader_%dst%.asm -Fh shader_%dst%.h -Vn shader_%dst% -Fd shader_%dst%.pdb ..\%src%.hlsl
    endlocal
exit /b 0

:fetch_nuget_package
    start /wait /b curl -L -s -o %1.%2.zip https://www.nuget.org/api/v2/package/%1/%2
    mkdir %1.%2
    start /wait /b tar -xf %1.%2.zip -C %1.%2
exit /b 0

:eof
