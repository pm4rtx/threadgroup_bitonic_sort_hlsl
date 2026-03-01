
#ifndef COMPILER_PRAGMA_MSVC
#   if defined(_MSC_VER)
#       define COMPILER_PRAGMA_MSVC(x) __pragma(x)
#   else
#       define COMPILER_PRAGMA_MSVC(x)
#   endif
#endif /* COMPILER_PRAGMA_MSVC */

#ifndef COMPILER_PRAGMA_GNUC
#   if defined(__clang__) || defined(__GNUC__)
#       define COMPILER_PRAGMA_GNUC(x) _Pragma(#x)
#   else
#       define COMPILER_PRAGMA_GNUC(x)
#   endif
#endif /* COMPILER_PRAGMA_GNUC */

#ifndef COMPILER_WARNING_DISABLE_GNUC
#   if defined(__clang__) || defined(__GNUC__)
#       if defined(__clang__)
#           define COMPILER_WARNING_GNUC_PUSH()     COMPILER_PRAGMA_GNUC(clang diagnostic push)
#           define COMPILER_WARNING_GNUC_POP()      COMPILER_PRAGMA_GNUC(clang diagnostic pop)
#           define COMPILER_WARNING_DISABLE_GNUC(w) COMPILER_PRAGMA_GNUC(clang diagnostic ignored #w)
#       elif defined(__GNUC__)
#           define COMPILER_WARNING_GNUC_PUSH()     COMPILER_PRAGMA_GNUC(GCC diagnostic push)
#           define COMPILER_WARNING_GNUC_POP()      COMPILER_PRAGMA_GNUC(GCC diagnostic pop)
#           define COMPILER_WARNING_DISABLE_GNUC(w) COMPILER_PRAGMA_GNUC(GCC diagnostic ignored #w)
#       endif
#   elif defined (_MSC_VER)
#       define COMPILER_WARNING_GNUC_PUSH()
#       define COMPILER_WARNING_GNUC_POP()
#       define COMPILER_WARNING_DISABLE_GNUC(w)
#   else
#       error Unknown Compiler
#   endif
#endif /** COMPILER_WARNING_DISABLE_GNUC */

#ifndef COMPILER_WARNING_DISABLE_MSVC
#   if defined(_MSC_VER)
#       define COMPILER_WARNING_MSVC_PUSH()     COMPILER_PRAGMA_MSVC(warning(push))
#       define COMPILER_WARNING_MSVC_POP()      COMPILER_PRAGMA_MSVC(warning(pop))
#       define COMPILER_WARNING_DISABLE_MSVC(w) COMPILER_PRAGMA_MSVC(warning(disable : w))
#       if _MSC_VER >= 1912
#           define COMPILER_WARNING_DISABLE_MSVC1912(w) COMPILER_PRAGMA_MSVC(warning(disable : w))
#       else
#           define COMPILER_WARNING_DISABLE_MSVC1912(w)
#       endif
#       if _MSC_VER >= 1920
#           define COMPILER_WARNING_DISABLE_MSVC19(w) COMPILER_PRAGMA_MSVC(warning(disable : w))
#       else
#           define COMPILER_WARNING_DISABLE_MSVC19(w)
#       endif
#   elif defined(__clang__) || defined(__GNUC__)
#       define COMPILER_WARNING_MSVC_PUSH()
#       define COMPILER_WARNING_MSVC_POP()
#       define COMPILER_WARNING_DISABLE_MSVC(w)
#       define COMPILER_WARNING_DISABLE_MSVC19(w)
#   else
#       error Unknown Compiler
#   endif
#endif /** COMPILER_WARNING_DISABLE_MSVC */

#if !defined(COMPILER_WARNING_PUSH) && !defined(COMPILER_WARNING_POP)
#   define COMPILER_WARNING_PUSH() COMPILER_WARNING_MSVC_PUSH() COMPILER_WARNING_GNUC_PUSH()
#   define COMPILER_WARNING_POP()  COMPILER_WARNING_MSVC_POP()  COMPILER_WARNING_GNUC_POP()
#endif /** COMPILER_WARNING_PUSH/COMPILER_WARNING_POP */

#include <stdint.h>
#include <stdio.h>
#include <math.h>

COMPILER_WARNING_PUSH()
COMPILER_WARNING_DISABLE_MSVC(4820) /** warning C4820: 'Name A' : 'N' bytes padding added after data member 'Name B' */
COMPILER_WARNING_DISABLE_MSVC(4255) /** warning C4255: 'Name A' : no function prototype given: converting '()' to '(void)' */
COMPILER_WARNING_DISABLE_MSVC(4365) /** warning C4365: 'Name A' : 'return' : conversion from 'Type A' to 'Type B', signed/unsigned mismatch */
COMPILER_WARNING_DISABLE_MSVC(4480)
COMPILER_WARNING_DISABLE_MSVC(4668) /** warning C4668: 'Name A' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif' */
COMPILER_WARNING_DISABLE_MSVC(5039) /** warning C5039: 'Name A': pointer or reference to potentially throwing function passed to extern C function under -EHc. Undefined behavior may occur if this function throws an exception. Note: to implify migration, consider the temporary use of /Wv:18 flag with the version of the compiler with which you used to build without warnings */
#include <windows.h>
COMPILER_WARNING_POP()

COMPILER_WARNING_PUSH()
COMPILER_WARNING_DISABLE_MSVC(4255) /** warning C4255: 'Name A' : no function prototype given: converting '()' to '(void)' */
COMPILER_WARNING_DISABLE_MSVC(4820) /** warning C4820: 'Name A' : 'N' bytes padding added after data member 'Name B' */
#include <dxgi1_6.h>
COMPILER_WARNING_POP()

