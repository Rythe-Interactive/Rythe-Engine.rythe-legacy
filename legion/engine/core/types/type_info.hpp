#pragma once
#include <core/platform/platform.hpp>
#include <core/types/primitives.hpp>
#include <core/types/type_util.hpp>
#include <core/containers/pointer.hpp>

#include <unordered_map>
#include <memory>

namespace legion::core
{
    class TypeInfoRegistry;
    struct type_info_base
    {
    public:
        id_type typeId;
        std::string typeName;

        type_info_base() = default;
        virtual ~type_info_base() = default;

        virtual std::string to_string() LEGION_PURE;
    };

    template<typename ObjectType>
    struct type_info : public type_info_base
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
            typeId = make_hash<ObjectType>().id();
            typeName = nameOfType<ObjectType>();
        };
        virtual ~type_info() = default;

        template<typename ChildType>
        void add_child();
        void add_child(id_type typeId);
        template<typename ParentType>
        void add_parent();
        void add_parent(id_type typeId);

        template<typename MemberType>
        void add_member(const std::string& name);
        void add_member(const std::string& name, id_type typeId);

        template<typename MemberType>
        pointer<type_info<MemberType>> get_member(const std::string& name);
        pointer<type_info_base> get_member(const std::string& name);

        virtual std::string to_string() override
        {
            std::string outPut = "Type Name: " + typeName +
                "\nType ID: " + std::to_string(typeId) +
                "\n\tChildren:\n";
            for (size_t i = 0; i < children.size(); i++)
            {
                outPut.append("\t" + TypeInfoRegistry::getTypeInfo(children[i])->to_string() + "\n");
            }
            return outPut;
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
