// Copyright (C) 2023  absurdworlds
//
// See LICENSE in the root of this repository.
#ifndef LUABIND_FOLD_HPP_INCLUDED
#define LUABIND_FOLD_HPP_INCLUDED
// standalone version of fold from awlib (
// https://github.com/absurdworlds/awlib/blob/master/meta/include/aw/meta/fold.h )
namespace luabind {
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
using fold = typename meta::fold<Op, Init, Ts...>::type;

template<
	// Operation to apply to each pair
	template<typename A, typename B> typename Op,
	// Initial value
	typename Init,
	// List of types
	typename...Ts>
using fold_apply = typename meta::fold<Op, Init, Ts...>::type;

template<
	template<typename A, typename B> typename Op,
	typename Init,
	typename...Ts>
constexpr inline auto fold_v = fold<Op, Init, Ts...>::value;

} // namespace luabind
#endif//LUABIND_FOLD_HPP_INCLUDED