COMPILER_WARNING_PUSH()
COMPILER_WARNING_DISABLE_MSVC(4820) /** warning C4820: 'Name A' : 'N' bytes padding added after data member 'Name B' */
COMPILER_WARNING_DISABLE_MSVC(4201) /** warning C4201: nonstandard extension used : nameless struct/union */
#include <d3d12.h>
COMPILER_WARNING_POP()

COMPILER_WARNING_PUSH()
COMPILER_WARNING_DISABLE_MSVC(4365) /** warning C4365:  '=': conversion from 'Type A' to 'Type B', signed/unsigned mismatch */
COMPILER_WARNING_DISABLE_MSVC(4820) /** warning C4820: 'Name A' : 'N' bytes padding added after data member 'Name B' */
COMPILER_WARNING_DISABLE_MSVC1912(5039)
#define STB_SPRINTF_IMPLEMENTATION 1
#include "stb_sprintf.h"
COMPILER_WARNING_POP()

#include <shader_threadgroup_bitonic_sort_1_1_0.h>
#include <shader_threadgroup_bitonic_sort_2_1_0.h>
#include <shader_threadgroup_bitonic_sort_2_2_0.h>
#include <shader_threadgroup_bitonic_sort_3_1_0.h>
#include <shader_threadgroup_bitonic_sort_3_2_0.h>
#include <shader_threadgroup_bitonic_sort_3_3_0.h>
#include <shader_threadgroup_bitonic_sort_4_2_0.h>
#include <shader_threadgroup_bitonic_sort_4_3_0.h>
#include <shader_threadgroup_bitonic_sort_4_4_0.h>
#include <shader_threadgroup_bitonic_sort_5_3_0.h>
#include <shader_threadgroup_bitonic_sort_5_4_0.h>
#include <shader_threadgroup_bitonic_sort_5_5_0.h>
#include <shader_threadgroup_bitonic_sort_6_4_0.h>
#include <shader_threadgroup_bitonic_sort_6_5_0.h>
#include <shader_threadgroup_bitonic_sort_6_6_0.h>
#include <shader_threadgroup_bitonic_sort_7_5_0.h>
#include <shader_threadgroup_bitonic_sort_7_6_0.h>
#include <shader_threadgroup_bitonic_sort_7_7_0.h>
#include <shader_threadgroup_bitonic_sort_8_6_0.h>
#include <shader_threadgroup_bitonic_sort_8_7_0.h>
#include <shader_threadgroup_bitonic_sort_8_8_0.h>
#include <shader_threadgroup_bitonic_sort_9_7_0.h>
#include <shader_threadgroup_bitonic_sort_9_8_0.h>
#include <shader_threadgroup_bitonic_sort_9_9_0.h>
#include <shader_threadgroup_bitonic_sort_10_8_0.h>
#include <shader_threadgroup_bitonic_sort_10_9_0.h>
#include <shader_threadgroup_bitonic_sort_10_10_0.h>
#include <shader_threadgroup_bitonic_sort_11_9_0.h>
#include <shader_threadgroup_bitonic_sort_11_10_0.h>
#include <shader_threadgroup_bitonic_sort_12_10_0.h>

#include <shader_threadgroup_bitonic_sort_1_1_1.h>
#include <shader_threadgroup_bitonic_sort_2_1_1.h>
#include <shader_threadgroup_bitonic_sort_2_2_1.h>
#include <shader_threadgroup_bitonic_sort_3_1_1.h>
#include <shader_threadgroup_bitonic_sort_3_2_1.h>
#include <shader_threadgroup_bitonic_sort_3_3_1.h>
#include <shader_threadgroup_bitonic_sort_4_2_1.h>
#include <shader_threadgroup_bitonic_sort_4_3_1.h>
#include <shader_threadgroup_bitonic_sort_4_4_1.h>
#include <shader_threadgroup_bitonic_sort_5_3_1.h>
#include <shader_threadgroup_bitonic_sort_5_4_1.h>
#include <shader_threadgroup_bitonic_sort_5_5_1.h>
#include <shader_threadgroup_bitonic_sort_6_4_1.h>
#include <shader_threadgroup_bitonic_sort_6_5_1.h>
#include <shader_threadgroup_bitonic_sort_6_6_1.h>
#include <shader_threadgroup_bitonic_sort_7_5_1.h>
#include <shader_threadgroup_bitonic_sort_7_6_1.h>
#include <shader_threadgroup_bitonic_sort_7_7_1.h>
#include <shader_threadgroup_bitonic_sort_8_6_1.h>
#include <shader_threadgroup_bitonic_sort_8_7_1.h>
#include <shader_threadgroup_bitonic_sort_8_8_1.h>
#include <shader_threadgroup_bitonic_sort_9_7_1.h>
#include <shader_threadgroup_bitonic_sort_9_8_1.h>
#include <shader_threadgroup_bitonic_sort_9_9_1.h>
#include <shader_threadgroup_bitonic_sort_10_8_1.h>
#include <shader_threadgroup_bitonic_sort_10_9_1.h>
#include <shader_threadgroup_bitonic_sort_10_10_1.h>
#include <shader_threadgroup_bitonic_sort_11_9_1.h>
#include <shader_threadgroup_bitonic_sort_11_10_1.h>
#include <shader_threadgroup_bitonic_sort_12_10_1.h>

