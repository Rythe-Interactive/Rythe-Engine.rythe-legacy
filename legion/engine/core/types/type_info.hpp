#pragma once
#include <core/platform/platform.hpp>
#include <core/types/primitives.hpp>
#include <core/types/type_util.hpp>
#include <core/containers/pointer.hpp>

#include <unordered_map>
#include <memory>

namespace legion::core
{
    struct type_info_base
    {

    };

    template<typename ObjectType>
    struct type_info : type_info_base
    {
    public:
        using nameHash = id_type;
        using typeHash = id_type;
        id_type typeId;
        std::string typeName;

    private:
        std::vector<id_type> parents;
        std::vector<id_type> children;
        //std::unordered_map<nameHash, typeHash> templateParameters;
        //std::unordered_map<nameHash, typeHash> attributes;
        std::unordered_map<nameHash, typeHash> members;
        //std::unordered_map<nameHash, typeHash> functions;

    public:
        type_info()
        {
            typeId = typeHash<ObjectType>();
            typeName = nameOfType<ObjectType>();
        };
        ~type_info();

        template<typename ChildType>
        void addChild();
        void addChild(id_type typeId);
        template<typename ParentType>
        void addParent();
        void addParent(id_type typeId);

        template<typename MemberType>
        void addMember(const std::string& name);
        void addMember(const std::string& name, id_type typeId);

        template<typename MemberType>
        pointer<type_info<MemberType>> getMember(const std::string& name);
        pointer<type_info_base> getMember(const std::string& name);

        const std::string& to_string()
        {
            return "Type Name: " + typeName +
                   "Type ID: " + typeId;
        }
    };

    class TypeInfoRegistry
    {
    private:
        static std::unordered_map<id_type, std::unique_ptr<type_info_base>> registry;
    public:
        template<typename ObjectType>
        static pointer<type_info<ObjectType>> registerType();
        template<typename ObjectType>
        static pointer<type_info<ObjectType>> getTypeInfo();
        static pointer<type_info_base> getTypeInfo(id_type typeId);
    };
}

#include <core/types/type_info.inl>
