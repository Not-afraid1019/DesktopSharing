// Copyright (c) 2012-2019 Intel Corporation
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "mfx_dispatcher.h"
#include "mfx_load_dll.h"

#include <wchar.h>
#include <string.h>
#include <Windows.h>

#if !defined(_DEBUG)

#if defined(_WIN64)
const
wchar_t * const defaultDLLName[2] = {L"libmfxhw64.dll",
                                     L"libmfxsw64.dll"};
const
wchar_t * const defaultAudioDLLName[2] = {L"libmfxaudiosw64.dll",
                                          L"libmfxaudiosw64.dll"};

const 
wchar_t  * const defaultPluginDLLName[2] = {L"mfxplugin64_hw.dll",
                                            L"mfxplugin64_sw.dll"};
#elif defined(_WIN32)
const
wchar_t * const defaultDLLName[2] = {L"libmfxhw32.dll",
                                     L"libmfxsw32.dll"};

const
wchar_t * const defaultAudioDLLName[2] = {L"libmfxaudiosw32.dll",
                                          L"libmfxaudiosw32.dll"};

const 
wchar_t  * const defaultPluginDLLName[2] = {L"mfxplugin32_hw.dll",
                                            L"mfxplugin32_sw.dll"};

#endif // (defined(_WIN64))

#else // defined(_DEBUG)

#if defined(_WIN64)
const
wchar_t * const defaultDLLName[2] = {L"libmfxhw64_d.dll",
                                     L"libmfxsw64_d.dll"};
const
wchar_t * const defaultAudioDLLName[2] = {L"libmfxaudiosw64_d.dll",
                                          L"libmfxaudiosw64_d.dll"};

const 
wchar_t  * const defaultPluginDLLName[2] = {L"mfxplugin64_hw_d.dll",
                                            L"mfxplugin64_sw_d.dll"};

#elif defined(WIN32)
const
wchar_t * const defaultDLLName[2] = {L"libmfxhw32_d.dll",
                                     L"libmfxsw32_d.dll"};


const
wchar_t * const defaultAudioDLLName[2] = {L"libmfxaudiosw32_d.dll",
                                          L"libmfxaudiosw32_d.dll"};

const 
wchar_t  * const defaultPluginDLLName[2] = {L"mfxplugin32_hw_d.dll",
                                            L"mfxplugin32_sw_d.dll"};
#endif // (defined(_WIN64))

#endif // !defined(_DEBUG)

namespace MFX
{


mfxStatus mfx_get_default_dll_name(wchar_t *pPath, size_t pathSize, eMfxImplType implType)
{
    if (!pPath)
    {
        return MFX_ERR_NULL_PTR;
    }
    

    // there are only 2 implementation with default DLL names
#if _MSC_VER >= 1400
    return 0 == wcscpy_s(pPath, pathSize, defaultDLLName[implType & 1])
        ? MFX_ERR_NONE : MFX_ERR_UNKNOWN;
#else    
    wcscpy(pPath, defaultDLLName[implType & 1]);
    return MFX_ERR_NONE;
#endif
} // mfxStatus mfx_get_default_dll_name(wchar_t *pPath, size_t pathSize, eMfxImplType implType)

mfxStatus mfx_get_default_plugin_name(wchar_t *pPath, size_t pathSize, eMfxImplType implType)
{
    if (!pPath)
    {
        return MFX_ERR_NULL_PTR;
    }


    // there are only 2 implementation with default DLL names
#if _MSC_VER >= 1400
    return 0 == wcscpy_s(pPath, pathSize, defaultPluginDLLName[implType & 1])
        ? MFX_ERR_NONE : MFX_ERR_UNKNOWN;
#else    
    wcscpy(pPath, defaultPluginDLLName[implType & 1]);
    return MFX_ERR_NONE;
#endif
}

mfxStatus mfx_get_default_audio_dll_name(wchar_t *pPath, size_t pathSize, eMfxImplType implType)
{
    if (!pPath)
    {
        return MFX_ERR_NULL_PTR;
    }
    
    // there are only 2 implementation with default DLL names
#if _MSC_VER >= 1400
    return 0 == wcscpy_s(pPath, pathSize, defaultAudioDLLName[implType & 1])
        ? MFX_ERR_NONE : MFX_ERR_UNKNOWN;
#else    
    wcscpy(pPath, defaultAudioDLLName[implType & 1]);
    return MFX_ERR_NONE;
#endif
} // mfxStatus mfx_get_default_audio_dll_name(wchar_t *pPath, size_t pathSize, eMfxImplType implType)

mfxModuleHandle mfx_dll_load(const wchar_t *pFileName)
{
    mfxModuleHandle hModule = (mfxModuleHandle) 0;

    // check error(s)
    if (NULL == pFileName)
    {
        return NULL;
    }
    // set the silent error mode
    DWORD prevErrorMode = 0;
#if (_WIN32_WINNT >= 0x0600)
    SetThreadErrorMode(SEM_FAILCRITICALERRORS, &prevErrorMode);
#else
    prevErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS);
#endif

    // load the library's module
    hModule = LoadLibraryExW(pFileName, NULL, 0);

    // set the previous error mode
#if (_WIN32_WINNT >= 0x0600)
    SetThreadErrorMode(prevErrorMode, NULL);
#else
    SetErrorMode(prevErrorMode);
#endif

    return hModule;

} // mfxModuleHandle mfx_dll_load(const wchar_t *pFileName)

mfxFunctionPointer mfx_dll_get_addr(mfxModuleHandle handle, const char *pFunctionName)
{
    if (NULL == handle)
    {
        return NULL;
    }

    return (mfxFunctionPointer) GetProcAddress((HMODULE) handle, pFunctionName);
} // mfxFunctionPointer mfx_dll_get_addr(mfxModuleHandle handle, const char *pFunctionName)

bool mfx_dll_free(mfxModuleHandle handle)
{
    if (NULL == handle)
    {
        return true;
    }

    BOOL bRes = FreeLibrary((HMODULE)handle);

    return !!bRes;
} // bool mfx_dll_free(mfxModuleHandle handle)

mfxModuleHandle mfx_get_dll_handle(const wchar_t *pFileName)
{
    mfxModuleHandle hModule = (mfxModuleHandle) 0;

    // check error(s)
    if (NULL == pFileName)
    {
        return NULL;
    }

    // set the silent error mode
    DWORD prevErrorMode = 0;
#if (_WIN32_WINNT >= 0x0600)
    SetThreadErrorMode(SEM_FAILCRITICALERRORS, &prevErrorMode); 
#else
    prevErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS);
#endif
    // load the library's module
    GetModuleHandleExW(0, pFileName, (HMODULE*) &hModule);
    // set the previous error mode
#if (_WIN32_WINNT >= 0x0600)
    SetThreadErrorMode(prevErrorMode, NULL);
#else
    SetErrorMode(prevErrorMode);
#endif
    return hModule;
}

} // namespace MFX

