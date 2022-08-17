#pragma once
#include <core/platform/platform.hpp>
#include <core/types/primitives.hpp>
#include <core/types/type_util.hpp>
#include <core/containers/pointer.hpp>


#include <unordered_map>
#include <memory>

namespace legion::core
{
    enum DataType
    {
        STRUCT,
        CLASS,
        FUNC,
        ATTRIB,
        PRIMITIVE
    };

    class TypeInfoRegistry;
    struct type_info_base
    {
    public:
        id_type typeId;
        std::string typeName;

        type_info_base() = default;
        virtual ~type_info_base() = default;

        virtual std::string to_string(int indent = 0) LEGION_PURE;
    };

    template<typename ObjectType>
    struct type_info : public type_info_base
    {
    public:
        using name = std::string;
        using typeHash = id_type;
        id_type typeId;
        std::string typeName;
        DataType type;

    private:
        std::vector<id_type> parents;
        std::vector<id_type> children;
        std::unordered_map<name, typeHash> templateParameters;
        std::unordered_map<name, typeHash> attributes;
        std::unordered_map<name, typeHash> members;
        std::unordered_map<name, typeHash> functions;

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

        template<typename TemplateParameterType>
        void add_template_parameter(const std::string& name);
        void add_template_parameter(const std::string& name, id_type typeId);

        template<typename AttributeType>
        void add_attribute(const std::string& name);
        void add_attribute(const std::string& name, id_type typeId);

        template<typename MemberType>
        void add_member(const std::string& name);
        void add_member(const std::string& name, id_type typeId);

        template<typename FunctionType>
        void add_function(const std::string& name);
        void add_function(const std::string& name, id_type typeId);


        template<typename ChildType>
        pointer<type_info<ChildType>> get_child() const noexcept;
        pointer<type_info_base> get_child(id_type typeId) const noexcept;

        template<typename ParentType>
        pointer<type_info<ParentType>> get_parent() const noexcept;
        pointer<type_info_base> get_parent(id_type typeId) const noexcept;

        template<typename MemberType>
        pointer<type_info<MemberType>> get_template_parameter(const std::string& name) const noexcept;
        pointer<type_info_base> get_template_parameter(const std::string& name) const noexcept;

        template<typename MemberType>
        pointer<type_info<MemberType>> get_attribute(const std::string& name) const noexcept;
        pointer<type_info_base> get_attribute(const std::string& name) const noexcept;

        template<typename MemberType>
        pointer<type_info<MemberType>> get_member(const std::string& name) const noexcept;
        pointer<type_info_base> get_member(const std::string& name) const noexcept;

        template<typename MemberType>
        pointer<type_info<MemberType>> get_function(const std::string& name) const noexcept;
        pointer<type_info_base> get_function(const std::string& name) const noexcept;

        virtual std::string to_string(int indent = 0) override
        {
            std::string outPut = "Type Name: " + typeName +
                /*           "\n" + std::string(indent, '\t') + "Data Type: " + std::to_string(type) +*/
                "\n" + std::string(indent, '\t') + "Type ID: " + std::to_string(typeId);

            if (templateParameters.size() > 0)
            {
                outPut.append("\n" + std::string(indent, '\t') + "Template Parameters:\n");
                indent++;
                for (auto& [name, typeId] : templateParameters)
                {
                    outPut.append(std::string(indent, '\t') + TypeInfoRegistry::getTypeInfo(typeId)->to_string(indent) + "\n");
                }
                indent--;
            }
            if (attributes.size() > 0)
            {
                outPut.append("\n" + std::string(indent, '\t') + "Attributes:\n");
                indent++;
                for (auto& [name, typeId] : attributes)
                {
                    outPut.append(std::string(indent, '\t') + TypeInfoRegistry::getTypeInfo(typeId)->to_string(indent) + "\n");
                }
                indent--;
            }
            if (members.size() > 0)
            {
                outPut.append("\n" + std::string(indent, '\t') + "Members:\n");
                indent++;
                for (auto& [name, typeId] : members)
                {
                    outPut.append(std::string(indent, '\t') + "Member Name: " + name + "\n");
                    outPut.append(std::string(indent, '\t') + TypeInfoRegistry::getTypeInfo(typeId)->to_string(indent) + "\n");
                }
                indent--;
            }
            if (functions.size() > 0)
            {
                outPut.append("\n" + std::string(indent, '\t') + "Functions:\n");
                indent++;
                for (auto& [name, typeId] : functions)
                {
                    outPut.append(std::string(indent, '\t') + "Function Name: " + name + "\n");
                    outPut.append(std::string(indent, '\t') + TypeInfoRegistry::getTypeInfo(typeId)->to_string(indent) + "\n");
                }
                indent--;
            }
            if (children.size() > 0)
            {
                outPut.append("\n" + std::string(indent, '\t') + "Children:\n");
                indent++;
                for (size_t i = 0; i < children.size(); i++)
                {
                    outPut.append(std::string(indent, '\t') + TypeInfoRegistry::getTypeInfo(children[i])->to_string(indent) + "\n");
                }
                indent--;
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
