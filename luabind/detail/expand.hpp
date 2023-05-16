// Copyright (C) 2016  absurdworlds
//
// See LICENSE in the root of this repository.
#ifndef LUABIND_EXPAND_HPP_INCLUDED
#define LUABIND_EXPAND_HPP_INCLUDED
// standalone version of fold from awlib (
// https://github.com/absurdworlds/awlib/blob/master/meta/include/aw/meta/expand.h )
namespace luabind {
struct _1 {};
struct _2 {};
struct _3 {};

namespace detail {
template<typename F, typename...Args>
struct fill_placeholders {
	using type = F;
};

template<typename T, typename...Args>
struct fill_placeholders<_1, T, Args...>
{
	using type = T;
};

template<typename T, typename U, typename...Args>
struct fill_placeholders<_2, T, U, Args...>
{
	using type = U;
};

template<typename T, typename U, typename V, typename...Args>
struct fill_placeholders<_3, T, U, V, Args...>
{
	using type = V;
};

template<typename F, typename...Args>
struct expand {
	using type = F;
};
template<template <typename...> class F, typename...Ts, typename...Args>
struct expand<F<Ts...>, Args...> {
	using type = F<typename fill_placeholders<Ts, Args...>::type...>;
};

template<typename F, typename...Args>
struct expand_r {
	using type = typename fill_placeholders<F, Args...>::type;
};

template<template <typename...> class F, typename...Ts, typename...Args>
struct expand_r<F<Ts...>, Args...> {
	using type = F<typename expand_r<Ts,Args...>::type...>;
};

template<typename Pred>
struct find_type {
	template<typename T>
	using F = typename expand<Pred, T>::type;

	template <typename...Ts>
	struct result {
	};

	template <typename T, typename...Ts>
	struct result<T,Ts...> {
		using type = std::conditional_t<F<T>::value, T, typename result<Ts...>::type>;
	};
};
} // namespace detail
//-----------------------------------------------

/*!
 * Expand placeholders in metafunction.
 * For example:
 *     expand< is_same<_1, int>, int >
 * results in
 *     is_same<int, int>
 */
template<typename F, typename...Args>
using expand = typename detail::expand<F,Args...>::type;

/*!
 * Expand placeholders in metafunction recursively.
 */
template<typename F, typename...Args>
using expand_r = typename detail::expand_r<F,Args...>::type;

template<typename F, typename...Args>
using apply = typename expand<F,Args...>::type;

template<typename Pred, typename...Ts>
using find_if = typename detail::find_type<Pred>::template result<Ts...>::type;
} // namespace luabind
#endif//LUABIND_EXPAND_HPP_INCLUDED
