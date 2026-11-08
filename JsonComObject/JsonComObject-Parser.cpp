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
#include "resource.h"
#include <Windows.h>
#include <algorithm>
#include <atlbase.h>
#include <atlcom.h>
#include <atlcomcli.h>
#include <atlconv.h>
#include <atldef.h>
#include <gsl/narrow>
#include <gsl/util>
#include <jsoncons/basic_json.hpp>
#include <jsoncons/json_cursor.hpp>
#include <jsoncons/json_exception.hpp>
#include <jsoncons/staj_event.hpp>
#include <jsoncons/utility/byte_string.hpp>
#include <string_view>
#include <strsafe.h>
#include <utility>
#include <wil/result_macros.h>

using ATL::CA2W;
using ATL::CComBSTR;
using ATL::CComPtr;
using ATL::CComVariant;
using jsoncons::byte_string_view;
using jsoncons::json_exception;
using jsoncons::staj_events;
using jsoncons::wjson;
using jsoncons::wjson_string_cursor;
using std::exception;
using std::move;
using std::wstring_view;


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
    DECLARE_OBJECT_DESCRIPTION("JSON Parser Object 1.0")
    DECLARE_REGISTRY_RESOURCEID(IDR_JSONPARSER)

  public:
    CJsonParser() = default;

    virtual ~CJsonParser() noexcept = default;

protected:
    auto ParseJsonValue(wjson_string_cursor &cursor) const -> CComVariant
    {
        if (cursor.done())
        {
            return {};
        }

        [[maybe_unused]] auto call_next_finally = gsl::finally([&cursor]() { cursor.next(); });

        using enum staj_events;
        switch (const auto &cur = cursor.current(); cur.event_type())
        {
        case string_value: {
            const auto sv = cur.get<wstring_view>();
            return CComVariant(CComBSTR(gsl::narrow<int>(sv.size()), sv.data()));
        }
        case byte_string_value: {
            const auto sv = cur.get<byte_string_view>();
            return CComVariant(CComBSTR(gsl::narrow<int>(sv.size()), reinterpret_cast<LPCSTR>(sv.data())));
        }
        case null_value:
            return CComVariant(VARIANT{VT_NULL});
        case bool_value:
            return CComVariant(cur.get<bool>());
        default:
            LOG_HR_MSG(E_NOTIMPL, "Not implemented yet.");
            return {};
        }
    }


    constexpr auto &GetRootJsonValue() const noexcept
    {
        return this->root_json_value;
    }

    void SetRootJsonValue(CComVariant json_value)
    {
        auto v = VARIANT{};
        THROW_IF_FAILED(json_value.Detach(&v));
        THROW_IF_FAILED(this->root_json_value.Attach(&v));
    }

private:
    CComVariant root_json_value;

public:
    auto ParseJson(const BSTR json_str, VARIANT *const result) noexcept -> HRESULT override
    {
        RETURN_HR_IF_NULL(E_INVALIDARG, result);
        RETURN_HR_IF_NULL(E_INVALIDARG, json_str);

        try
        {
            auto cursor = wjson_string_cursor(json_str);
            this->SetRootJsonValue(this->ParseJsonValue(cursor));

            ::VariantInit(result);
            RETURN_IF_FAILED(::VariantCopy(result, &this->GetRootJsonValue()));

            return S_OK;
        }
        catch (const json_exception &e)
        {
            auto cei = CComPtr<ICreateErrorInfo>();
            RETURN_IF_FAILED(::CreateErrorInfo(&cei));
            RETURN_IF_FAILED(cei->SetGUID(IID_IJsonParser));
            RETURN_IF_FAILED(cei->SetSource(CComBSTR(OLESTR("JsonComObject.JsonParser"))));
            RETURN_IF_FAILED(cei->SetDescription(CA2W(e.what())));
            
            auto ei = CComPtr<IErrorInfo>();
            RETURN_IF_FAILED(cei->QueryInterface(&ei));
            RETURN_IF_FAILED(::SetErrorInfo(0, ei));

            RETURN_HR(DISP_E_EXCEPTION);
        }
        catch (const gsl::narrowing_error &)
        {
            RETURN_HR(DISP_E_OVERFLOW);
        }
        catch (const exception &)
        {
            RETURN_HR(E_UNEXPECTED);
        }
    }
};

OBJECT_ENTRY_AUTO(CLSID_JsonParser, CJsonParser)

}  // namespace JsonComObject
