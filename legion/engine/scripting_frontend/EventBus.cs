using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using JetBrains.Annotations;
using Legion.Attributes;

namespace Legion
{


    [PublicAPI]
    [Event("core::exit")]
    [StructLayout(LayoutKind.Sequential)]
    public struct ExitEvent
    {
        public readonly int ExitCode;
    }

    [PublicAPI]
    [Event("core::component_creation<transform>")]
    [StructLayout(LayoutKind.Sequential)]
    public struct TransformCreatedEvent
    {
        private ulong entity;
        public Entity Parent => new Entity(entity);
    }

    [PublicAPI]
    [Event("notify_one")]
    [StructLayout(LayoutKind.Sequential)]
    public struct NotifyOne
    {
        public int Payload;
    }


    [PublicAPI]
    public class EventBus
    {
        public delegate void EmitEventImplFn(IntPtr ev,ulong size, ulong id);
        private static EmitEventImplFn EmitEventImpl;

        public delegate void RegisterEmitEventImplFnFn(EmitEventImplFn fn);
        public static void RegisterEmitEventImplFn(EmitEventImplFn fn)
        {
            EmitEventImpl = fn;
        }


        public delegate GetOut.Container GetPendingDataFn();
        public static GetPendingDataFn GetPendingData;

        public delegate void RegisterGetPendingDataFuncFn(GetPendingDataFn fn);
        public static void RegisterGetPendingDataFunc(GetPendingDataFn fn)
        {
            GetPendingData = fn;
        }


        [StructLayout(LayoutKind.Sequential)]
        private struct PendingData
        {
            public ulong Type;
            public ulong PayloadLength;
            public IntPtr Payload;
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct MessageRegisterEvent
        {
            public ulong id;
            public IntPtr name;
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct MessageEmitEvent
        {
            public ulong id;
            public IntPtr data;
        }

        public delegate void OnPendingDataFn();
        public static void OnPendingData()
        {

            var ctr = GetPendingData();

            PendingData data = Marshal.PtrToStructure<PendingData>(ctr.ptr);

            // 0x00 = event emitted
            if (data.Type == 0)
            {
                var msg = Marshal.PtrToStructure<MessageEmitEvent>(data.Payload);
                if (m_emitters.ContainsKey(msg.id))
                {
                    m_emitters[msg.id]?.Invoke(msg.data);
                }
            }

            // 0xFF = register event type
            if (data.Type == 0XFF)
            {
                var msg = Marshal.PtrToStructure<MessageRegisterEvent>(data.Payload);
                string str = Marshal.PtrToStringAnsi(msg.name);
                m_unmapped_events[str!] = msg.id;
                Console.WriteLine($"\t\t\t\t\t[C#] Registered <{str}> with id <{msg.id}>");
            }


            GetOut.FreeContainer(ctr);
        }


        private static Dictionary<Type, ulong> m_events = new Dictionary<Type, ulong>();
        private static Dictionary<string, ulong> m_unmapped_events = new Dictionary<string, ulong>();

        private static Dictionary<ulong, Action<IntPtr>> m_emitters = new Dictionary<ulong, Action<IntPtr>>();

        public static void Subscribe<T>(Action<T> evt)
        {
            
            if (m_events.ContainsKey(typeof(T)))
            {
                RegisterToType(evt);
            }
            else
            {
                var attribs = typeof(T).GetCustomAttributes(typeof(EventAttribute), false);
                if (!(attribs.Length > 0)) throw new InvalidOperationException("You can only subscribe to Events that have the [Event] Attribute");
                CreateEventType<T>((EventAttribute)attribs[0]);
                RegisterToType(evt);
            }
        }

        public static void Emit<T>(T evt)
        {
            //TODO(algo-ryth-mix): Get CPP backend & registries and emit event on eventbus
            //Also need to find a way to convert C# structs to Unmarshaled type (aka IntPtr)

            if (m_events.ContainsKey(typeof(T)))
            {
                EmitInner(evt);

            }
            else
            {
                var attribs = typeof(T).GetCustomAttributes(typeof(EventAttribute), false);
                if (!(attribs.Length > 0))
                    throw new InvalidOperationException(
                        "You can only subscribe to Events that have the [Event] Attribute");
                CreateEventType<T>((EventAttribute) attribs[0]);
                EmitInner(evt);
            }
        }

        private static void EmitInner<T>(T evt)
        {
            IntPtr ptr = Marshal.AllocHGlobal(Marshal.SizeOf<T>());
            Marshal.StructureToPtr(evt, ptr, false);

            var id = m_events[typeof(T)];

            EmitEventImpl(ptr, (ulong) Marshal.SizeOf<T>(), id);
        }

        private static void RegisterToType<T>(Action<T> evt)
        {
            if (!m_emitters.ContainsKey(m_events[typeof(T)]))
            {
                m_emitters[m_events[typeof(T)]] = ptr =>
                {
                    var structure = Marshal.PtrToStructure<T>(ptr);
                    if (structure != null)
                    {
                        evt?.Invoke(structure);
                    }
                    else throw new InvalidCastException($"Attempt to cast Ptr to \"{typeof(T).FullName}\" failed!");
                };
            }
            else
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
        }

        private static void CreateEventType<T>(EventAttribute attr)
        {
            if (m_unmapped_events.ContainsKey(attr.Name))
            {
                var id = m_unmapped_events[attr.Name];
                m_events.Add(typeof(T), id);
            }
            else throw new ArgumentOutOfRangeException("attr", $"The Event you tried to create does not exist in the registry! {attr.Name}");
        }
    }
}
