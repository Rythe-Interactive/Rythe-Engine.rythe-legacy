#include "autogen_prototype_camera.hpp"
#include "../../rendering/components/camera.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::camera>(const legion::rendering::camera& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::rendering::camera>();
        prot.typeName = "legion::rendering::camera";
        {
            static const legion::reflectable_attribute reflectable_attr{};
            prot.attributes.push_back(std::cref(reflectable_attr));
        }
        prot.members.emplace("renderTarget", member_value("renderTarget", make_prototype(obj.renderTarget)));
        prot.members.emplace("clearColor", member_value("clearColor", make_prototype(obj.clearColor)));
        prot.members.emplace("fov", member_value("fov", primitive_value{typeHash(obj.fov), std::any(obj.fov)}));
        prot.members.emplace("nearz", member_value("nearz", primitive_value{typeHash(obj.nearz), std::any(obj.nearz)}));
        prot.members.emplace("farz", member_value("farz", primitive_value{typeHash(obj.farz), std::any(obj.farz)}));
        return prot;
    }
}
