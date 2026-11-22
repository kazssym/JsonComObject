// JsonComObject-Array.cpp -
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
#include <algorithm>
#include <atlbase.h>
#include <atlcom.h>
#include <atlcomcli.h>
#include <atldef.h>
#include <gsl/narrow>
#include <mutex>
#include <strsafe.h>
#include <utility>
#include <vector>
#include <wil/result_macros.h>

using ATL::CComCoClass;
using ATL::CComMultiThreadModel;
using ATL::CComObjectRootEx;
using ATL::CComPtr;
using ATL::CComVariant;
using ATL::IDispatchImpl;
using ATL::IEnumOnSTLImpl;
using ATL::ISupportErrorInfoImpl;
using ATL::_Copy;
using std::move;
using std::mutex;
using std::scoped_lock;
using std::size_t;
using std::vector;


export module JsonComObject:Array;

namespace JsonComObject
{

export class ATL_NO_VTABLE CJsonArrayEnumerator
    : public CComObjectRootEx<CComMultiThreadModel>,
      public CComCoClass<CJsonArrayEnumerator>,
      public IEnumOnSTLImpl<IEnumVARIANT, &IID_IEnumVARIANT, VARIANT, _Copy<VARIANT>, vector<CComVariant>>,
      public IAgileObject
{
    using Base = CComObjectRootEx<CComMultiThreadModel>;
    BEGIN_COM_MAP(CJsonArrayEnumerator)
        COM_INTERFACE_ENTRY(IEnumVARIANT)
        COM_INTERFACE_ENTRY(IAgileObject)
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()
    DECLARE_OBJECT_DESCRIPTION("JSON Array Object 1.0")
    DECLARE_NO_REGISTRY()

public:
    CJsonArrayEnumerator() = default;

protected:
    virtual ~CJsonArrayEnumerator() noexcept = default;
};

export class ATL_NO_VTABLE CJsonArray
    : public CComObjectRootEx<CComMultiThreadModel>,
      public CComCoClass<CJsonArray>,
      public IDispatchImpl<IJsonArray, &IID_IJsonArray, &LIBID_JsonComObjectLib, 1, 0>,
      public ISupportErrorInfoImpl<&IID_IJsonArray>,
      public IAgileObject
{
    using Base = CComObjectRootEx<CComMultiThreadModel>;

    BEGIN_COM_MAP(CJsonArray)
        COM_INTERFACE_ENTRY(IJsonArray)
        COM_INTERFACE_ENTRY(IDispatch)
        COM_INTERFACE_ENTRY(ISupportErrorInfo)
        COM_INTERFACE_ENTRY(IAgileObject)
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()
    DECLARE_OBJECT_DESCRIPTION("JSON Array Object 1.0")
    DECLARE_NO_REGISTRY()

public:
    CJsonArray() = default;

protected:
    virtual ~CJsonArray() noexcept = default;

public:
    constexpr auto &GetObjectMutex() & noexcept
    {
        return this->object_mutex;
    }

private:
    mutex object_mutex;


public:
    constexpr auto &GetItems() const & noexcept
    {
        return this->items;
    }

    constexpr void SetItems(vector<CComVariant> new_items) noexcept
    {
        auto lock = scoped_lock(this->GetObjectMutex());
        this->items = move(new_items);
    }

protected:
    constexpr auto &GetMutableItems() & noexcept
    {
        return this->items;
    }

private:
    vector<CComVariant> items;


public:
    auto get_Item(const LONG pos, VARIANT *const result) -> HRESULT override
    {
        auto lock = scoped_lock(this->GetObjectMutex());
        RETURN_HR_IF(E_INVALIDARG, !(pos >= 0 && static_cast<size_t>(pos) < this->items.size()));
        RETURN_HR_IF_NULL(E_INVALIDARG, result);

        ::VariantInit(result);
        RETURN_IF_FAILED(::VariantCopy(result, &this->GetItems()[pos]));
        return S_OK;
    }

    auto get__NewEnum(IEnumVARIANT **const result) -> HRESULT override
    {
        auto lock = scoped_lock(this->GetObjectMutex());
        RETURN_HR_IF_NULL(E_INVALIDARG, result);
        
        auto enumerator = CComPtr<IEnumVARIANT>();
        RETURN_IF_FAILED(CJsonArrayEnumerator::CreateInstance(nullptr, &enumerator));

        const auto &com_enumerator = dynamic_cast<CJsonArrayEnumerator *>(static_cast<IEnumVARIANT *>(enumerator));
        RETURN_HR_IF_NULL(E_UNEXPECTED, com_enumerator);
        RETURN_IF_FAILED(com_enumerator->Init(this->GetUnknown(), this->GetMutableItems()));

        *result = enumerator.Detach();
        return S_OK;
    }

    auto get_Count(LONG *const result) -> HRESULT override
    {
        auto lock = scoped_lock(this->GetObjectMutex());
        RETURN_HR_IF_NULL(E_INVALIDARG, result);

        try
        {
            *result = gsl::narrow<LONG>(this->items.size());
            return S_OK;
        }
        catch (const gsl::narrowing_error &)
        {
            RETURN_HR(E_UNEXPECTED);
        }
    }
};

} // namespace JsonComObject
