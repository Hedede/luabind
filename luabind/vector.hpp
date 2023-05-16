// Copyright Daniel Wallin 2010. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUABIND_VECTOR_100629_HPP
# define LUABIND_VECTOR_100629_HPP

#include <luabind/mpl.h>

namespace luabind {

// This is used to specify function signatures, both in the user interface and
// in the implementation. In the C++03 version this used to be any MPL forward
// sequence. For backward compatibility reasons we provide a conversion layer
// from an MPL sequence.
template <class... Args>
struct vector
{};

namespace detail
{


  template <class... Args>
  struct vector_iterator;

  template <class Head, class... Tail>
  struct vector_iterator<Head, Tail...>
  {
      typedef Head type;
      typedef vector_iterator<Tail...> next;
  };

  template <>
  struct vector_iterator<>
  {};

  struct append_vector
  {
      template <class V, class T>
      struct apply;

      template <class... Args, class T>
      struct apply<vector<Args...>, T>
      {
          typedef vector<Args..., T> type;
      };
  };

  template <class Sequence>
  struct as_vector : fold<Sequence, vector<>, append_vector>
  {};

  template <class... Args>
  struct as_vector<vector<Args...> >
  {
      typedef vector<Args...> type;
  };


}} // namespace luabind::detail

#endif // LUABIND_VECTOR_100629_HPP
