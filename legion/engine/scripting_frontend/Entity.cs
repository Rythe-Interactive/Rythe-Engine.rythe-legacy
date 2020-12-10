using System;
using System.Linq;
using JetBrains.Annotations;

namespace Legion
{
    [PublicAPI]
    public class Entity
    {
        // ========================== Implementation Details ========================== 
        public delegate bool ValidateFn(ulong entity);
        private static ValidateFn ValidateInternal;

        public delegate ulong GetParentFn(ulong entity);
        private static GetParentFn GetParentImpl;

        public delegate ulong GetChildCountFn(ulong entity);
        private static GetChildCountFn GetChildCountImpl;

        public delegate ulong GetChildFn(ulong entity, ulong index);
        private static GetChildFn GetChildImpl;

        public delegate bool CompareTagFn(ulong entity, string tag);
        private static CompareTagFn CompareTagImpl;

        public delegate GetOut.Container GetAllEntitiesFn();
        public static GetAllEntitiesFn GetAllEntitiesImpl;

        public delegate ulong CreateEntityFn();
        private static CreateEntityFn CreateEntityImpl;

        public delegate void RegisterEntityFnsFn(
            ValidateFn fn1,
            GetParentFn fn2,
            GetChildCountFn fn3,
            GetChildFn fn4,
            CompareTagFn fn5,
            GetAllEntitiesFn fn6,
            CreateEntityFn fn7
        );

        public static void RegisterEntityFns(
            ValidateFn fn1,
            GetParentFn fn2,
            GetChildCountFn fn3,
            GetChildFn fn4,
            CompareTagFn fn5,
            GetAllEntitiesFn fn6,
            CreateEntityFn fn7
        )
        {
            ValidateInternal = fn1;
            GetParentImpl = fn2;
            GetChildCountImpl = fn3;
            GetChildImpl = fn4;
            CompareTagImpl = fn5;
            GetAllEntitiesImpl = fn6;
            CreateEntityImpl = fn7;
        }

        // ========================== ====================== ========================== 

        internal Entity(ulong id)
        {
            this.id = id;
        }

        private ulong id = 0;
        public ulong Id => ValidateInternal(id) ? id : 0;

        public static bool operator true(Entity val) => val.Id != 0;
        public static bool operator false(Entity val) => val.Id == 0;

        public static bool operator ==(Entity left, Entity right)
        {
            if (ReferenceEquals(left, null))
            {
                return ReferenceEquals(right, null);
            }

            return left.Id == right?.Id;
        }
        public static bool operator !=(Entity left, Entity right)
        {
            return !(left == right);
        }

        public bool Valid()
        {
            return Id != 0;
        }
        
        public Entity GetParent()
        {
            return new Entity(GetParentImpl(this.id));
        }

        public int GetChildCount()
        {
            return (int)GetChildCountImpl(this.id);
        }

        [CanBeNull]
        public Entity GetChild(int index)
        {
            if (index >= GetChildCount())
                return null;

            return new Entity(GetChildImpl(this.id, (ulong)index));
        }

        public Entity[] GetChildren()
        {
            Entity[] children = new Entity[GetChildCount()];

            for (int i = 0; i < children.Length; i++)
            {
                children[i] = GetChild(i);
            }

            return children;
        }

        public bool HasTag(string tag)
        {
            return CompareTagImpl(this.id, tag);
        }

        public Entity[] GetAllEntities()
        {
            var container = GetAllEntitiesImpl();
            ulong[] ids = GetOut.CopyToDotNetULong(container);
            GetOut.FreeContainer(container);
            return ids.Select(x => new Entity(x)).ToArray();
        }

        public static Entity[] GetEntitiesByTag(string tag)
        {
            var container = GetAllEntitiesImpl();
            ulong[] ids = GetOut.CopyToDotNetULong(container);

            ulong[] matches = Array.FindAll(ids, x => CompareTagImpl(x, tag));

            return matches.Select(x => new Entity(x)).ToArray();
        }

        public static Entity CreateEntity() => new Entity(CreateEntityImpl());
    }
}
