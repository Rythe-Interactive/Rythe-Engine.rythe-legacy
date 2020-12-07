using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using JetBrains.Annotations;

namespace Legion
{
    [PublicAPI,AttributeUsage(AttributeTargets.Struct)]
    public class EventAttribute : Attribute
    {
        public EventAttribute(string name) => Name = name;
        public readonly string Name;
    }

    [PublicAPI]
    [Event("core::exit")]
    [StructLayout(LayoutKind.Sequential)]
    public struct ExitEvent
    {
        public readonly int ExitCode;
    }


    [PublicAPI]
    public class EventBus
    {

        public delegate GetOut.Container GetPendingDataFn();
        public static GetPendingDataFn GetPendingData;

        public delegate void RegisterGetPendingDataFuncFn(GetPendingDataFn fn);
        public static void RegisterGetPendingDataFunc(GetPendingDataFn fn)
        {
            GetPendingData = fn;
        }


        public delegate void OnPendingDataFn();
        public static void OnPendingData()
        {

            //TODO:comment
            //NEVER CHANGE THIS!
            var ctr = GetPendingData();
            string str = Marshal.PtrToStringAnsi(ctr.ptr);
            long[] tmp = new long[1];
            Marshal.Copy(ctr.ptr + ((int)ctr.size - Marshal.SizeOf<long>()), tmp,0,1);
            ulong id = MemoryMarshal.Cast<long, ulong>(tmp)[0];
            GetOut.FreeContainer(ctr);

            m_unmapped_events[str!] = id;

        }


        private static Dictionary<Type, ulong> m_events = new Dictionary<Type, ulong>();
        private static Dictionary<string, ulong> m_unmapped_events = new Dictionary<string, ulong>();

        private static Dictionary<ulong, Action<IntPtr>> m_emitters;

        public static void Subscribe<T>(Action<T> evt)
        {
            var attribs = typeof(T).GetCustomAttributes(typeof(EventAttribute), false);
            if(!(attribs.Length > 0)) throw new InvalidOperationException("You can only subscribe to Events that have the [Event] Attribute");

            if (m_events.ContainsKey(typeof(T)))
            {
                RegisterToType(evt);
            }
            else
            {
                CreateEventType<T>((EventAttribute)attribs[0]);
                RegisterToType(evt);
            }
        }

        public static void Emit<T>(T evt)
        {
            //TODO(algo-ryth-mix): Get CPP backend & registries and emit event on eventbus
            //Also need to find a way to convert C# structs to Unmarshaled type (aka IntPtr)
        }

        private static void RegisterToType<T>(Action<T> evt)
        {
            m_emitters[m_events[typeof(T)]] += ptr =>
            {
                var structure = Marshal.PtrToStructure<T>(ptr);
                if (structure != null)
                {
                    evt(structure);
                }
                else throw new InvalidCastException($"Attempt to cast Ptr to \"{typeof(T).FullName}\" failed!");
            };
        }

        private static void CreateEventType<T>(EventAttribute attr)
        {
            if (m_unmapped_events.ContainsKey(attr.Name))
            {
                var id = m_unmapped_events[attr.Name];
                m_events.Add(typeof(T),id);
            } else throw new ArgumentOutOfRangeException("attr",$"The Event you tried to create does not exist in the registry! {attr.Name}");
        }


    }
}
