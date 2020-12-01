using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using JetBrains.Annotations;

namespace Legion
{
    [PublicAPI]
    public class AssetView
    {
        public AssetView(string path)
        {
            m_path = path;
        }

        private string m_path;

        public delegate GetOut.Container ReadFileFn(string path);


        public delegate IntPtr LsFilesFn(string path);

        private static ReadFileFn m_rFileImpl;
        private static LsFilesFn m_lsFiles;

        [PublicAPI("Used in Backend")]
        public delegate void RegisterReadFileFnsFn(ReadFileFn fn1,LsFilesFn fn3);
        [PublicAPI("Used in Backend")]
        public static void RegisterReadFileFns(ReadFileFn fn1, LsFilesFn fn3)
        {
            m_rFileImpl = fn1;
            m_lsFiles = fn3;
        }

        public byte[] ReadFile()
        {
            ulong size;

            var ret = m_rFileImpl(m_path);
            if (ret.size != 0)
            {
                byte[] managedArray = GetOut.CopyToDotNetByte(ret);
                GetOut.FreeContainer(ret);
                return managedArray;
            }
            throw new FileNotFoundException($"Could not read {m_path}");
        }

        //TODO: broken in backend ?
        public List<AssetView> Ls()
        {
            var tmp = m_lsFiles(m_path);
            string[] paths = Marshal.PtrToStringAnsi(tmp)?.Split("\n");
            GetOut.FreeContainer(new GetOut.Container{ptr=tmp});

            Debug.Assert(paths != null, nameof(paths) + " != null");

            List<AssetView> assetViews = new List<AssetView>(paths.Length-1 );

            foreach (var t in paths)
            {
                if(t is not "")
                    assetViews.Add(new AssetView(t));
            }

            return assetViews;
        }

        public string Path => m_path;
    }
}