#define USE_PIX 1
#if USE_PIX
    COMPILER_WARNING_PUSH()
    COMPILER_WARNING_DISABLE_MSVC(4820) /** warning C4820: 'Name A' : 'N' bytes padding added after data member 'Name B' */
    COMPILER_WARNING_DISABLE_MSVC(4201) /** warning C4201: nonstandard extension used : nameless struct/union */
    COMPILER_WARNING_DISABLE_MSVC(4365) /** warning C4365: 'initializing': conversion from 'type A' to 'type B' signed/unsigned mismatch */
    COMPILER_WARNING_DISABLE_MSVC(5039) /** warning C5039: 'Name A': pointer or reference to potentially throwing function passed to extern C function under -EHc. Undefined behavior may occur if this function throws an exception. Note: to implify migration, consider the temporary use of /Wv:18 flag with the version of the compiler with which you used to build without warnings */
#   include <pix3.h>
    COMPILER_WARNING_POP()
#endif

COMPILER_WARNING_PUSH()
COMPILER_WARNING_DISABLE_MSVC19(5045) /** warning C5045: Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified */

static void debugPrint(const char *msg);
static void debugPrintF(const char *format, ...);

#define D3D12AID_CHECK(call)                            \
    do                                                  \
    {                                                   \
        HRESULT hr = call;                              \
        if (S_OK != hr)                                 \
        {                                               \
            debugPrintF("S_OK != 0x%08lx " #call "\n", hr);  \
            if (IsDebuggerPresent())                    \
                __debugbreak();                         \
        }                                               \
    }                                                   \
    while(0)
#define D3D12AID_ASSERT(cond)                               \
    do                                                      \
    {                                                       \
        if (!(cond))                                        \
        {                                                   \
            debugPrint("Assert with condition "#cond" failed.");  \
            if (IsDebuggerPresent())                        \
                __debugbreak();                             \
        }                                                   \
    }                                                       \
    while(0)

#include "d3d12aid.h"
COMPILER_WARNING_POP()

extern "C" { __declspec(dllexport) extern const UINT D3D12SDKVersion = D3D12_SDK_VERSION;}

extern "C" { __declspec(dllexport) extern const char* D3D12SDKPath = u8".\\"; }

typedef struct module_t
{
    HMODULE handle;
    HMODULE unload;
} module_t;

static void load_module(module_t *out_module, const wchar_t *name)
{
    out_module->handle = GetModuleHandleW(name);
    out_module->unload = NULL;
    if (NULL == out_module->handle)
    {
        out_module->handle = out_module->unload = LoadLibraryW(name);
    }
}

static void unload_module(module_t *inout_module)
{
    if (NULL != inout_module->unload)
    {
        FreeLibrary(inout_module->unload);
    }
    inout_module->handle = NULL;
    inout_module->unload = NULL;
}

#define DECLARE_IMPORT(name, ...)                                       \
    typedef HRESULT WINAPI pfn##name(__VA_ARGS__);                      \
    union ucvt_##name                                                   \
    {                                                                   \
        pfn##name *typedFn;                                             \
        FARPROC    farprFn;                                             \
    };                                                                  \
    typedef ucvt_##name ucvt_##name;                                    \
    static pfn##name *fn##name = NULL;                                  \
    static void load_##name(const module_t *module)                     \
    {                                                                   \
        ucvt_##name cvt;                                                \
        cvt.farprFn = GetProcAddress(module->handle, #name);            \
        fn##name = cvt.typedFn;                                         \
    }

DECLARE_IMPORT(CreateDXGIFactory2, UINT Flags, REFIID riid, void **ppFactory);
DECLARE_IMPORT(D3D12CreateDevice, IUnknown *pAdapter, D3D_FEATURE_LEVEL MinimumFeatureLevel, REFIID riid, void **ppDevice);

typedef void benchmark_ForEachDeviceCback(IDXGIAdapter *adapter, const DXGI_ADAPTER_DESC1 *desc, ID3D12Device *device, const char *deviceName, void *userdata);
static int benchmark_ForEachDevice(benchmark_ForEachDeviceCback *callback, void *userdata)
{
    module_t d3d12;
    module_t dxgi;
    IDXGIFactory7 *factory = NULL;

    load_module(&dxgi, L"dxgi.dll");
    load_module(&d3d12, L"d3d12.dll");
    load_CreateDXGIFactory2(&dxgi);
    load_D3D12CreateDevice(&d3d12);
    D3D12AID_CHECK(fnCreateDXGIFactory2(0, IID_PPV_ARGS(&factory)));
    IDXGIAdapter1 *adapter = NULL;
    for (uint32_t adapterIdx = 0; DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(adapterIdx, &adapter); ++adapterIdx)
    {
        DXGI_ADAPTER_DESC1 desc;
        D3D12AID_CHECK(adapter->GetDesc1(&desc));
        if (desc.VendorId != 0x1414)
        {
            static const char deviceNamePattern [] = "venId_0x0000_devId_0x0000_revId_0x00_";
            char deviceDescription[_countof(desc.Description)];
            char deviceName[_countof(deviceDescription) + _countof(deviceNamePattern) - 1];
            if (0 != WideCharToMultiByte(CP_UTF8, 0, desc.Description, -1, deviceDescription, _countof(deviceDescription), NULL, NULL))
            {
                debugPrintF("Found: vendorId=0x%04x, deviceId=0x%04x, revision=0x%02x, %s\n", desc.VendorId, desc.DeviceId, desc.Revision, deviceDescription);
                stbsp_snprintf(deviceName, _countof(deviceName), "venId_0x%04x_devId_0x%04x_revId_0x%02x_%s", desc.VendorId, desc.DeviceId, desc.Revision, deviceDescription);
            }
            ID3D12Device *device = NULL;
            D3D12AID_CHECK(fnD3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)));
            D3D12AID_CHECK(device->SetStablePowerState(TRUE));
            callback(adapter, &desc, device, deviceName, userdata);
            D3D12AID_CHECK(device->SetStablePowerState(FALSE));
            D3D12AID_SAFE_RELEASE(device);
        }
        D3D12AID_SAFE_RELEASE(adapter);
    }
    D3D12AID_SAFE_RELEASE(factory);
    unload_module(&d3d12);
    unload_module(&dxgi);
    return 0;
}

static void debugPrint(const char *msg)
{
    DWORD len = (DWORD)strlen(msg);
    OutputDebugStringA(msg);
    WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), msg, len, NULL, NULL);
}

