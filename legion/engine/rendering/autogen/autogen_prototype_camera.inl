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
        prot.members.emplace("fov", member_value("fov", primitive_value{typeHash<float>(), std::make_any<float>(obj.fov)}));
        prot.members.emplace("nearz", member_value("nearz", primitive_value{typeHash<float>(), std::make_any<float>(obj.nearz)}));
        prot.members.emplace("farz", member_value("farz", primitive_value{typeHash<float>(), std::make_any<float>(obj.farz)}));
        prot.members.emplace("targetWindow", member_value("targetWindow", prototype(typeHash<decltype(obj.targetWindow)>(), nameOfType<decltype(obj.targetWindow)>(), prototype::member_container())));
        prot.members.emplace("renderTarget", member_value("renderTarget", prototype(typeHash<decltype(obj.renderTarget)>(), nameOfType<decltype(obj.renderTarget)>(), prototype::member_container())));
        prot.members.emplace("clearColor", member_value("clearColor", prototype(typeHash<decltype(obj.clearColor)>(), nameOfType<decltype(obj.clearColor)>(), prototype::member_container())));
        return prot;
    }
}
