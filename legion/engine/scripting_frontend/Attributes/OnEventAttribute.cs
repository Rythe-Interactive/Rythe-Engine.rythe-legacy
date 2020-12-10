using System;
using JetBrains.Annotations;

namespace Legion.Attributes
{
    [PublicAPI]
    [AttributeUsage(AttributeTargets.Method)]
    public class OnEventAttribute : Attribute
    {
    }
}
