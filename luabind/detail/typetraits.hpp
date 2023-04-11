// Copyright (c) 2003 Daniel Wallin and Arvid Norberg

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


#ifndef LUABIND_TYPETRAITS_HPP_INCLUDED
#define LUABIND_TYPETRAITS_HPP_INCLUDED

#include <luabind/config.hpp>
#include <luabind/detail/primitives.hpp>
#include <type_traits>

namespace luabind { namespace detail 
{
	template<class T>
	struct is_const_reference
	{
		enum { value = false };
		using type = std::integral_constant<bool, value>;
	};

	template<class T>
	struct is_const_reference<const T&>
	{
		enum { value = true };
		using type = std::integral_constant<bool, value>;
	};


	template<class T>
	struct is_nonconst_reference
	{
		enum
		{
			value = std::is_reference<T>::value && !is_const_reference<T>::value
		};
		using type = std::integral_constant<bool, value>;
	};

	template<class A>
	yes_t is_const_pointer_helper(void(*)(const A*));
	no_t is_const_pointer_helper(...);

	template<class T>
	struct is_const_pointer
	{
		enum { value = sizeof(is_const_pointer_helper((void(*)(T))0)) == sizeof(yes_t) };
		using type = std::integral_constant<bool, value>;
	};

	template<class A>
	yes_t is_nonconst_pointer_helper(void(*)(A*));
	no_t is_nonconst_pointer_helper(...);

	template<class T>
	struct is_nonconst_pointer
	{
		enum { value = sizeof(is_nonconst_pointer_helper((void(*)(T))0)) == sizeof(yes_t) && !is_const_pointer<T>::value };
		using type = std::integral_constant<bool, value>;
	};

	template<int v1, int v2>
	struct max_c
	{
		enum { value = (v1>v2)?v1:v2 };
	};

}}

#endif // LUABIND_TYPETRAITS_HPP_INCLUDED

