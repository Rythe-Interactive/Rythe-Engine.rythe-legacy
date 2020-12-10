using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using JetBrains.Annotations;
using Legion.Attributes;


//TODO(algo-ryth-mix): make this stuff faster with "unsafe features" (clearly .NET developers never saw any of my CPP code)
//https://levelup.gitconnected.com/the-hidden-unsafe-features-of-c-63895400800a

namespace Legion
{
    [PublicAPI("Used in Backend")]
    class Engine
    {
        public static event Action OnInitializeEvent;
        public static event Action<float> OnUpdateEvent;

        private static List<object> m_gameSystems;
        public static List<object> GameSystems => m_gameSystems;

        
        public delegate void InitializeFn();
        public static void Initialize()
        {
            //Load External "Game" Assemblies
            foreach (var asmV in Directory.GetFiles(".\\assemblies", "*.dll", SearchOption.AllDirectories))
            {

                //do not load self
                if (asmV.EndsWith("dotnetlegion.dll")) continue;

                //read and load
                byte[] b = File.ReadAllBytes(asmV);
                Assembly a = Assembly.Load(b);
            }


            Log.Debug("assembling Dependency resolver");

            //the assemblies are probably going to depend on dotnetlegion.dll itself
            //thus we need to resolve their dependencies
            //this also makes it so that we can load other dependencies in that folder
            //automatically
            AppDomain.CurrentDomain.AssemblyResolve += (object sender, ResolveEventArgs e) =>
            {
                foreach (var a in AppDomain.CurrentDomain.GetAssemblies())
                {
                    Log.Trace($"{a.GetName().FullName}:{e.Name}");
                    if (a.GetName().FullName == e.Name)
                    {
                        return a;
                    }
                }
                return null;
            };


            Log.Debug("collecting GameSystems");

            //get all types with the GameSystemAttribute attribute,
            //this messed up linq statement somehow does that
            var enumerationWithGameSystemAttributes =
                from a in AppDomain.CurrentDomain.GetAssemblies().AsParallel()
                from t in a.GetTypes()
                let attributes = t.GetCustomAttributes(typeof(GameSystemAttribute), true)
                where attributes.Length > 0
                select t;

            Log.Debug("GameSystems collected");


            var types = enumerationWithGameSystemAttributes.ToList();

            m_gameSystems = new List<object>(types.Count);


            //iterate over all types
            foreach (var type in types)
            {

                // create a new instance of the targeted class
                var instance = Activator.CreateInstance(type);
                if (instance is null)
                {
                    Log.Debug($"Warning cannot create instance of {type.FullName}");
                    continue;
                }

                Log.Debug($"Processing Hooks for {type.FullName}");


                //add it to our system collection
                m_gameSystems.Add(instance);


                //get a ref to the Init Method of the instance if it has one and add it to the onInitialize event
                MethodInfo init_method = type.GetMethod("Init");
                if (init_method is not null)
                {
                    Log.Debug("\t- Init hook registered");
                    OnInitializeEvent += () => init_method.Invoke(instance, null);
                }

                //get a ref to the Update Method of the instance if it has one and add it to the onUpdate event
                MethodInfo update_method = type.GetMethod("Update");
                if (update_method is not null)
                {
                    Log.Debug("\t- Update hook registered");
                    OnUpdateEvent += x => update_method.Invoke(instance, new object[] { x });
                }
            }

            //invoke the onInitialize event
            OnInitializeEvent?.Invoke();
        }

        public delegate void UpdateFn(float delta);
        public static void Update(float delta)
        {
            OnUpdateEvent?.Invoke(delta);
        }
    }
}
