using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using Legion.Attributes;

namespace Legion
{
    class Engine
    {
        public static event Action onInitializeEvent;
        public static event Action<float> onUpdateEvent;

        private static List<object> m_gameSystems =
            new List<object>();

        public delegate void InitializeFn();
        public static void Initialize()
        {

            Console.WriteLine("[C#] collecting GameSystems");

            //get all types with the GameSystemAttribute attribute,
            //this messed up linq statement somehow does that
            var typesWithMyAttribute =
                from a in AppDomain.CurrentDomain.GetAssemblies().AsParallel()
                from t in a.GetTypes()
                let attributes = t.GetCustomAttributes(typeof(GameSystemAttribute), true)
                where attributes.Length > 0
                select t;

            Console.WriteLine("[C#] GameSystems collected");


            //iterate over all types
            foreach (var type in typesWithMyAttribute)
            {

                // create a new instance of the targeted class
                var instance = Activator.CreateInstance(type);
                if (instance is null)
                {
                    Console.WriteLine($"[C#] Warning cannot create instance of {type.FullName}");
                    continue;
                }

                Console.WriteLine($"[C#] Processing Hooks for {type.FullName}");


                //add it to our system collection
                m_gameSystems.Add(instance);


                //get a ref to the Init Method of the instance if it has one and add it to the onInitialize event
                MethodInfo init_method = type.GetMethod("Init");
                if (init_method is not null)
                {
                    Console.WriteLine("[C#]\t- Init hook registered");
                    onInitializeEvent += () => init_method.Invoke(instance, null);
                }

                //get a ref to the Update Method of the instance if it has one and add it to the onUpdate event
                MethodInfo update_method = type.GetMethod("Update");
                if (update_method is not null)
                {
                    Console.WriteLine("[C#]\t- Update hook registered");
                    onUpdateEvent += (x) => update_method.Invoke(instance, new object[] { x });
                }
            }

            //invoke the onInitialize event
            onInitializeEvent?.Invoke();
        }

        public delegate void UpdateFn(float delta);
        public static void Update(float delta)
        {
            onUpdateEvent?.Invoke(delta);
        }
    }
}
