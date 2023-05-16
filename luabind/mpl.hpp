#ifndef LUABIND_MPL_HPP_INCLUDED
#define LUABIND_MPL_HPP_INCLUDED
#include <type_traits>
// standalone version of meta from awlib
// https://github.com/absurdworlds/awlib/blob/master/meta/include/aw/meta/expand.h
// https://github.com/absurdworlds/awlib/blob/master/meta/include/aw/meta/fold.h
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
	using F = typename expand_r<Pred, T>::type;

	template <typename...Ts>
	struct result {
	};

	template <typename T>
	struct result<T> {
		using type = T;
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

namespace detail {

template<typename Pred, typename Vector>
struct find_if;

template<typename Pred, template<typename...> class Vector, typename...Ts>
struct find_if<Pred,Vector<Ts...>> : detail::find_type<Pred>::template result<Ts...>
{
};

} // namespace detail

template<typename Pred, typename Vector>
using find_if = typename detail::find_if<Pred, Vector>::type;


namespace detail {
template<
	template<typename A, typename B> typename Op,
	typename Init,
	typename T,
	typename...Ts>
struct fold {
	using type = typename fold<Op, Op<Init, T>, Ts...>::type;
};

template<
	template<typename A, typename B> typename Op,
	typename Init,
	typename T>
struct fold<Op, Init, T> {
	using type = Op<Init, T>;
};

template<
	template<typename A, typename B> typename Op,
	typename Init,
	typename T,
	typename...Ts>
struct fold_apply {
	using type = typename fold<Op, typename Op<Init, T>::type, Ts...>::type;
};

template<
	template<typename A, typename B> typename Op,
	typename Init,
	typename T>
struct fold_apply<Op, Init, T> {
	using type = typename Op<Init, T>::type;
};

} // namespace detail

/*!
 * For each of Ts, applies the binary Opto the result of the
 * previous application (or Init for the first application)
 * and the T.
 *
 * For example:
 *
 * given
 *
 *     template<typename L, typename R>>
 *     using op_t = typename op<L,R>::type;
 *
 * the
 *
 *     fold<op_t, None, A, B, C, D>
 *
 * is the same as
 *
 *     op_t<op_t<op_t<op_t<None, A>, B>, C>, D>
 *
 * Note that the op_t<> here is a type alias that returns a concrete type,
 * not a wrapper with ::type in it. If you don't want to declare a type alias,
 * use the fold_apply<>.
 */
template<
	// Operation to apply to each pair
	template<typename A, typename B> typename Op,
	// Initial value
	typename Init,
	// List of types
	typename...Ts>
using fold = typename detail::fold<Op, Init, Ts...>::type;

template<
	// Operation to apply to each pair
	template<typename A, typename B> typename Op,
	// Initial value
	typename Init,
	// List of types
	typename...Ts>
using fold_apply = typename detail::fold<Op, Init, Ts...>::type;

template<
	template<typename A, typename B> typename Op,
	typename Init,
	typename...Ts>
constexpr inline auto fold_v = fold<Op, Init, Ts...>::value;

template<typename T>
struct identity { typedef T type; };

template< typename C , typename F1 , typename F2 >
struct eval_if : std::conditional_t<C::value,F1,F2>
{
};

template< typename C , typename F1 , typename F2 >
using eval_if_t = typename eval_if<C,F1,F2>::type;

} // namespace luabind
#endif//LUABIND_MPL_HPP_INCLUDED
