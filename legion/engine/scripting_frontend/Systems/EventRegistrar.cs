#nullable enable
using System;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using Legion.Attributes;

namespace Legion.Systems
{
    [GameSystem]
    class EventRegistrar
    {
        public void Init()
        {


            //enumerate all methods with OnEvent
            var enumeration =
                from a in AppDomain.CurrentDomain.GetAssemblies().AsParallel()
                from type in a.GetTypes()
                from method in type.GetMethods()
                let attributes = method.GetCustomAttributes(typeof(OnEventAttribute), true)
                where attributes.Length > 0
                select new {type,method};

            foreach(var e in enumeration)
            {


                foreach (var gs in Engine.GameSystems)
                {
                    if (gs.GetType() == e.type)
                    {

                        //probe the method we need to register
                        var parameters = e.method.GetParameters();
                        if(parameters.Length > 1) throw new ArgumentException("Event Methods may only have one parameter!");
                        var param = parameters[0];
                        Type generic = param.ParameterType;


                        //get less generic version of "Subscribe"
                        MethodInfo eventbusSubscribe =
                            typeof(EventBus).GetMethod("Subscribe", BindingFlags.Static | BindingFlags.Public);
                        Debug.Assert(eventbusSubscribe != null, nameof(eventbusSubscribe) + " != null");
                        eventbusSubscribe = eventbusSubscribe.MakeGenericMethod(generic);



                        var wrapper = typeof(Action<>).MakeGenericType(generic);
                        var deleg = Delegate.CreateDelegate(wrapper, gs, e.method);
                        
                        //create some extremely unnecessary boilerplate code
                        eventbusSubscribe.Invoke(null, new[]{deleg});
                    }
                }
            }
        }
    }
}