static void debugPrintF(const char *format, ...)
{
    char buffer[1024];
    int result;
    va_list va;
    va_start(va, format);
    result = stbsp_vsnprintf(buffer, sizeof(buffer), format, va);
    STBSP__NOTUSED(result);
    debugPrint(buffer);
    va_end(va);
}

struct ShaderBytecode
{
    uint32_t    kernelSizeLog2;
    uint32_t    tgroupSizeLog2;
    size_t      shaderBytecodeSizeInBytes;
    const void *shaderBytecode;
};

typedef struct ShaderBytecode ShaderBytecode;

static const ShaderBytecode GShaderBytecodes[] =
{
    { 1 , 1 , sizeof(shader_threadgroup_bitonic_sort_1_1_0), shader_threadgroup_bitonic_sort_1_1_0 },
    { 2 , 1 , sizeof(shader_threadgroup_bitonic_sort_2_1_0), shader_threadgroup_bitonic_sort_2_1_0 },
    { 2 , 2 , sizeof(shader_threadgroup_bitonic_sort_2_2_0), shader_threadgroup_bitonic_sort_2_2_0 },
    { 3 , 1 , sizeof(shader_threadgroup_bitonic_sort_3_1_0), shader_threadgroup_bitonic_sort_3_1_0 },
    { 3 , 2 , sizeof(shader_threadgroup_bitonic_sort_3_2_0), shader_threadgroup_bitonic_sort_3_2_0 },
    { 3 , 3 , sizeof(shader_threadgroup_bitonic_sort_3_3_0), shader_threadgroup_bitonic_sort_3_3_0 },
    { 4 , 2 , sizeof(shader_threadgroup_bitonic_sort_4_2_0), shader_threadgroup_bitonic_sort_4_2_0 },
    { 4 , 3 , sizeof(shader_threadgroup_bitonic_sort_4_3_0), shader_threadgroup_bitonic_sort_4_3_0 },
    { 4 , 4 , sizeof(shader_threadgroup_bitonic_sort_4_4_0), shader_threadgroup_bitonic_sort_4_4_0 },
    { 5 , 3 , sizeof(shader_threadgroup_bitonic_sort_5_3_0), shader_threadgroup_bitonic_sort_5_3_0 },
    { 5 , 4 , sizeof(shader_threadgroup_bitonic_sort_5_4_0), shader_threadgroup_bitonic_sort_5_4_0 },
    { 5 , 5 , sizeof(shader_threadgroup_bitonic_sort_5_5_0), shader_threadgroup_bitonic_sort_5_5_0 },
    { 6 , 4 , sizeof(shader_threadgroup_bitonic_sort_6_4_0), shader_threadgroup_bitonic_sort_6_4_0 },
    { 6 , 5 , sizeof(shader_threadgroup_bitonic_sort_6_5_0), shader_threadgroup_bitonic_sort_6_5_0 },
    { 6 , 6 , sizeof(shader_threadgroup_bitonic_sort_6_6_0), shader_threadgroup_bitonic_sort_6_6_0 },
    { 7 , 5 , sizeof(shader_threadgroup_bitonic_sort_7_5_0), shader_threadgroup_bitonic_sort_7_5_0 },
    { 7 , 6 , sizeof(shader_threadgroup_bitonic_sort_7_6_0), shader_threadgroup_bitonic_sort_7_6_0 },
    { 7 , 7 , sizeof(shader_threadgroup_bitonic_sort_7_7_0), shader_threadgroup_bitonic_sort_7_7_0 },
    { 8 , 6 , sizeof(shader_threadgroup_bitonic_sort_8_6_0), shader_threadgroup_bitonic_sort_8_6_0 },
    { 8 , 7 , sizeof(shader_threadgroup_bitonic_sort_8_7_0), shader_threadgroup_bitonic_sort_8_7_0 },
    { 8 , 8 , sizeof(shader_threadgroup_bitonic_sort_8_8_0), shader_threadgroup_bitonic_sort_8_8_0 },
    { 9 , 7 , sizeof(shader_threadgroup_bitonic_sort_9_7_0), shader_threadgroup_bitonic_sort_9_7_0 },
    { 9 , 8 , sizeof(shader_threadgroup_bitonic_sort_9_8_0), shader_threadgroup_bitonic_sort_9_8_0 },
    { 9 , 9 , sizeof(shader_threadgroup_bitonic_sort_9_9_0), shader_threadgroup_bitonic_sort_9_9_0 },
    { 10, 8 , sizeof(shader_threadgroup_bitonic_sort_10_8_0), shader_threadgroup_bitonic_sort_10_8_0 },
    { 10, 9 , sizeof(shader_threadgroup_bitonic_sort_10_9_0), shader_threadgroup_bitonic_sort_10_9_0 },
    { 10, 10, sizeof(shader_threadgroup_bitonic_sort_10_10_0), shader_threadgroup_bitonic_sort_10_10_0 },
    { 11, 9 , sizeof(shader_threadgroup_bitonic_sort_11_9_0), shader_threadgroup_bitonic_sort_11_9_0 },
    { 11, 10, sizeof(shader_threadgroup_bitonic_sort_11_10_0), shader_threadgroup_bitonic_sort_11_10_0 },
    { 12, 10, sizeof(shader_threadgroup_bitonic_sort_12_10_0), shader_threadgroup_bitonic_sort_12_10_0 }
};

