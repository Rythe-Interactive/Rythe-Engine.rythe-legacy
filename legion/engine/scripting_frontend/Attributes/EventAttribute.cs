using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using JetBrains.Annotations;

namespace Legion.Attributes
{
    [PublicAPI, AttributeUsage(AttributeTargets.Struct)]
    public class EventAttribute : Attribute
    {
        public EventAttribute(string name) => Name = name;
        public readonly string Name;
    }
}
