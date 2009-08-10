// Copyright Daniel Wallin 2009. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUABIND_INHERITANCE_090217_HPP
# define LUABIND_INHERITANCE_090217_HPP

# include <cassert>
# include <limits>
# include <map>
# include <memory>
# include <vector>
# include <luabind/typeid.hpp>

namespace luabind { namespace detail {

typedef void*(*cast_function)(void*);
typedef std::size_t class_id;

class_id const unknown_class = std::numeric_limits<class_id>::max();

class class_rep;

class LUABIND_API cast_graph
{
public:
    cast_graph();
    ~cast_graph();

    // `src` and `p` here describe the *most derived* object. This means that
    // for a polymorphic type, the pointer must be cast with
    // dynamic_cast<void*> before being passed in here, and `src` has to
    // match typeid(*p).
    std::pair<void*, std::size_t> cast(
        void* p, class_id src, class_id target
      , class_id dynamic_id, void const* dynamic_ptr) const;
    void insert(class_id src, class_id target, cast_function cast);

private:
    class impl;
    std::auto_ptr<impl> m_impl;
};

// Maps a type_id to a class_id. Note that this..
class class_id_map
{
public:
    class_id get(type_id const& type) const;
    class_id get_local(type_id const& type);
    void put(class_id id, type_id const& type);

private:
    typedef std::map<type_id, class_id> map_type;
    map_type m_classes;
};

inline class_id class_id_map::get(type_id const& type) const
{
    map_type::const_iterator i = m_classes.find(type);
    if (i == m_classes.end() || i->second > unknown_class / 2)
        return unknown_class;
    return i->second;
}

inline class_id class_id_map::get_local(type_id const& type)
{
    std::pair<map_type::iterator, bool> result = m_classes.insert(
        std::make_pair(type, 0));

    if (result.second)
        result.first->second = unknown_class - m_classes.size();

    return result.first->second;
}

inline void class_id_map::put(class_id id, type_id const& type)
{
    std::pair<map_type::iterator, bool> result = m_classes.insert(
        std::make_pair(type, id));
    assert(result.second || result.first->second == id);
    (void)result;
}

class class_map
{
public:
    class_rep* get(class_id id) const;
    void put(class_id id, class_rep* cls);

private:
    std::vector<class_rep*> m_classes;
};

inline class_rep* class_map::get(class_id id) const
{
    if (id >= m_classes.size())
        return 0;
    return m_classes[id];
}

inline void class_map::put(class_id id, class_rep* cls)
{
    if (id >= m_classes.size())
        m_classes.resize(id + 1);
    m_classes[id] = cls;
}

template <class S, class T>
struct static_cast_
{
    static void* execute(void* p)
    {
        return static_cast<T*>(static_cast<S*>(p));
    }
};

template <class S, class T>
struct dynamic_cast_
{
    static void* execute(void* p)
    {
        return dynamic_cast<T*>(static_cast<S*>(p));
    }
};

// Thread safe class_id allocation.
LUABIND_API class_id allocate_class_id();

template <class T>
struct registered_class
{
    static class_id const id;
};

template <class T>
class_id const registered_class<T>::id = allocate_class_id();

template <class T>
struct registered_class<T const>
  : registered_class<T>
{};

}} // namespace luabind::detail

#endif // LUABIND_INHERITANCE_090217_HPP
