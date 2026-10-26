// JsonComObject-Parser.cpp -
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
#include <Windows.h>
#include <atlbase.h>
#include <atlcom.h>
#include <atldef.h>


export module JsonComObject:Parser;

namespace JsonComObject
{

export class ATL_NO_VTABLE CJsonParser
    : public ATL::CComObjectRootEx<ATL::CComMultiThreadModel>,
      public ATL::CComCoClass<CJsonParser, &CLSID_JsonParser>,
      public ATL::IDispatchImpl<IJsonParser, &IID_IJsonParser, &LIBID_JsonComObjectLib, 1, 0>,
      public ATL::ISupportErrorInfoImpl<&IID_IJsonParser>
{
    using Base = ATL::CComObjectRootEx<ATL::CComMultiThreadModel>;

    BEGIN_COM_MAP(CJsonParser)
        COM_INTERFACE_ENTRY(IJsonParser)
        COM_INTERFACE_ENTRY(IDispatch)
        COM_INTERFACE_ENTRY(ISupportErrorInfo)
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()

public:
    CJsonParser() = default;

    virtual ~CJsonParser() noexcept = default;


    auto ParseJson(BSTR json, VARIANT *result) noexcept -> HRESULT override
    {
        return E_NOTIMPL;
    }
};

}  // namespace JsonComObject
