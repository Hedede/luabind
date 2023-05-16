find_package(Lua REQUIRED)

add_library(luabind STATIC)

target_sources(luabind PUBLIC
	class.cpp	    exception_handler.cpp   pcall.cpp
	class_info.cpp	    function.cpp	    scope.cpp
	class_registry.cpp  inheritance.cpp	    stack_content_by_name.cpp
	class_rep.cpp	    link_compatibility.cpp  weak_ref.cpp
	create_class.cpp    object_rep.cpp	    wrapper_base.cpp
	error.cpp	    open.cpp)


target_include_directories(luabind
	PUBLIC
		${LUA_INCLUDE_DIR})

target_link_libraries(luabind
	PUBLIC
		awtypes
		${LUA_LIBRARIES})