static const ShaderBytecode GShaderBytecodesNoWaveIntrinsics[] =
{
    { 1 , 1 , sizeof(shader_threadgroup_bitonic_sort_1_1_1), shader_threadgroup_bitonic_sort_1_1_1 },
    { 2 , 1 , sizeof(shader_threadgroup_bitonic_sort_2_1_1), shader_threadgroup_bitonic_sort_2_1_1 },
    { 2 , 2 , sizeof(shader_threadgroup_bitonic_sort_2_2_1), shader_threadgroup_bitonic_sort_2_2_1 },
    { 3 , 1 , sizeof(shader_threadgroup_bitonic_sort_3_1_1), shader_threadgroup_bitonic_sort_3_1_1 },
    { 3 , 2 , sizeof(shader_threadgroup_bitonic_sort_3_2_1), shader_threadgroup_bitonic_sort_3_2_1 },
    { 3 , 3 , sizeof(shader_threadgroup_bitonic_sort_3_3_1), shader_threadgroup_bitonic_sort_3_3_1 },
    { 4 , 2 , sizeof(shader_threadgroup_bitonic_sort_4_2_1), shader_threadgroup_bitonic_sort_4_2_1 },
    { 4 , 3 , sizeof(shader_threadgroup_bitonic_sort_4_3_1), shader_threadgroup_bitonic_sort_4_3_1 },
    { 4 , 4 , sizeof(shader_threadgroup_bitonic_sort_4_4_1), shader_threadgroup_bitonic_sort_4_4_1 },
    { 5 , 3 , sizeof(shader_threadgroup_bitonic_sort_5_3_1), shader_threadgroup_bitonic_sort_5_3_1 },
    { 5 , 4 , sizeof(shader_threadgroup_bitonic_sort_5_4_1), shader_threadgroup_bitonic_sort_5_4_1 },
    { 5 , 5 , sizeof(shader_threadgroup_bitonic_sort_5_5_1), shader_threadgroup_bitonic_sort_5_5_1 },
    { 6 , 4 , sizeof(shader_threadgroup_bitonic_sort_6_4_1), shader_threadgroup_bitonic_sort_6_4_1 },
    { 6 , 5 , sizeof(shader_threadgroup_bitonic_sort_6_5_1), shader_threadgroup_bitonic_sort_6_5_1 },
    { 6 , 6 , sizeof(shader_threadgroup_bitonic_sort_6_6_1), shader_threadgroup_bitonic_sort_6_6_1 },
    { 7 , 5 , sizeof(shader_threadgroup_bitonic_sort_7_5_1), shader_threadgroup_bitonic_sort_7_5_1 },
    { 7 , 6 , sizeof(shader_threadgroup_bitonic_sort_7_6_1), shader_threadgroup_bitonic_sort_7_6_1 },
    { 7 , 7 , sizeof(shader_threadgroup_bitonic_sort_7_7_1), shader_threadgroup_bitonic_sort_7_7_1 },
    { 8 , 6 , sizeof(shader_threadgroup_bitonic_sort_8_6_1), shader_threadgroup_bitonic_sort_8_6_1 },
    { 8 , 7 , sizeof(shader_threadgroup_bitonic_sort_8_7_1), shader_threadgroup_bitonic_sort_8_7_1 },
    { 8 , 8 , sizeof(shader_threadgroup_bitonic_sort_8_8_1), shader_threadgroup_bitonic_sort_8_8_1 },
    { 9 , 7 , sizeof(shader_threadgroup_bitonic_sort_9_7_1), shader_threadgroup_bitonic_sort_9_7_1 },
    { 9 , 8 , sizeof(shader_threadgroup_bitonic_sort_9_8_1), shader_threadgroup_bitonic_sort_9_8_1 },
    { 9 , 9 , sizeof(shader_threadgroup_bitonic_sort_9_9_1), shader_threadgroup_bitonic_sort_9_9_1 },
    { 10, 8 , sizeof(shader_threadgroup_bitonic_sort_10_8_1), shader_threadgroup_bitonic_sort_10_8_1 },
    { 10, 9 , sizeof(shader_threadgroup_bitonic_sort_10_9_1), shader_threadgroup_bitonic_sort_10_9_1 },
    { 10, 10, sizeof(shader_threadgroup_bitonic_sort_10_10_1), shader_threadgroup_bitonic_sort_10_10_1 },
    { 11, 9 , sizeof(shader_threadgroup_bitonic_sort_11_9_1), shader_threadgroup_bitonic_sort_11_9_1 },
    { 11, 10, sizeof(shader_threadgroup_bitonic_sort_11_10_1), shader_threadgroup_bitonic_sort_11_10_1 },
    { 12, 10, sizeof(shader_threadgroup_bitonic_sort_12_10_1), shader_threadgroup_bitonic_sort_12_10_1 }
};

