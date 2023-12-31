// Copyright Daniel Wallin 2008. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUABIND_TYPEID_081227_HPP
# define LUABIND_TYPEID_081227_HPP

# include <typeinfo>
# include <luabind/detail/primitives.hpp>

namespace luabind {

class type_id
{
public:
    type_id()
      : id(&typeid(detail::null_type))
    {}

    type_id(std::type_info const& id)
      : id(&id)
    {}

    bool operator!=(type_id const& other) const
    {
        return *id != *other.id;
    }

    bool operator==(type_id const& other) const
    {
        return *id == *other.id;
    }

    bool operator<(type_id const& other) const
    {
        return id->before(*other.id);
    }

    friend bool operator>(const type_id& x, const type_id& y)  { return y < x; }
    friend bool operator<=(const type_id& x, const type_id& y) { return !static_cast<bool>(y < x); }
    friend bool operator>=(const type_id& x, const type_id& y) { return !static_cast<bool>(x < y); }

    char const* name() const
    {
        return id->name();
    }

private:
    std::type_info const* id;
};

} // namespace luabind

#endif // LUABIND_TYPEID_081227_HPP

