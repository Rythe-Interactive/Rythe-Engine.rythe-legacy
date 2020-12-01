using System;
using JetBrains.Annotations;

namespace Legion.Attributes
{
    [AttributeUsage(AttributeTargets.Class)]
    [MeansImplicitUse(ImplicitUseTargetFlags.WithMembers)]
    public class GameSystemAttribute : Attribute
    {
    }
}
