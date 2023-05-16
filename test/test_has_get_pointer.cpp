// Copyright (c) 2005 Daniel Wallin

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.

#include <luabind/detail/has_get_pointer.hpp>

namespace lb = luabind::detail;

namespace test
{

  struct X
  {
  };

  struct Y
  {
  };

  Y* get_pointer(Y const&);
  
  struct Z : std::enable_shared_from_this<Z> {};
  
} // namespace test

static_assert( lb::has_get_pointer<std::shared_ptr<int> >::value );
static_assert( lb::has_get_pointer<test::Y>::value );
static_assert( lb::has_get_pointer<char*>::value );
static_assert(! lb::has_get_pointer<int>::value );
static_assert(! lb::has_get_pointer<test::X>::value );
static_assert( lb::has_get_pointer<test::Z*>::value );

#include "test.hpp"
void test_main(lua_State*)
{
}
