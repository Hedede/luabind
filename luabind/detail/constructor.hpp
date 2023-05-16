// Copyright Daniel Wallin 2008. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# ifndef LUABIND_DETAIL_CONSTRUCTOR_081018_HPP
#  define LUABIND_DETAIL_CONSTRUCTOR_081018_HPP

#  include <luabind/get_main_thread.hpp>
#  include <luabind/object.hpp>
#  include <luabind/wrapper_base.hpp>
#  include <luabind/detail/inheritance.hpp>


namespace luabind { namespace detail {

inline void inject_backref(lua_State*, void*, void*)
{}

template <class T>
void inject_backref(lua_State* L, T* p, wrap_base*)
{
    weak_ref(get_main_thread(L), L, 1).swap(wrap_access::ref(*p));
}


template <class T, class Pointer, class Signature>
struct construct;

template <class T, class Pointer, class R, class Self, class... Args>
struct construct<T, Pointer, vector<R, Self, Args...> >
{
    typedef pointer_holder<Pointer, T> holder_type;

    void operator()(argument const& self_, Args... args) const
    {
        object_rep* self = touserdata<object_rep>(self_);

        std::unique_ptr<T> instance(new T(args...));
        inject_backref(self_.interpreter(), instance.get(), instance.get());

        void* naked_ptr = instance.get();
        Pointer ptr(instance.release());

        void* storage = self->allocate(sizeof(holder_type));

        self->set_instance(new (storage) holder_type(
            std::move(ptr), registered_class<T>::id, naked_ptr));
    }
};

}} // namespace luabind::detail

# endif // LUABIND_DETAIL_CONSTRUCTOR_081018_HPP
