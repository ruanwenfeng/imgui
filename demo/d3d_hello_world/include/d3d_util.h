#ifndef D3DUTIL_H
#define D3DUTIL_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <dxerr.h>
#include <cassert>

//-------------------------------
//·½±ã¼ì²âd3d errors
//-------------------------------
#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)                                              \
    {                                                          \
    HRESULT hr = (x);                                      \
if (FAILED(hr))                                         \
        {                                                      \
        DXTrace(__FILEW__, (DWORD)__LINE__, hr, L#x, true); \
        }                                                      \
    }
#endif

#else
#ifndef HR
#define HR(x) (x)
#endif
#endif 

//------------------------------
//·½±ãÉ¾³ýCOM objects
//------------------------------
#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }


#endif//D3DUTIL_H