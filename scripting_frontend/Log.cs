namespace Legion
{
    public static class Log
    {
        public delegate void LogFn(string param);

        private static LogFn InternalErr;
        private static LogFn InternalWarn;
        private static LogFn InternalInfo;
        private static LogFn InternalDebug;
        private static LogFn InternalTrace;

        public delegate void RegisterLogFnsFn(LogFn err, LogFn warn, LogFn info, LogFn debug, LogFn trace);
        public static void RegisterLogFns(LogFn err, LogFn warn, LogFn info, LogFn debug, LogFn trace)
        {
            InternalErr = err;
            InternalWarn = warn;
            InternalInfo = info;
            InternalDebug = debug;
            InternalTrace = trace;
        }
        private static void Apply(LogFn fn, object o) => fn?.Invoke(o.ToString());


        public static void Err(object o) => Apply(InternalErr, o);
        public static void Warn(object o) => Apply(InternalWarn, o);
        public static void Info(object o) => Apply(InternalInfo, o);
        public static void Debug(object o) => Apply(InternalDebug, o);
        public static void Trace(object o) => Apply(InternalTrace, o);
    }
}
