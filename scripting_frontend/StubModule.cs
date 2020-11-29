using System;
using Legion.Attributes;

namespace Legion
{
    [GameSystem]
    class StubModule : GameSystemBase
    {
        public override void Init() => Console.WriteLine("StubModule has been initialized!");


        private int i = 0;
        public override void Update(float dt)
        {
            if (i > 100)
            { 
                Console.WriteLine("StubModule has been updated 100 times");
                i = 0;
            }

            i++;
        }

    }
}