typedef struct PerfData
{
    uint64_t    sumTime;
    uint64_t    minTime;
    uint64_t    maxTime;
    double      avgTime;
    double      varTime;
    uint64_t    addCount;
    uint64_t    frq;
} PerfData;

static void perfData_Init(PerfData *inoutPerfData, uint64_t frq)
{
    inoutPerfData->sumTime  = 0;
    inoutPerfData->minTime  = ~0ull;
    inoutPerfData->maxTime  = 0;
    inoutPerfData->avgTime  = 0.0;
    inoutPerfData->varTime  = 0.0;
    inoutPerfData->addCount = 0;
    inoutPerfData->frq      = frq;
}

static void perfData_AddSample(PerfData *inoutPerfData, uint64_t sample)
{
    double delta1 = 0.0;
    double delta2 = 0.0;
    double sampleF64 = (double)sample;
    inoutPerfData->sumTime += sample;
    if (inoutPerfData->minTime > sample)
    {
        inoutPerfData->minTime = sample;
    }
    if (inoutPerfData->maxTime < sample)
    {
        inoutPerfData->maxTime = sample;
    }
    /** use Welford's algorithm to compue running mean(avg) and variance(to compute standard deviation)*/
    inoutPerfData->addCount += 1;
    delta1 = sampleF64 - inoutPerfData->avgTime;
    inoutPerfData->avgTime += delta1 / (double)inoutPerfData->addCount;
    delta2 = sampleF64 - inoutPerfData->avgTime;
    inoutPerfData->varTime += delta1 * delta2;
}

static void perfData_PrintHeader()
{
    debugPrintF("%*s | Min Time (us) | Max Time (us) | Mean Time (us) |       StdDev0 (us) |       StdDev1 (us) | Total Time (us) | Mean Time per Elem (ns) |\n", 54, "");
}

static void perfData_Print(const PerfData *inPerfData, uint32_t workItemCount)
{
    uint64_t popCount = inPerfData->addCount;
    uint64_t smpCount = popCount - ((inPerfData->addCount == 0) ? 0u : 1u);

    uint64_t minTimeUs = (inPerfData->minTime * 1000000ULL) / inPerfData->frq;
    uint64_t maxTimeUs = (inPerfData->maxTime * 1000000ULL) / inPerfData->frq;
    uint64_t sumTimeUs = (inPerfData->sumTime * 1000000ULL) / inPerfData->frq;
    double avgTimeUs = (inPerfData->avgTime * 1000000.0) / (double)inPerfData->frq;

    double popStdevTimeClocks = sqrt(inPerfData->varTime / (double) popCount);
    double smpStdevTimeClocks = sqrt(inPerfData->varTime / (double) smpCount);

    double popStdevTimeUs = (popStdevTimeClocks * 1000000) / (double)inPerfData->frq;
    double smpStdevTimeUs = (smpStdevTimeClocks * 1000000) / (double)inPerfData->frq;

    double avgTimeUsF64 = (double)avgTimeUs;

    debugPrintF("| %13llu | %13llu | %14.3f | +/- %4.1f%% %8.3f | +/- %4.1f%% %8.3f | %15llu | %23.3f |\n",
        minTimeUs, maxTimeUs, avgTimeUs,
        popStdevTimeUs * 100.0 / avgTimeUsF64, popStdevTimeUs,
        smpStdevTimeUs * 100.0 / avgTimeUsF64, smpStdevTimeUs,
        sumTimeUs,
        avgTimeUs * 1000.0 / (double)workItemCount
    );
}

typedef struct benchmark_SharedData
{
    uint32_t options;
    char     captureDrive;
    char     padding[3];
} benchmark_SharedData;

static void benchmark_threadgroup_bitonic_sort_Cback(IDXGIAdapter *adapter, const DXGI_ADAPTER_DESC1 *desc, ID3D12Device *device, const char *deviceName, void *userdata);

static const uint32_t kCmdBufferInFlight = 3;
static const uint32_t kMaxSortKeysPerTg = 4096;
static const uint32_t kBenchmarkFrameCount = 10;

int main(int argc, char **argv)
{
    benchmark_SharedData sharedData;
    sharedData.options = 0;
    for (int argi = 1; argi < argc; ++argi)
    {
        if (0 == strcmp("--pixcaptures", argv[argi]))
        {
            sharedData.options ^= 0x1u;
        }
    }

#if USE_PIX
    if (sharedData.options & 0x1u)
    {
        PIXLoadLatestWinPixGpuCapturerLibrary();
    }
#endif
    sharedData.captureDrive = argv[0][0];
    return benchmark_ForEachDevice(benchmark_threadgroup_bitonic_sort_Cback, &sharedData);
}

