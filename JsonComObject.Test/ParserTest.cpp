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
#include <atlconv.h>
#include <atldef.h>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <string_view>
#include <strsafe.h>
#include <wil/result_macros.h>

using std::string_view;
using namespace ATL;
using namespace std::literals;


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


BOOST_FIXTURE_TEST_CASE(ParserInterfaceTest, ParserTestFixture)
{
    auto parser = CComPtr<IJsonParser>();
    BOOST_TEST_REQUIRE(!FAILED(parser.CoCreateInstance(CLSID_JsonParser, nullptr)));
    ATLASSERT(parser != nullptr);

    auto support_error_info = CComPtr<ISupportErrorInfo>();
    BOOST_TEST(!FAILED(parser.QueryInterface(&support_error_info)));
    ATLASSERT(support_error_info != nullptr);

    auto agile_object = CComPtr<IAgileObject>();
    BOOST_TEST(!FAILED(parser.QueryInterface(&agile_object)));
    ATLASSERT(agile_object != nullptr);
}

BOOST_FIXTURE_TEST_CASE(ParseJsonTest, ParserTestFixture)
{
    auto parser = CComPtr<IJsonParser>();
    BOOST_TEST_REQUIRE(!FAILED(parser.CoCreateInstance(CLSID_JsonParser, nullptr)));
    ATLASSERT(parser != nullptr);

    BOOST_TEST(parser->ParseJson(CComBSTR(), nullptr) == E_INVALIDARG);

    auto var1 = CComVariant();
    BOOST_TEST(parser->ParseJson(CComBSTR(), &var1) == E_INVALIDARG);

    auto var_null = CComVariant();
    BOOST_TEST_REQUIRE(!FAILED(parser->ParseJson(CComBSTR(OLESTR("null")), &var_null)));
    BOOST_TEST(V_VT(&var_null) == VT_NULL);

    auto var_false = CComVariant();
    BOOST_TEST_REQUIRE(!FAILED(parser->ParseJson(CComBSTR(OLESTR("false")), &var_false)));
    BOOST_TEST(V_VT(&var_false) == VT_BOOL);
    BOOST_TEST(V_BOOL(&var_false) == VARIANT_FALSE);

    auto var_true = CComVariant();
    BOOST_TEST_REQUIRE(!FAILED(parser->ParseJson(CComBSTR(OLESTR("true")), &var_true)));
    BOOST_TEST(V_VT(&var_true) == VT_BOOL);
    BOOST_TEST(V_BOOL(&var_true) == VARIANT_TRUE);
}

BOOST_FIXTURE_TEST_CASE(ParseJsonStringTest, ParserTestFixture)
{
    auto parser = CComPtr<IJsonParser>();
    BOOST_TEST_REQUIRE(!FAILED(parser.CoCreateInstance(CLSID_JsonParser, nullptr)));
    ATLASSERT(parser != nullptr);

    auto var_string1 = CComVariant();
    BOOST_TEST_REQUIRE(!FAILED(parser->ParseJson(CComBSTR(OLESTR(R"""("hello")""")), &var_string1)));
    BOOST_TEST(V_VT(&var_string1) == VT_BSTR);
    BOOST_TEST(string_view(CW2A(V_BSTR(&var_string1))) == "hello"sv);
}

BOOST_FIXTURE_TEST_CASE(ParseJsonNumberTest, ParserTestFixture)
{
    auto parser = CComPtr<IJsonParser>();
    BOOST_TEST_REQUIRE(!FAILED(parser.CoCreateInstance(CLSID_JsonParser, nullptr)));
    ATLASSERT(parser != nullptr);

    auto var_number1 = CComVariant();
    BOOST_TEST_REQUIRE(!FAILED(parser->ParseJson(CComBSTR(OLESTR("-123")), &var_number1)));
    BOOST_TEST(V_VT(&var_number1) == VT_I8);
    BOOST_TEST(V_I8(&var_number1) == -123);

    auto var_number2 = CComVariant();
    BOOST_TEST_REQUIRE(!FAILED(parser->ParseJson(CComBSTR(OLESTR("123")), &var_number2)));
    BOOST_TEST(V_VT(&var_number2) == VT_UI8);
    BOOST_TEST(V_UI8(&var_number2) == 123);

    auto var_number3 = CComVariant();
    BOOST_TEST_REQUIRE(!FAILED(parser->ParseJson(CComBSTR(OLESTR("123.5")), &var_number3)));
    BOOST_TEST(V_VT(&var_number3) == VT_R8);
    BOOST_TEST(V_R8(&var_number3) == 123.5);
}

BOOST_FIXTURE_TEST_CASE(ParseJsonArrayTest, ParserTestFixture)
{
    auto parser = CComPtr<IJsonParser>();
    BOOST_TEST_REQUIRE(!FAILED(parser.CoCreateInstance(CLSID_JsonParser, nullptr)));
    ATLASSERT(parser != nullptr);

    auto var_array1 = CComVariant();
    BOOST_TEST_REQUIRE(!FAILED(parser->ParseJson(CComBSTR(OLESTR(R"([1, 2, 3])")), &var_array1)));
    BOOST_TEST_REQUIRE(V_VT(&var_array1) == VT_DISPATCH);

    CComPtr<IJsonArray> json_array1;
    BOOST_TEST_REQUIRE(!FAILED(V_DISPATCH(&var_array1)->QueryInterface(&json_array1)));

    auto count1 = LONG();
    BOOST_TEST(!FAILED(json_array1->get_Count(&count1)));
    BOOST_TEST(count1 == 3);

    auto item1 = CComVariant();
    BOOST_TEST(!FAILED(json_array1->get_Item(0, &item1)));
    THROW_IF_FAILED(item1.ChangeType(VT_I8));
    BOOST_TEST(V_VT(&item1) == VT_I8);
    BOOST_TEST(V_I8(&item1) == 1);

    auto var_array2 = CComVariant();
    BOOST_TEST_REQUIRE(!FAILED(parser->ParseJson(CComBSTR(OLESTR(R"([[], []])")), &var_array2)));
    BOOST_TEST_REQUIRE(V_VT(&var_array2) == VT_DISPATCH);

    auto var_array3 = CComVariant();
    BOOST_TEST(parser->ParseJson(CComBSTR(OLESTR(R"([)")), &var_array3) == DISP_E_EXCEPTION);

    auto var_array4 = CComVariant();
    BOOST_TEST(parser->ParseJson(CComBSTR(OLESTR(R"(])")), &var_array4) == DISP_E_EXCEPTION);
}

} // namespace JsonComObject
