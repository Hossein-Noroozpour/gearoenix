#ifndef GEAROENIX_DIRECTX11_CHECK_HPP
#define GEAROENIX_DIRECTX11_CHECK_HPP
#include "../core/gx-cr-build-configuration.hpp"
#ifdef GX_USE_DIRECTX11
#define GXHRCHK(x)                                          \
    {                                                       \
        HRESULT gearoenix_macro_hresult = (x);              \
        if (FAILED(gearoenix_macro_hresult)) {              \
            GXLOGF("HRESULT: " << gearoenix_macro_hresult); \
        }                                                   \
    }
#endif
#endif
