#pragma once
#include <core/types/reflector.hpp>
struct escape_cursor_action;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::escape_cursor_action>(::escape_cursor_action& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::escape_cursor_action>(const ::escape_cursor_action& obj);
}
