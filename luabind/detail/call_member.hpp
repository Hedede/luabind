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

#ifndef LUABIND_CALL_MEMBER_HPP_INCLUDED
#define LUABIND_CALL_MEMBER_HPP_INCLUDED

#include <luabind/config.hpp>
#include <luabind/detail/convert_to_lua.hpp>
#include <luabind/detail/pcall.hpp>
#include <luabind/error.hpp>
#include <luabind/detail/stack_utils.hpp>
#include <luabind/object.hpp> // TODO: REMOVE DEPENDENCY

#include <tuple>

namespace luabind
{
	namespace detail
	{
		// if the proxy_member_caller returns non-void
			template<class Ret, class Tuple>
			class proxy_member_caller
			{
//			friend class luabind::object;
			public:

				proxy_member_caller(lua_State* L_, const Tuple args)
					: L(L_)
					, m_args(args)
					, m_called(false)
				{
				}

				proxy_member_caller(const proxy_member_caller& rhs)
					: L(rhs.L)
					, m_args(rhs.m_args)
					, m_called(rhs.m_called)
				{
					rhs.m_called = true;
				}

				~proxy_member_caller() noexcept(false)
				{
					if (m_called) return;

					m_called = true;

					// don't count the function and self-reference
					// since those will be popped by pcall
					int top = lua_gettop(L) - 2;

					// pcall will pop the function and self reference
					// and all the parameters

					push_args_from_tuple<1>::apply(L, m_args);
					if (pcall(L, std::tuple_size<Tuple>::value + 1, 0))
					{
						assert(lua_gettop(L) == top + 1);
#ifndef LUABIND_NO_EXCEPTIONS
						throw luabind::error(L);
#else
						error_callback_fun e = get_error_callback();
						if (e) e(L);
	
						assert(0 && "the lua function threw an error and exceptions are disabled."
								"If you want to handle this error use luabind::set_error_callback()");
						std::terminate();
#endif
					}
					// pops the return values from the function
					stack_pop pop(L, lua_gettop(L) - top);
				}

				operator Ret()
				{
					typename apply_wrap2<default_policy,Ret,lua_to_cpp>::type converter;

					m_called = true;

					// don't count the function and self-reference
					// since those will be popped by pcall
					int top = lua_gettop(L) - 2;

					// pcall will pop the function and self reference
					// and all the parameters
					push_args_from_tuple<1>::apply(L, m_args);
					if (pcall(L, std::tuple_size<Tuple>::value + 1, 1))
					{
						assert(lua_gettop(L) == top + 1);
#ifndef LUABIND_NO_EXCEPTIONS
						throw luabind::error(L); 
#else
						error_callback_fun e = get_error_callback();
						if (e) e(L);
	
						assert(0 && "the lua function threw an error and exceptions are disabled."
							"If you want to handle this error use luabind::set_error_callback()");
						std::terminate();
#endif
					}

					// pops the return values from the function
					stack_pop pop(L, lua_gettop(L) - top);

					if (converter.match(L, LUABIND_DECORATE_TYPE(Ret), -1) < 0)
					{
						assert(lua_gettop(L) == top + 1);
#ifndef LUABIND_NO_EXCEPTIONS
						throw cast_failed(L, typeid(Ret));
#else
						cast_failed_callback_fun e = get_cast_failed_callback();
						if (e) e(L, typeid(Ret));

						assert(0 && "the lua function's return value could not be converted."
							"If you want to handle this error use luabind::set_error_callback()");
						std::terminate();
#endif
					}

					return converter.apply(L, LUABIND_DECORATE_TYPE(Ret), -1);
				}

				template<class Policies>
				Ret operator[](const Policies& p)
				{
					typedef typename find_conversion_policy<0, Policies>::type converter_policy;
					typename apply_wrap2<converter_policy,Ret,lua_to_cpp>::type converter;

					m_called = true;

					// don't count the function and self-reference
					// since those will be popped by pcall
					int top = lua_gettop(L) - 2;

					// pcall will pop the function and self reference
					// and all the parameters

					detail::push_args_from_tuple<1>::apply(L, m_args, p);
					if (pcall(L, std::tuple_size<Tuple>::value + 1, 1))
					{
						assert(lua_gettop(L) == top + 1);
#ifndef LUABIND_NO_EXCEPTIONS
						throw error(L);
#else
						error_callback_fun e = get_error_callback();
						if (e) e(L);
	
						assert(0 && "the lua function threw an error and exceptions are disabled."
							"If you want to handle this error use luabind::set_error_callback()");
						std::terminate();
#endif
					}

					// pops the return values from the function
					stack_pop pop(L, lua_gettop(L) - top);

					if (converter.match(L, LUABIND_DECORATE_TYPE(Ret), -1) < 0)
					{
						assert(lua_gettop(L) == top + 1);
#ifndef LUABIND_NO_EXCEPTIONS
						throw cast_failed(L, typeid(Ret));
#else
						cast_failed_callback_fun e = get_cast_failed_callback();
						if (e) e(L, typeid(Ret));

						assert(0 && "the lua function's return value could not be converted."
							"If you want to handle this error use luabind::set_error_callback()");
						std::terminate();
#endif
					}

					return converter.apply(L, LUABIND_DECORATE_TYPE(Ret), -1);
				}

