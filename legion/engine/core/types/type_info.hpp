#pragma once
#include <core/platform/platform.hpp>
#include <core/types/primitives.hpp>
#include <core/types/type_util.hpp>
#include <core/containers/pointer.hpp>

#include <unordered_map>
#include <memory>

namespace legion::core
{
    struct base_type_info
    {

    };

    template<typename ObjectType>
    struct type_info : base_type_info
    {
        id_type typeId = typeHash<ObjectType>();
        std::string typeName = nameOfType<ObjectType>();
        //std::string nameSpace;
        std::vector<id_type> parents;
        std::vector<id_type> children;
        //std::unordered_map<id_type, std::unique_ptr<base_type_info>> templateParameters;
        //std::unordered_map<id_type, std::unique_ptr<base_type_info>> attributes;
        //std::unordered_map<id_type, std::unique_ptr<base_type_info>> members;
        //std::unordered_map<id_type, std::unique_ptr<base_type_info>> functions;
        type_info();
        ~type_info();
        template<typename type>
        void addChild();
        template<typename type>
        void addParent();
    };

    class TypeInfoRegistry
    {
    private:
        static std::unordered_map<id_type, std::unique_ptr<base_type_info>> registry;
    public:
        template<typename ObjectType>
        static pointer<type_info<ObjectType>> registerType();
        template<typename ObjectType>
        static pointer<type_info<ObjectType>> getTypeInfo();
    };
}

#include <core/types/type_info.inl>