void benchmark_threadgroup_bitonic_sort_Cback(IDXGIAdapter *adapter, const DXGI_ADAPTER_DESC1 *desc, ID3D12Device *device, const char *deviceName, void *userdata)
{
    (void)adapter;
    (void)desc;
    (void)deviceName;
    const benchmark_SharedData *sharedData = (const benchmark_SharedData *)userdata;
    D3D12_FEATURE_DATA_D3D12_OPTIONS1 options1;
    D3D12AID_CHECK(device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS1, &options1, sizeof(options1)));
    debugPrintF("Total Lane Count %u\n", options1.TotalLaneCount);

    const uint32_t kSortKeysPerDispatch = options1.TotalLaneCount * kMaxSortKeysPerTg;

    d3d12aid_CmdQueue queue;
    d3d12aid_CmdQueue_Create(&queue, device, kCmdBufferInFlight, 1, D3D12_COMMAND_LIST_TYPE_DIRECT);

    debugPrintF("Created D3D12 Command Queue, Allocators and Lists ...\n");

    uint64_t gpuTimestampFreq = 0;
    D3D12AID_CHECK(queue.queue->GetTimestampFrequency(&gpuTimestampFreq));

    PerfData perfData;

    d3d12aid_Timestamps timestamps;
    d3d12aid_Timestamps_Create(&timestamps, device, 2, kCmdBufferInFlight);

    d3d12aid_MappedBuffer sortInput;
    d3d12aid_MappedBuffer sortOutput;
    d3d12aid_MappedBuffer_Create(&sortInput, device, 1, sizeof(uint32_t) * kSortKeysPerDispatch, D3D12_HEAP_TYPE_UPLOAD);
    d3d12aid_MappedBuffer_Create(&sortOutput, device, kCmdBufferInFlight, sizeof(uint32_t) * kSortKeysPerDispatch, D3D12_HEAP_TYPE_READBACK);

    uint32_t *randomSortInput = (uint32_t *)malloc(sizeof(uint32_t) * kSortKeysPerDispatch);
    // initialize with a sequence of increasing order to check it's going to be sorted in decreasing order
    for (uint32_t i = 0; i < kSortKeysPerDispatch; ++i)
    {
        randomSortInput[i] = i;
    }
    d3d12aid_MappedBuffer_Append(&sortInput, 0, randomSortInput, sizeof(uint32_t) * kSortKeysPerDispatch);
    free(randomSortInput);

    const uint32_t kShaderMaxCount = _countof(GShaderBytecodesNoWaveIntrinsics) + _countof(GShaderBytecodes);

    // We test kernels w/ and w/o wave intrinsics support if possible
    const uint32_t kShaderCount = _countof(GShaderBytecodesNoWaveIntrinsics) + (options1.WaveOps ? _countof(GShaderBytecodes) : 0);

    d3d12aid_ComputeRsPs rspsBitonicSort[kShaderMaxCount];

    for (uint32_t i = 0; i < _countof(GShaderBytecodesNoWaveIntrinsics); ++i)
    {
        d3d12aid_ComputeRsPs_Create(&rspsBitonicSort[i], device, GShaderBytecodesNoWaveIntrinsics[i].shaderBytecode, GShaderBytecodesNoWaveIntrinsics[i].shaderBytecodeSizeInBytes);
    }

    for (uint32_t i = 0; i < _countof(GShaderBytecodes); ++i)
    {
        d3d12aid_ComputeRsPs_Create(&rspsBitonicSort[i + _countof(GShaderBytecodesNoWaveIntrinsics)], device, GShaderBytecodes[i].shaderBytecode, GShaderBytecodes[i].shaderBytecodeSizeInBytes);
    }

    debugPrintF("Created D3D12 Resources and PSOs ...\n");

    perfData_PrintHeader();

    const uint32_t kFrameCount = kBenchmarkFrameCount * kShaderCount + kCmdBufferInFlight;

    uint32_t kCmdBufferIndex = 0;
    for (uint32_t frameIndex = 0; frameIndex < kFrameCount; ++frameIndex)
    {
        // run each Shader/Kernel every 'kBenchmarkFrameCount' frames
        const uint32_t dispatchShaderId = (frameIndex / kBenchmarkFrameCount) % kShaderCount;
        const ShaderBytecode *dispatchShaderBytecode = NULL;

        if (dispatchShaderId >= _countof(GShaderBytecodesNoWaveIntrinsics))
            dispatchShaderBytecode = &GShaderBytecodes[dispatchShaderId - _countof(GShaderBytecodesNoWaveIntrinsics)];
        else
            dispatchShaderBytecode = &GShaderBytecodesNoWaveIntrinsics[dispatchShaderId];

        const uint32_t dispatchKernelSize = 1u << dispatchShaderBytecode->kernelSizeLog2;
        const uint32_t dispatchTGroupSize = 1u << dispatchShaderBytecode->tgroupSizeLog2;

        // use only frame_0 of each benchmark
#if USE_PIX
        if ((sharedData->options & 0x1) && (frameIndex % kBenchmarkFrameCount) == 0)
        {
            char buffer[256];
            wchar_t wbuffer[256];
            stbsp_snprintf(buffer, _countof(buffer), "%c:\\tg_bitonic_kernel_%u_tgroup_%u_waveintr_%u_%s.wpix", sharedData->captureDrive, dispatchKernelSize, dispatchTGroupSize, dispatchShaderId > _countof(GShaderBytecodesNoWaveIntrinsics) ? 1 : 0, deviceName);
            if (0 != MultiByteToWideChar(CP_UTF8, 0, buffer, -1, wbuffer, _countof(wbuffer)))
            {
                PIXCaptureParameters params;
                params.GpuCaptureParameters.FileName = wbuffer;
                PIXBeginCapture(PIX_CAPTURE_GPU, &params);
            }
        }
#else
        (void)sharedData;
#endif

        ID3D12GraphicsCommandList *cmdList = d3d12aid_CmdQueue_StartCmdList(&queue, 0);

        if (frameIndex == 0)
        {
            d3d12aid_MappedBuffer_Transfer(cmdList, &sortInput, 0);

            D3D12_RESOURCE_BARRIER barrier;
            d3d12aid_MappedBuffer_EndTransfer(&barrier, &sortInput, D3D12_RESOURCE_STATE_GENERIC_READ);
            cmdList->ResourceBarrier(1, &barrier);
        }

        d3d12aid_ComputeRsPs_Set(&rspsBitonicSort[dispatchShaderId], cmdList);
        cmdList->SetComputeRootShaderResourceView(0, sortInput.bufGpu->GetGPUVirtualAddress());
        cmdList->SetComputeRootUnorderedAccessView(1, sortOutput.bufGpu->GetGPUVirtualAddress());
        cmdList->SetComputeRoot32BitConstant(2, options1.TotalLaneCount, 0);

        const uint32_t dispatchKernelCount = (kSortKeysPerDispatch + dispatchKernelSize - 1) >> dispatchShaderBytecode->kernelSizeLog2;
        const uint32_t dispatchKernelCountY = dispatchKernelCount / options1.TotalLaneCount;
        const uint32_t dispatchKernelCountX = options1.TotalLaneCount;

        d3d12aid_Timestamps_Push(&timestamps, cmdList);
        cmdList->Dispatch(dispatchKernelCountX, dispatchKernelCountY, 1);
        d3d12aid_Timestamps_Push(&timestamps, cmdList);

        {
            D3D12_RESOURCE_BARRIER barrier;
            d3d12aid_MappedBuffer_BeginTransfer(&barrier, &sortOutput, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
            cmdList->ResourceBarrier(1, &barrier);
        }
        d3d12aid_MappedBuffer_Transfer(cmdList, &sortOutput, kCmdBufferIndex);

        if (frameIndex >= kCmdBufferInFlight)
        {
            const uint32_t readbackFrameIndex = frameIndex - kCmdBufferInFlight;
            const uint64_t gpuTimestampDelta = d3d12aid_Timestamps_GetDelta(&timestamps, kCmdBufferIndex, 0, 1);

            const uint32_t benchmarkFrameIndex = readbackFrameIndex % kBenchmarkFrameCount;

            uint32_t readbackShaderId = (readbackFrameIndex / kBenchmarkFrameCount) % kShaderCount;

            const ShaderBytecode *readbackShaderBytecode = NULL;

            if (readbackShaderId >= _countof(GShaderBytecodesNoWaveIntrinsics))
                readbackShaderBytecode = &GShaderBytecodes[readbackShaderId - _countof(GShaderBytecodesNoWaveIntrinsics)];
            else
                readbackShaderBytecode = &GShaderBytecodesNoWaveIntrinsics[readbackShaderId];

            const uint32_t readbackKernelSize = 1u << readbackShaderBytecode->kernelSizeLog2;
            const uint32_t readbackTGroupSize = 1u << readbackShaderBytecode->tgroupSizeLog2;

            /** if it's a first benchmark run for currently selected shader variant, initialise 'perfData' */
            if (benchmarkFrameIndex == 0)
            {
                perfData_Init(&perfData, gpuTimestampFreq);
            }

            perfData_AddSample(&perfData, gpuTimestampDelta);

            /** if it's a the last benchmark run for currently selected shader variant, dump 'perfData' information */
            if (benchmarkFrameIndex == kBenchmarkFrameCount - 1u)
            {
                //const uint32_t readbackKernelCount = (kSortKeysPerDispatch + readbackKernelSize - 1) >> readbackShaderBytecode->kernelSizeLog2;
                debugPrintF("[KernelSize=%4u, TGroupSize=%4u, NoWaveIntrinsics=%1u] ", readbackKernelSize, readbackTGroupSize, readbackShaderId > _countof(GShaderBytecodesNoWaveIntrinsics) ? 0 : 1);
                // Per Lane
                //perfData_Print(&perfData, readbackKernelCount << GShaderBytecodes[readbackShaderId].tgroupSizeLog2);

                // Per TG
                //perfData_Print(&perfData, readbackKernelCount);

                // Per Elem
                perfData_Print(&perfData, kSortKeysPerDispatch);
            }

            const uint32_t *readbackData = (const uint32_t *)sortOutput.bufMem[kCmdBufferIndex];
            for (uint32_t i = 0; i < kSortKeysPerDispatch; i += readbackKernelSize)
            {
                for (uint32_t j = i; j < i + readbackKernelSize - 1; ++j)
                {
                    D3D12AID_ASSERT(readbackData[j] >= readbackData[j + 1]);
                }
            }
        }

        d3d12aid_Timestamps_AdvanceFrame(&timestamps, cmdList);
        d3d12aid_CmdQueue_SubmitCmdList(&queue, 0);

#if USE_PIX
        if ((sharedData->options & 0x1) && (frameIndex % kBenchmarkFrameCount) == 0)
        {
            PIXEndCapture(FALSE);
        }
#endif

        kCmdBufferIndex = (kCmdBufferIndex + 1) % kCmdBufferInFlight;
    }

    d3d12aid_CmdQueue_CpuWaitForGpuIdle(&queue);

    for (uint32_t i = 0; i < kShaderCount; ++i)
    {
        d3d12aid_ComputeRsPs_Release(&rspsBitonicSort[i]);
    }

    d3d12aid_MappedBuffer_Release(&sortOutput);
    d3d12aid_MappedBuffer_Release(&sortInput);
    d3d12aid_Timestamps_Release(&timestamps);

    d3d12aid_CmdQueue_Release(&queue);
    debugPrintF("Destroyed D3D12 Objects ...\n");
}
