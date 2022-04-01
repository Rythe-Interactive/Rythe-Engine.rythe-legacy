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
        refl.members.emplace("fov", member_reference("fov", primitive_reference{typeHash<float>(), &obj.fov}));
        refl.members.emplace("nearz", member_reference("nearz", primitive_reference{typeHash<float>(), &obj.nearz}));
        refl.members.emplace("farz", member_reference("farz", primitive_reference{typeHash<float>(), &obj.farz}));
        refl.members.emplace("targetWindow", member_reference("targetWindow", reflector(typeHash<decltype(obj.targetWindow)>(), nameOfType<decltype(obj.targetWindow)>(), reflector::member_container(), std::addressof(obj.targetWindow))));
        refl.members.emplace("renderTarget", member_reference("renderTarget", reflector(typeHash<decltype(obj.renderTarget)>(), nameOfType<decltype(obj.renderTarget)>(), reflector::member_container(), std::addressof(obj.renderTarget))));
        refl.members.emplace("clearColor", member_reference("clearColor", reflector(typeHash<decltype(obj.clearColor)>(), nameOfType<decltype(obj.clearColor)>(), reflector::member_container(), std::addressof(obj.clearColor))));
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
        refl.members.emplace("fov", member_reference("fov", primitive_reference{typeHash<float>(), &obj.fov}));
        refl.members.emplace("nearz", member_reference("nearz", primitive_reference{typeHash<float>(), &obj.nearz}));
        refl.members.emplace("farz", member_reference("farz", primitive_reference{typeHash<float>(), &obj.farz}));
        refl.members.emplace("targetWindow", member_reference("targetWindow", reflector(typeHash<decltype(obj.targetWindow)>(), nameOfType<decltype(obj.targetWindow)>(), reflector::member_container(), std::addressof(obj.targetWindow))));
        refl.members.emplace("renderTarget", member_reference("renderTarget", reflector(typeHash<decltype(obj.renderTarget)>(), nameOfType<decltype(obj.renderTarget)>(), reflector::member_container(), std::addressof(obj.renderTarget))));
        refl.members.emplace("clearColor", member_reference("clearColor", reflector(typeHash<decltype(obj.clearColor)>(), nameOfType<decltype(obj.clearColor)>(), reflector::member_container(), std::addressof(obj.clearColor))));
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