			private:

				lua_State* L;
				Tuple m_args;
				mutable bool m_called;

			};

		// if the proxy_member_caller returns void
			template<class Tuple>
			class proxy_member_void_caller
			{
			friend class luabind::object;
			public:

				proxy_member_void_caller(lua_State* L_, const Tuple args)
					: L(L_)
					, m_args(args)
					, m_called(false)
				{
				}

				proxy_member_void_caller(const proxy_member_void_caller& rhs)
					: L(rhs.L)
					, m_args(rhs.m_args)
					, m_called(rhs.m_called)
				{
					rhs.m_called = true;
				}

				~proxy_member_void_caller() noexcept(false)
				{
					if (m_called) return;

					m_called = true;

					// don't count the function and self-reference
					// since those will be popped by pcall
					int top = lua_gettop(L) - 2;

					// pcall will pop the function and self reference
					// and all the parameters

					push_args_from_tuple<1>::apply(L, m_args);
					if (pcall(L, std::tuple_size<Tuple>::value + 1, 0))
					{
						assert(lua_gettop(L) == top + 1);
#ifndef LUABIND_NO_EXCEPTIONS
						throw luabind::error(L);
#else
						error_callback_fun e = get_error_callback();
						if (e) e(L);
	
						assert(0 && "the lua function threw an error and exceptions are disabled."
							"If you want to handle this error use luabind::set_error_callback()");
						std::terminate();
#endif
					}
					// pops the return values from the function
					stack_pop pop(L, lua_gettop(L) - top);
				}

				template<class Policies>
				void operator[](const Policies& p)
				{
					m_called = true;

					// don't count the function and self-reference
					// since those will be popped by pcall
					int top = lua_gettop(L) - 2;

					// pcall will pop the function and self reference
					// and all the parameters

					detail::push_args_from_tuple<1>::apply(L, m_args, p);
					if (pcall(L, std::tuple_size<Tuple>::value + 1, 0))
					{
						assert(lua_gettop(L) == top + 1);
#ifndef LUABIND_NO_EXCEPTIONS
						throw error(L);
#else
						error_callback_fun e = get_error_callback();
						if (e) e(L);
	
						assert(0 && "the lua function threw an error and exceptions are disabled."
							"If you want to handle this error use luabind::set_error_callback()");
						std::terminate();
#endif
					}
					// pops the return values from the function
					stack_pop pop(L, lua_gettop(L) - top);
				}

			private:
				lua_State* L;
				Tuple m_args;
				mutable bool m_called;

			};

	} // detail

namespace detail
{

  template <class R, class... Args>
  struct make_member_proxy
  {
      typedef proxy_member_caller<R, std::tuple<Args const*...> > type;
  };

  template <class... Args>
  struct make_member_proxy<void, Args...>
  {
      typedef proxy_member_void_caller<std::tuple<Args const*...> > type;
  };

} // namespace detail

template <class R, class... Args>
typename detail::make_member_proxy<R, Args...>::type call_member(
    object const& obj, char const* name, Args const&... args)
{
    // get the function
    obj.push(obj.interpreter());
    lua_pushstring(obj.interpreter(), name);
    lua_gettable(obj.interpreter(), -2);
    // duplicate the self-object
    lua_pushvalue(obj.interpreter(), -2);
    // remove the bottom self-object
    lua_remove(obj.interpreter(), -3);

    typedef typename detail::make_member_proxy<R, Args...>::type proxy_type;
    return proxy_type(obj.interpreter(), std::tuple<Args const*...>(&args...));
}

}

#endif // LUABIND_CALL_MEMBER_HPP_INCLUDED
