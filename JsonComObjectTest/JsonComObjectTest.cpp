// JsonComObjectTest.cpp - main test module
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

#define BOOST_TEST_MODULE JsonComObjectTest
#include <boost/test/unit_test.hpp>
#include <wil/result_macros.h>

extern "C"
{
#include <JsonComObject_i.c>  // For static GUID linkage
}

#pragma comment(linker, R"(/manifestdependency:"type='win32' name='JsonComObject' version='1.0.0.0'")")


namespace JsonComObject
{

struct GlobalFixture
{
    GlobalFixture() noexcept = default;
    ~GlobalFixture() noexcept = default;

    void setup() const
    {
        THROW_IF_FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE));
    }

    void teardown() const noexcept
    {
        CoUninitialize();
    }
};

BOOST_TEST_GLOBAL_FIXTURE(GlobalFixture);

}  // namespace JsonComObject
