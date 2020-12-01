using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using JetBrains.Annotations;

namespace Legion
{
    [PublicAPI("Used by Backend")]
    public class GetOut
    {
        private static TTo[] Cast<TFrom, TTo>(TFrom[] source) where TTo : struct where TFrom : struct =>
            MemoryMarshal.Cast<TFrom, TTo>(source).ToArray();


        [StructLayout(LayoutKind.Sequential)]
        public struct Container
        {
            public IntPtr ptr;
            public ulong size;
        }

        public delegate void UnmanagedFreeArray(IntPtr ptr);
        private static UnmanagedFreeArray FreeImpl;

        
        public delegate void RegisterFreeFnFn(UnmanagedFreeArray fn);
        public static void RegisterFreeFn(UnmanagedFreeArray fn)
        {
            FreeImpl = fn;
        }

        public static void FreeContainer(Container c)
        {
            FreeImpl(c.ptr);
        }



        public static int[] CopyToDotNetInt(Container c) 
        {
            int[] array = new int[(int) c.size / Marshal.SizeOf<int>()];
            Marshal.Copy(c.ptr,array,0,array.Length);
            return array;
        }
        public static long[] CopyToDotNetLong(Container c)
        {
            long[] array = new long[(int)c.size / Marshal.SizeOf<long>()];
            Marshal.Copy(c.ptr, array, 0,array.Length);
            return array;
        }
        public static short[] CopyToDotNetShort(Container c)
        {
            short[] array = new short[(int)c.size / Marshal.SizeOf<short>()];
            Marshal.Copy(c.ptr, array, 0,array.Length);
            return array;
        }
        public static float[] CopyToDotNetFloat(Container c)
        {
            float[] array = new float[(int)c.size / Marshal.SizeOf<float>()];
            Marshal.Copy(c.ptr, array, 0,array.Length);
            return array;
        }
        public static double[] CopyToDotNetDouble(Container c)
        {
            double[] array = new double[(int)c.size / Marshal.SizeOf<double>()];
            Marshal.Copy(c.ptr, array, 0,array.Length);
            return array;
        }

        public static char[] CopyToDotNetChar(Container c)
        {
            char[] array = new char[(int)c.size / Marshal.SizeOf<char>()];
            Marshal.Copy(c.ptr, array, 0,array.Length);
            return array;
        }

        public static byte[] CopyToDotNetByte(Container c)
        {
            byte[] array = new byte[(int)c.size / Marshal.SizeOf<byte>()];
            Marshal.Copy(c.ptr, array, 0,array.Length);
            return array;
        }

        public static uint[] CopyToDotNetUInt(Container c) 
        {
            int[] array = new int[(int) c.size / Marshal.SizeOf<uint>()];
            Marshal.Copy(c.ptr,array,0,(int)c.size);
            return Cast<int, uint>(array);
        }
        public static ulong[] CopyToDotNetULong(Container c)
        {
            long[] array = new long[(int)c.size / Marshal.SizeOf<ulong>()];
            Marshal.Copy(c.ptr, array, 0,array.Length);
            return Cast<long,ulong>(array);
        }
        public static ushort[] CopyToDotNetUShort(Container c)
        {
            short[] array = new short[(int)c.size / Marshal.SizeOf<ushort>()];
            Marshal.Copy(c.ptr, array, 0,array.Length);
            return Cast<short,ushort>(array);
        }
    }
}
