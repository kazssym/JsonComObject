// ParserTest.cpp - parser test module
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

#include <JsonComObject_i.h>
#include <Windows.h>
#include <atlcomcli.h>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <strsafe.h>
#include <wil/result_macros.h>


namespace JsonComObject
{

namespace
{

struct ParserTestFixture
{
    ParserTestFixture() noexcept = default;
    ~ParserTestFixture() noexcept = default;
};

} // namespace


using namespace ::ATL;


BOOST_FIXTURE_TEST_CASE(ParserTest, ParserTestFixture)
{
    auto parser = CComPtr<IJsonParser>();
    BOOST_TEST(SUCCEEDED(parser.CoCreateInstance(CLSID_JsonParser, nullptr)));
    BOOST_TEST_REQUIRE(parser != nullptr);

    BOOST_TEST(parser->ParseJson(CComBSTR(), nullptr) == E_INVALIDARG);

    auto var1 = CComVariant();
    BOOST_TEST(parser->ParseJson(CComBSTR(), &var1) == E_INVALIDARG);

    auto var_null = CComVariant();
    BOOST_TEST(SUCCEEDED(parser->ParseJson(CComBSTR(OLESTR("null")), &var_null)));
    BOOST_TEST(V_VT(&var_null) == VT_NULL);

    auto var_false = CComVariant();
    BOOST_TEST(SUCCEEDED(parser->ParseJson(CComBSTR(OLESTR("false")), &var_false)));
    BOOST_TEST(V_VT(&var_false) == VT_BOOL);
    BOOST_TEST(V_BOOL(&var_false) == VARIANT_FALSE);

    auto var_true = CComVariant();
    BOOST_TEST(SUCCEEDED(parser->ParseJson(CComBSTR(OLESTR("true")), &var_true)));
    BOOST_TEST(V_VT(&var_true) == VT_BOOL);
    BOOST_TEST(V_BOOL(&var_true) == VARIANT_TRUE);
}

} // namespace JsonComObject
