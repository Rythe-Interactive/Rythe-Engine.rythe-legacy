using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Legion
{
    abstract class GameSystemBase
    {
        public virtual void Update(float dt)
        {
        }

        public virtual void Init()
        {
        }

    }
}
