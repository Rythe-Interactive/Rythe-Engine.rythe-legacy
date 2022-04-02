#include "autogen_reflector_camera.hpp"
#include "../../rendering/components/camera.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::rendering::camera>(legion::rendering::camera& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::rendering::camera>();
        refl.typeName = "legion::rendering::camera";
        {
            static const legion::reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("targetWindow", member_reference("targetWindow", make_reflector(obj.targetWindow)));
        refl.members.emplace("renderTarget", member_reference("renderTarget", make_reflector(obj.renderTarget)));
        refl.members.emplace("clearColor", member_reference("clearColor", make_reflector(obj.clearColor)));
        refl.members.emplace("fov", member_reference("fov", primitive_reference{typeHash(obj.fov), &obj.fov}));
        refl.members.emplace("nearz", member_reference("nearz", primitive_reference{typeHash(obj.nearz), &obj.nearz}));
        refl.members.emplace("farz", member_reference("farz", primitive_reference{typeHash(obj.farz), &obj.farz}));
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::rendering::camera>(const legion::rendering::camera& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::rendering::camera>();
        refl.typeName = "legion::rendering::camera";
        {
            static const legion::reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("targetWindow", member_reference("targetWindow", make_reflector(obj.targetWindow)));
        refl.members.emplace("renderTarget", member_reference("renderTarget", make_reflector(obj.renderTarget)));
        refl.members.emplace("clearColor", member_reference("clearColor", make_reflector(obj.clearColor)));
        refl.members.emplace("fov", member_reference("fov", primitive_reference{typeHash(obj.fov), &obj.fov}));
        refl.members.emplace("nearz", member_reference("nearz", primitive_reference{typeHash(obj.nearz), &obj.nearz}));
        refl.members.emplace("farz", member_reference("farz", primitive_reference{typeHash(obj.farz), &obj.farz}));
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
