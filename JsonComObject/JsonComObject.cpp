// JsonComObject.cpp - main module interface 
// Copyright © 2026 Kaz Nishimura

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// SPDX-License-Identifier: MIT

module;

#include "pch.h"

#include "JsonComObject_i.h"
#include "resource.h"
#include <Windows.h>
#include <atlbase.h>
#include <sal.h>


export module JsonComObject;

export import :Parser;

namespace JsonComObject
{

class CJsonComObjectModule : public ATL::CAtlDllModuleT<CJsonComObjectModule>
{
public:
    DECLARE_LIBID(LIBID_JsonComObjectLib)
    DECLARE_REGISTRY_RESOURCEID(IDR_JSONCOMOBJECT)
};

export CJsonComObjectModule _AtlModule;  // NOSONAR

}  // namespace JsonComObject

using JsonComObject::_AtlModule;

_Check_return_
export STDAPI DllGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid, _Outptr_ LPVOID *const ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}

export STDAPI DllCanUnloadNow()
{
    return _AtlModule.DllCanUnloadNow();
}

export STDAPI DllRegisterServer()
{
    return _AtlModule.DllRegisterServer();
}

export STDAPI DllUnregisterServer()
{
    return _AtlModule.DllUnregisterServer();
}
