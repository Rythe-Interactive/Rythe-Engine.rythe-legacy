using System;
using System.Linq;
using JetBrains.Annotations;

namespace Legion
{
    [PublicAPI]
    public static class Log
    {
        public delegate void LogFn(string param);

        private static LogFn InternalErr;
        private static LogFn InternalWarn;
        private static LogFn InternalInfo;
        private static LogFn InternalDebug;
        private static LogFn InternalTrace;

        [PublicAPI("Used in Backend")]
        public delegate void RegisterLogFnsFn(LogFn err, LogFn warn, LogFn info, LogFn debug, LogFn trace);
        [PublicAPI("Used in Backend")]
        public static void RegisterLogFns(LogFn err, LogFn warn, LogFn info, LogFn debug, LogFn trace)
        {
            InternalErr = err;
            InternalWarn = warn;
            InternalInfo = info;
            InternalDebug = debug;
            InternalTrace = trace;
        }
        private static void Apply(LogFn fn, params object[] args) => fn?.Invoke(
            string.Join(",",args.Select(x => x.ToString()).ToArray()));


        [StringFormatMethod("format")]
        public static void Err(string format, params object[] args) => Apply(InternalErr, string.Format(format,args));
        public static void Warn(string format, params object[] args) => Apply(InternalWarn, string.Format(format,args));
        public static void Info(string format, params object[] args) => Apply(InternalInfo, string.Format(format,args));
        public static void Debug(string format, params object[] args) => Apply(InternalDebug, string.Format(format,args));
        public static void Trace(string format, params object[] args) => Apply(InternalTrace, string.Format(format,args));

        public static void Err(params object[] args) => Apply(InternalErr, args);
        public static void Warn(params object[] args) => Apply(InternalWarn, args);
        public static void Info(params object[] args) => Apply(InternalInfo, args);
        public static void Debug(params object[] args) => Apply(InternalDebug, args);
        public static void Trace(params object[] args) => Apply(InternalTrace, args);
    }
}
