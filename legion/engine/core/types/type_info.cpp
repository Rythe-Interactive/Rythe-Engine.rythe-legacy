#include <core/types/type_info.hpp>

namespace legion::core
{
    std::unordered_map<id_type, std::unique_ptr<type_info_base>> TypeInfoRegistry::registry;
}
